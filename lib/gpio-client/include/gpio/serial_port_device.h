#pragma once

#include <boost/scoped_ptr.hpp>

#include "gpio_device.h"

namespace gpio {

/**
 * Serial port attached device (Arduino, IoCore or similar) with a very simple
 * ASCII protocol:
 *
 * Set GPO 0 to HIGH voltage:
 *
 * Send '0' for port 0 followed by '1' for HIGH
 * end with \r\n
 *
 * The same is sent the other way whenever the device detects a voltage change
 * on one of the GPI ports.
 */
class serial_port_device : public gpio_device
{
    serial_port_device(
            const std::string& serial_port,
            int baud_rate,
            const connection_listener& connection_listener);
public:
    /**
     * Create a serial port GPIO device.
     *
     * @param serial_port         The serial port to use.
     * @param baud_rate           The baud rate to use.
     * @param connection_listener The connection listener to use.
     *
     * @return A reference counted pointer to the new instance.
     */
    static ptr create(
            const std::string& serial_port,
            int baud_rate,
            const connection_listener& connection_listener);

    /**
     * Closes the serial port.
     */
    virtual ~serial_port_device();

    /** @{inheritDoc} */
    virtual int get_num_gpi_ports() const;

    /** @{inheritDoc} */
    virtual int get_num_gpo_ports() const;

    /** @{inheritDoc} */
    virtual void setup_gpi_pulse(
            int gpi_port,
            voltage silent_state,
            const gpi_trigger_handler& handler);

    /** @{inheritDoc} */
    virtual void setup_gpi_tally(
            int gpi_port,
            voltage off_state,
            const gpi_switch_handler& handler);

    /** @{inheritDoc} */
    virtual void stop_gpi(int gpi_port);

    /** @{inheritDoc} */
    virtual gpo_trigger::ptr setup_gpo_pulse(
            int gpo_port, voltage silent_state, int duration_milliseconds);

    /** @{inheritDoc} */
    virtual gpo_switch::ptr setup_gpo_tally(int gpo_port, voltage off_state);
private:
    struct impl;
    boost::scoped_ptr<impl> impl_;
};

}
