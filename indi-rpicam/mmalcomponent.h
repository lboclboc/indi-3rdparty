/*
 Raspberry Pi High Quality Camera CCD Driver for Indi.
 Copyright (C) 2020 Lars Berntzon (lars.berntzon@cecilia-data.se).
 All rights reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef MMALCOMPONENT_H
#define MMALCOMPONENT_H

#include <vector>
#include <mmal_component.h>
#include <mmal_pool.h>

class MMALBufferListener;
class MMAL_CONNECTION_T;

/**
 * The MMALComponent class Base class for MMAL component wrappers MMALCamera and MMALEncoder
 *
 * Dealing with connections and callbacks basics.
 */
class MMALComponent
{
public:
    MMALComponent(const char *component_type);
    virtual ~MMALComponent();

    /**
     * Connect my output port to port of other component
     *
     * Note, only one connection at a time is possible for this mmal_component wrapper.
     *
     * @param src_port The source port number of this component.
     * @param dst component to connect to.
     * @param dst_port destination input port of other component.
     *
     * FIXME: Support multiple connections.
    */
    void connect(int src_port, MMALComponent *dst, int dst_port);

    /**
     * Disconnect my connection to other components.
     */
    void disconnect();

    /**
     * Add a MMALBufferListerner interface componet to the vector of listeners to 
     * receive port callbacks.
     */
    void add_buffer_listener(MMALBufferListener *l) { buffer_listeners.push_back(l); }

protected:
    virtual void callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
    virtual void return_buffer(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
    void enable_port_with_callback(MMAL_PORT_T *port);
    MMAL_COMPONENT_T *component {};
    MMAL_CONNECTION_T *connection {};

private:
    /**
     * @brief MMALComponent::c_callback Wraps a simple C-callback to a C++ object callback.
     * Uses the userdata as a pointer to the object to be called.
     * @param port MMAL Component port
     * @param buffer MMAL Buffer of data
     */
    static void c_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);

    std::vector<MMALBufferListener *>buffer_listeners;
};

#endif // MMALCOMPONENT_H
