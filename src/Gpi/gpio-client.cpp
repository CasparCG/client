#include "gpio-client.h"

#include <iostream>
#include <string>
#include <queue>
#include <cmath>
#include <map>
#include <memory>
#include <atomic>
#include <chrono>
#include <thread>

#include <boost/asio.hpp>
#include <boost/thread/future.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>

using namespace boost::asio;

namespace gpio
{

    struct gpi_port_handler
    {
        virtual ~gpi_port_handler() {}

        virtual void handle(bool port_state) = 0;
    };

    class gpi_port_pulse_handler : public gpi_port_handler
    {
        voltage silent_state_;
        bool last_port_state_;
        gpi_trigger_handler handler_;

    public:
        gpi_port_pulse_handler(
            voltage silent_state,
            const gpi_trigger_handler &handler)
            : silent_state_(silent_state), last_port_state_(false), handler_(handler)
        {
        }

        virtual void handle(bool port_state)
        {
            if (silent_state_ == LOW && !last_port_state_ && port_state) // RISING
                handler_();
            else if (silent_state_ == HIGH && last_port_state_ && !port_state) // FALLING
                handler_();

            last_port_state_ = port_state;
        }
    };

    class gpi_port_tally_handler : public gpi_port_handler
    {
        voltage off_state_;
        gpi_switch_handler handler_;

    public:
        gpi_port_tally_handler(
            voltage off_state, const gpi_switch_handler &handler)
            : off_state_(off_state), handler_(handler)
        {
        }

        virtual void handle(bool port_state)
        {
            if (off_state_ == LOW)
                handler_(port_state);
            else
                handler_(!port_state);
        }
    };

    typedef std::array<char, 4> payload_data;

    struct write_package
    {
        payload_data payload;
        std::function<void()> completion_handler;

        write_package()
        {
            payload[2] = '\r';
            payload[3] = '\n';
        }
    };

    template <class Func>
    class delayed_task
    {
        Func task_;

    public:
        delayed_task(const Func &task)
            : task_(task)
        {
        }

        void operator()(const boost::system::error_code &e)
        {
            if (!e)
                task_();
        }
    };

    class writer : public std::enable_shared_from_this<writer>
    {
        io_service &service;
        std::queue<write_package> &outgoing;
        std::function<void()> write_pending;
        deadline_timer timer;

    public:
        writer(
            io_service &service,
            std::queue<write_package> &outgoing,
            const std::function<void()> &write_pending)
            : service(service), outgoing(outgoing), write_pending(write_pending), timer(service)
        {
        }

        void write_silence(int gpo, voltage silent_state)
        {
            write_package package;
            package.payload[0] = '0' + gpo;
            package.payload[1] = silent_state == LOW ? '0' : '1';
            write(package);
        }

        void write(const write_package &package)
        {
            service.post(bind(&writer::do_write, shared_from_this(), package));
        }

        template <class Func>
        void post(const Func &task)
        {
            service.post(task);
        }

        template <class Func>
        void post_delayed(long millis_delayed, const Func &task)
        {
            timer.expires_from_now(
                boost::posix_time::milliseconds(millis_delayed));
            timer.async_wait(delayed_task<Func>(task));
        }

    private:
        void do_write(const write_package &package)
        {
            outgoing.push(package);
            write_pending();
        }
    };

    class gpo_port_handler
    {
    public:
        virtual ~gpo_port_handler() {}

        virtual void device_connected() = 0;
    };

    class gpo_switch_impl : public gpo_switch, public gpo_port_handler
    {
        int port_;
        voltage off_state_;
        bool current_state_;
        std::weak_ptr<writer> writer_;

    public:
        gpo_switch_impl(int port, voltage off_state, std::weak_ptr<writer> writer)
            : port_(port), off_state_(off_state), writer_(std::move(writer))
        {
            set(false);
        }

        virtual void device_connected()
        {
            set(get());
        }

        virtual void set(bool state)
        {
            std::shared_ptr<writer> strong = writer_.lock();

            if (strong)
            {
                write_package package;
                package.payload[0] = '0' + port_;
                package.payload[1] = off_state_ == LOW
                                         ? (state ? '1' : '0')
                                         : (state ? '0' : '1');

                strong->write(package);
            }
            else
            {
                throw gpo_handle_expired("GPO switch is expired");
            }
        }

        virtual bool get() const
        {
            return current_state_;
        }

        virtual void toggle()
        {
            set(!current_state_);
        }
    };

    class gpo_trigger_impl : public gpo_trigger, public gpo_port_handler
    {
        int port_;
        voltage silent_state_;
        int duration_milliseconds_;
        std::weak_ptr<writer> writer_;
        int pending;
        bool triggering;

    public:
        gpo_trigger_impl(
            int port,
            voltage silent_state,
            int duration_milliseconds,
            const std::shared_ptr<writer> &writer)
            : port_(port), silent_state_(silent_state), duration_milliseconds_(duration_milliseconds), writer_(writer), pending(0), triggering(false)
        {
            writer->write_silence(port, silent_state);
        }

        virtual void device_connected()
        {
            if (pending == 0 && !triggering)
            {
                std::shared_ptr<writer> strong = writer_.lock();

                if (strong)
                    strong->write_silence(port_, silent_state_);
            }
        }

        virtual void fire()
        {
            std::shared_ptr<writer> strong = writer_.lock();

            if (strong)
            {
                strong->post(bind(&gpo_trigger_impl::increment_and_process,
                                  this, strong));
            }
            else
            {
                throw gpo_handle_expired("GPO trigger is expired");
            }
        }

    private:
        void increment_and_process(const std::shared_ptr<writer> &strong)
        {
            ++pending;
            process_pending(strong);
        }

        void process_pending(const std::shared_ptr<writer> &writer)
        {
            if (pending == 0 || triggering)
                return;

            write_package package;
            package.payload[0] = port_ + '0';
            package.payload[1] = silent_state_ == LOW ? '1' : '0';
            package.completion_handler = bind(
                &gpo_trigger_impl::schedule_delayed_pulse_off, this, writer);

            writer->write(package);
            triggering = true;
        }

        void schedule_delayed_pulse_off(const std::shared_ptr<writer> &writer)
        {
            writer->post_delayed(duration_milliseconds_,
                                 bind(&gpo_trigger_impl::write_pulse_off, this, writer));
        }

        void write_pulse_off(const std::shared_ptr<writer> &writer)
        {
            write_package package;
            package.payload[0] = port_ + '0';
            package.payload[1] = silent_state_ == LOW ? '0' : '1';
            package.completion_handler =
                std::bind(&gpo_trigger_impl::written_pulse_off, this);

            writer->write(package);
        }

        void written_pulse_off()
        {
            triggering = false;

            if (--pending == 0)
                return;

            std::shared_ptr<gpio::writer> strong = writer_.lock();

            if (strong)
            {
                process_pending(strong);
            }
        }
    };

    struct connection_details
    {
        io_service service;
        boost::asio::serial_port serial_port;
        std::string serial_port_name;
        int baud_rate;
        deadline_timer timer;

        connection_details(
            std::string serial_port_name,
            int baud_rate)
            : serial_port(service), serial_port_name(serial_port_name), baud_rate(baud_rate), timer(service)
        {
        }
    };

    enum serial_port_state
    {
        DISCONNECTED,
        HANDSHAKE,
        CONNECTED
    };

    struct serial_port_device::impl
    {
        connection_details conn;
        std::array<char, 4> read_buffer;
        boost::ptr_unordered_map<int, gpi_port_handler> gpi_handlers;
        std::map<int, std::weak_ptr<gpo_port_handler>> gpo_handlers;
        std::map<int, std::shared_ptr<writer>> writers;
        std::queue<write_package> outgoing;
        bool writing;
        bool got_keep_alive;
        serial_port_state state;
        connection_listener conn_listener;
        boost::promise<int> gpi_fetched;
        boost::promise<int> gpo_fetched;
        boost::shared_future<int> num_gpi;
        boost::shared_future<int> num_gpo;
        std::thread io_thread;
        std::atomic<bool> shutdown;

        impl(const std::string &serial_port_name,
             int baud_rate,
             const connection_listener &connection_listener)
            : conn(serial_port_name, baud_rate), state(DISCONNECTED), conn_listener(connection_listener), shutdown(false)
        {
            conn.service.post(std::bind(&impl::connect, this));
            io_thread = std::thread(std::bind(&impl::run, this));
        }

        void disconnected()
        {
            if (state == DISCONNECTED)
                return;

            gpi_fetched.set_value(-1);
            gpo_fetched.set_value(-1);

            if (state == CONNECTED)
                conn_listener(false);

            state = DISCONNECTED;

            schedule_connect();
        }

        void disconnect()
        {
            if (state == DISCONNECTED)
                return;

            conn.timer.cancel();
            conn.serial_port.cancel();
            conn.serial_port.close();

            disconnected();
        }

        void schedule_connect()
        {
            delay(300, std::bind(&impl::connect, this));
        }

        void connect()
        {
            try
            {
                conn.serial_port.open(conn.serial_port_name);
            }
            catch (const std::exception &e)
            {
                schedule_connect();
                return;
            }

            conn.serial_port.set_option(
                serial_port_base::baud_rate(conn.baud_rate));
            conn.serial_port.set_option(serial_port_base::character_size(8));
            conn.serial_port.set_option(serial_port_base::flow_control(
                serial_port_base::flow_control::none));
            conn.serial_port.set_option(serial_port_base::parity(
                serial_port_base::parity::none));
            conn.serial_port.set_option(serial_port_base::stop_bits(
                serial_port_base::stop_bits::one));

            gpi_fetched = boost::promise<int>();
            gpo_fetched = boost::promise<int>();
            num_gpi = gpi_fetched.get_future();
            num_gpo = gpo_fetched.get_future();
            writing = false;

            read_next();
            send_keep_alive();
        }

        void connected()
        {
            state = CONNECTED;
            conn_listener(true);

            typedef std::pair<const int, std::weak_ptr<gpo_port_handler>> entry;

            for (const entry &pair : gpo_handlers)
            {
                std::shared_ptr<gpo_port_handler> strong = pair.second.lock();

                if (strong)
                    strong->device_connected();
            }
        }

        void schedule_keep_alive()
        {
            delay(2000, std::bind(&impl::send_keep_alive, this));
        }

        void send_keep_alive()
        {
            write_package package;
            package.payload[0] = 'a';
            package.payload[1] = '?';
            package.completion_handler =
                std::bind(&impl::schedule_keepalive_timeout, this);
            got_keep_alive = false;
            outgoing.push(package);
            write_pending();
        }

        void schedule_keepalive_timeout()
        {
            delay(500, std::bind(&impl::verify_keepalive_timeout, this));
        }

        void verify_keepalive_timeout()
        {
            if (got_keep_alive)
            {
                if (state == DISCONNECTED || state == HANDSHAKE)
                    connected();

                schedule_keep_alive();
            }
            else if (state == DISCONNECTED)
            {
                state = HANDSHAKE;
                schedule_keep_alive();
            }
            else
            {
                disconnect();
            }
        }

        void run()
        {
            conn_listener(false);

            while (!shutdown)
            {
                try
                {
                    conn.service.run();
                    conn.service.reset();
                }
                catch (const std::exception &e)
                {
                    std::cerr << "Exception: " << e.what() << std::endl;
                    throw;
                }
            }
        }

        template <class Func>
        void delay(long delay_millis, const Func &command)
        {
            conn.timer.expires_from_now(boost::posix_time::milliseconds(delay_millis));
            conn.timer.async_wait(delayed_task<Func>(command));
        }

        void request_gpi()
        {
            write_package package;
            package.payload[0] = 'i';
            package.payload[1] = '?';

            outgoing.push(package);
            write_pending();
        }

        void request_gpo()
        {
            write_package package;
            package.payload[0] = 'o';
            package.payload[1] = '?';

            outgoing.push(package);
            write_pending();
        }

        void read_next()
        {
            async_read(
                conn.serial_port,
                buffer(read_buffer),
                transfer_exactly(read_buffer.size()),
                std::bind(&impl::on_read, this, std::placeholders::_1, std::placeholders::_2));
        }

        void on_read(const boost::system::error_code &e, std::size_t bytes_transferred)
        {
            if (e)
            {
                disconnect();
                return;
            }

            if (bytes_transferred == read_buffer.size())
            {
                char byte1 = read_buffer[0];
                char byte2 = read_buffer[1];

                switch (byte1)
                {
                case 'i':
                    if (!num_gpi.is_ready())
                        gpi_fetched.set_value(byte2 - '0');

                    break;
                case 'o':
                    if (!num_gpo.is_ready())
                        gpo_fetched.set_value(byte2 - '0');

                    break;
                case 'a':
                    if (byte2 == '!')
                    {
                        got_keep_alive = true;
                        // verify_keepalive_timeout();
                    }

                    break;
                default:
                    char port = byte1;
                    int gpi_port = port - '0';

                    boost::ptr_unordered_map<int, gpi_port_handler>::iterator iter =
                        gpi_handlers.find(gpi_port);

                    if (iter != gpi_handlers.end())
                        iter->second->handle(byte2 == '1');
                    else
                        std::cout << "Unsubsribed GPI event on port " << gpi_port
                                  << std::endl;
                    break;
                }
            }

            read_next();
        }

        void write_pending()
        {
            if (outgoing.empty() || writing)
                return;

            write_package &package = outgoing.front();

            writing = true;
            async_write(
                conn.serial_port,
                buffer(package.payload),
                transfer_exactly(package.payload.size()),
                std::bind(&impl::on_wrote_pending, this, std::placeholders::_1, std::placeholders::_2));
        }

        void on_wrote_pending(
            const boost::system::error_code &e, std::size_t bytes_transferred)
        {
            if (e)
            {
                disconnect();
                return;
            }

            write_package &package = outgoing.front();

            if (bytes_transferred != package.payload.size())
                return;

            if (package.completion_handler)
            {
                package.completion_handler();
            }

            outgoing.pop();

            writing = false;
            write_pending();
        }

        int get_num_gpi_ports()
        {
            if (state != CONNECTED)
                return -1;

            if (!num_gpi.is_ready())
            {
                request_gpi();

                if (!num_gpi.timed_wait(boost::posix_time::milliseconds(1000)))
                    return -1;
            }

            return num_gpi.get();
        }

        int get_num_gpo_ports()
        {
            if (state != CONNECTED)
                return -1;

            if (!num_gpo.is_ready())
            {
                request_gpo();

                if (!num_gpo.timed_wait(boost::posix_time::milliseconds(1000)))
                    return -1;
            }

            return num_gpo.get();
        }

        std::shared_ptr<writer> new_writer(int gpo_port)
        {
            std::shared_ptr<writer> result(new writer(
                conn.service,
                outgoing,
                std::bind(&impl::write_pending, this)));
            conn.service.post(
                bind(&impl::do_insert_writer, this, gpo_port, result));

            return result;
        }

        void do_insert_writer(int gpo_port, const std::shared_ptr<writer> &writer)
        {
            writers.erase(gpo_port);
            writers.insert(std::make_pair(gpo_port, writer));
        }

        void setup_gpi_pulse(
            int gpi_port,
            voltage silent_state,
            const gpi_trigger_handler &handler)
        {
            conn.service.post(bind(&impl::do_setup_gpi_pulse, this,
                                   gpi_port, silent_state, handler));
        }

        void do_setup_gpi_pulse(
            int gpi_port,
            voltage silent_state,
            const gpi_trigger_handler &handler)
        {
            gpi_handlers.erase(gpi_port);
            gpi_handlers.insert(
                gpi_port,
                new gpi_port_pulse_handler(silent_state, handler));
        }

        void setup_gpi_tally(
            int gpi_port,
            voltage off_state,
            const gpi_switch_handler &handler)
        {
            conn.service.post(bind(&impl::do_setup_gpi_tally, this,
                                   gpi_port, off_state, handler));
        }

        void do_setup_gpi_tally(
            int gpi_port,
            voltage off_state,
            const gpi_switch_handler &handler)
        {
            gpi_handlers.erase(gpi_port);
            gpi_handlers.insert(
                gpi_port,
                new gpi_port_tally_handler(off_state, handler));
        }

        void stop_gpi(int gpi_port)
        {
            conn.service.post(std::bind(&impl::do_stop_gpi, this, gpi_port));
        }

        void do_stop_gpi(int gpi_port)
        {
            gpi_handlers.erase(gpi_port);
        }

        gpo_trigger::ptr setup_gpo_pulse(
            int gpo_port, voltage silent_state, int duration_milliseconds)
        {
            std::shared_ptr<gpo_trigger_impl> trigger(new gpo_trigger_impl(
                gpo_port,
                silent_state,
                duration_milliseconds,
                new_writer(gpo_port)));

            conn.service.post(
                bind(&impl::do_insert_gpo_handler, this, gpo_port, trigger));

            return trigger;
        }

        void do_insert_gpo_handler(
            int gpo_port, const std::shared_ptr<gpo_port_handler> &handler)
        {
            gpo_handlers.erase(gpo_port);
            gpo_handlers.insert(std::make_pair(gpo_port, handler));
        }

        gpo_switch::ptr setup_gpo_tally(int gpo_port, voltage off_state)
        {
            std::shared_ptr<gpo_switch_impl> tally(new gpo_switch_impl(
                gpo_port, off_state, new_writer(gpo_port)));

            conn.service.post(
                bind(&impl::do_insert_gpo_handler, this, gpo_port, tally));

            return tally;
        }

        ~impl()
        {
            shutdown = true;
            conn.service.stop();
            io_thread.join();
        }
    };

    serial_port_device::serial_port_device(
        const std::string &serial_port,
        int baud_rate,
        const connection_listener &connection_listener)
        : impl_(new impl(serial_port, baud_rate, connection_listener))
    {
    }

    gpio_device::ptr serial_port_device::create(
        const std::string &serial_port,
        int baud_rate,
        const connection_listener &connection_listener)
    {
        return gpio_device::ptr(new serial_port_device(
            serial_port, baud_rate, connection_listener));
    }

    serial_port_device::~serial_port_device()
    {
        impl_.reset();
        // Cannot reopen otherwise
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int serial_port_device::get_num_gpi_ports() const
    {
        return impl_->get_num_gpi_ports();
    }

    int serial_port_device::get_num_gpo_ports() const
    {
        return impl_->get_num_gpo_ports();
    }

    void serial_port_device::setup_gpi_pulse(
        int gpi_port,
        voltage silent_state,
        const gpi_trigger_handler &handler)
    {
        impl_->setup_gpi_pulse(gpi_port, silent_state, handler);
    }

    void serial_port_device::setup_gpi_tally(
        int gpi_port,
        voltage off_state,
        const gpi_switch_handler &handler)
    {
        impl_->setup_gpi_tally(gpi_port, off_state, handler);
    }

    void serial_port_device::stop_gpi(int gpi_port)
    {
        impl_->stop_gpi(gpi_port);
    }

    gpo_trigger::ptr serial_port_device::setup_gpo_pulse(
        int gpo_port, voltage silent_state, int duration_milliseconds)
    {
        return impl_->setup_gpo_pulse(gpo_port, silent_state, duration_milliseconds);
    }

    gpo_switch::ptr serial_port_device::setup_gpo_tally(
        int gpo_port, voltage off_state)
    {
        return impl_->setup_gpo_tally(gpo_port, off_state);
    }

}
