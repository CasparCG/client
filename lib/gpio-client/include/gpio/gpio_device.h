#pragma once

#include <string>
#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace gpio {

/**
 * The voltage levels possible on a GPI or GPO port.
 */
enum voltage
{
    HIGH,
    LOW
};

/**
 * Thrown if a GPO handle has expired because the GPO port has been setup again.
 */
class gpo_handle_expired : public std::runtime_error
{
public:
    gpo_handle_expired(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

/**
 * A handle to GPO port provided by gpio_device::setup_gpo_pulse(). The handle
 * allows for triggering the GPO.
 */
class gpo_trigger
{
public:
    typedef boost::shared_ptr<gpo_trigger> ptr;

    virtual ~gpo_trigger() { }

    /**
     * Fire off the GPO now.
     *
     * @throws gpo_handle_expired if the GPO setup has been overwritten.
     */
    virtual void fire() = 0;
};

/**
 * A handle to the GPO port provided by gpio_device::setup_gpo_tally(). The
 * handle allows for switching the state of the tally.
 */
class gpo_switch
{
public:
    typedef boost::shared_ptr<gpo_switch> ptr;

    virtual ~gpo_switch() { }

    /**
     * @param state true if the tally should be turned on, false to turn it off.
     *
     * @throws gpo_handle_expired if the GPO setup has been overwritten.
     */
    virtual void set(bool state) = 0;

    /**
     * @return the current state.
     *
     * @throws gpo_handle_expired if the GPO setup has been overwritten.
     */
    virtual bool get() const = 0;

    /**
     * Toggle the state of the tally.
     *
     * @throws gpo_handle_expired if the GPO setup has been overwritten.
     */
    virtual void toggle() = 0;
};

/**
 * A handler that will be called when a GPI pulse has been detected.
 *
 * The handler should preferably be short-running and non-blocking, because it
 * may be called from and IO thread.
 */
typedef boost::function<void ()> gpi_trigger_handler;

/**
 * A handler that will be called when a GPI tally change has been detected.
 *
 * The handler should preferably be short-running and non-blocking, because it
 * may be called from and IO thread.
 *
 * The bool parameter indicates whether the switch was turned on (true) or off
 * (false).
 */
typedef boost::function<void (bool)> gpi_switch_handler;

/**
 * A listener that will get notified when a GPIO device is connected or
 * disconnected.
 */
typedef boost::function<void (bool)> connection_listener;

/**
 * gpi_switch_handler implementation that delegates to two separate handlers for
 * on and off.
 */
class separated_switch_handler
{
    boost::function<void ()> turned_off_;
    boost::function<void ()> turned_on_;
public:
    /**
     * Constructor.
     *
     * @param turned_off The handler to call when the tally switch is turned off
     * @param turned_on  The handler to call when the tally switch is turned on
     */
    separated_switch_handler(
            const boost::function<void ()>& turned_off,
            const boost::function<void ()>& turned_on)
        : turned_off_(turned_off)
        , turned_on_(turned_on)
    {
    }

    void operator()(bool state)
    {
        if (state)
            turned_on_();
        else
            turned_off_();
    }
};

/**
 * The interface that abstracts the underlying GPIO device implementation.
 */
class gpio_device
{
public:
    typedef boost::shared_ptr<gpio_device> ptr;

    /**
     * Will shutdown any IO threads and stop listening for GPI.
     */
    virtual ~gpio_device() { }

    /**
     * @return the number of GPI ports available on this device.
     */
    virtual int get_num_gpi_ports() const = 0;

    /**
     * @return the number of GPO ports available on this device.
     */
    virtual int get_num_gpo_ports() const = 0;

    /**
     * Setup a GPI port to be used as a pulse input.
     *
     * Will override any previous usage of the GPI port.
     *
     * @param gpi_port              The GPI port on the device to setup.
     * @param silent_state          The voltage to expect when the port is not
     *                              triggered.
     * @param handler               The handler to call every time a valid GPI
     *                              pulse is detected.
     */
    virtual void setup_gpi_pulse(
            int gpi_port,
            voltage silent_state,
            const gpi_trigger_handler& handler) = 0;

    /**
     * Setup a GPI port to be used as a tally input.
     *
     * Will override any previous usage of the GPI port.
     *
     * @param gpi_port  The GPI port on the device to setup.
     * @param off_state The voltage that indicates that the tally switch is off.
     * @param handler   The handler to call every time a tally state change is
     *                  detected.
     */
    virtual void setup_gpi_tally(
            int gpi_port,
            voltage off_state,
            const gpi_switch_handler& handler) = 0;

    /**
     * Stop listening on a GPI port (either used as GPI pulse or tally input).
     *
     * Noop if not listening on the specified port.
     *
     * @param gpi_port The GPI port to stop listening on.
     */
    virtual void stop_gpi(int gpi_port) = 0;

    /**
     * Setup a GPO port for use as a pulse port.
     *
     * Will override any previous usage of the GPO port.
     *
     * @param gpo_port              The GPO port on the device to setup.
     * @param silent_state          The voltage to put out when silent.
     * @param duration_milliseconds The duration in milliseconds of the pulse to
     *                              output when triggered. Cannot be less than
     *                              the value given by
     *                              #get_minimum_supported_duration().
     *
     * @return A reference counted pointer to the gpo_trigger handle to use when
     *         triggering the GPO.
     */
    virtual gpo_trigger::ptr setup_gpo_pulse(
            int gpo_port, voltage silent_state, int duration_milliseconds) = 0;

    /**
     * Setup a GPO port for use as a tally port.
     *
     * Will override any previous usage of the GPO port.
     *
     * @param gpo_port  The GPO port on the device to setup.
     * @param off_state The voltage to output when the tally is turned off.
     *
     * @return A reference counted pointer to the gpo_switch handle to use when
     *         changing the tally state.
     */
    virtual gpo_switch::ptr setup_gpo_tally(
            int gpo_port, voltage off_state) = 0;
};

}
