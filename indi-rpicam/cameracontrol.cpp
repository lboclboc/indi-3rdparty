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

#include <stdio.h>
#include <bcm_host.h>
#include <stdexcept>
#include <chrono>

#include <mmal_logging.h>
#include "cameracontrol.h"
#include "mmalexception.h"
#include "pipeline.h"

CameraControl::CameraControl()
{
    camera.reset(new MMALCamera(0));

    encoder.reset(new MMALEncoder());
    encoder->add_buffer_listener(this);

    camera->connect(MMALCamera::CAPTURE_PORT_NO, encoder.get(), 0); // Connected the capture port to the encoder.
}

CameraControl::~CameraControl()
{
    camera->disconnect();
    encoder.reset();
}

void CameraControl::startCapture()
{

    camera->setCapturePortFormat();

    camera->setExposureParameters();


    encoder->activate();
    if (capture_listeners.size() == 0) {
        throw std::runtime_error("No capture listeners registered, start_capture not possible.");
    }

    camera->startCapture();
    start_time = std::chrono::steady_clock::now();
    print_first = true;
}

void CameraControl::stopCapture()
{
    camera->stopCapture();
    std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start_time;
    fprintf(stderr, "%s: exposure stopped after %f s\n", __FUNCTION__, diff.count());
}

/**
 * @brief Buffer received from a port.
 * @param port
 * @param buffer
 */
void CameraControl::buffer_received(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer)
{
    if (port->type == MMAL_PORT_TYPE_OUTPUT)
    {
        assert(buffer->type->video.planes == 1);

        if (buffer->length) {
            signal_data_received(buffer->data + buffer->offset, buffer->length);
        }

        // Signal if completed
        if (buffer->flags & (MMAL_BUFFER_HEADER_FLAG_EOS | MMAL_BUFFER_HEADER_FLAG_FRAME_END | MMAL_BUFFER_HEADER_FLAG_TRANSMISSION_FAILED)) {
            signal_complete();
        }
    }
}

void CameraControl::signal_data_received(uint8_t *data, uint32_t length)
{
    if (print_first) {
        std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start_time;
        fprintf(stderr, "%s: first buffer received after %f s\n", __FUNCTION__, diff.count());
        print_first = false;
    }

    for(auto p : pipelines) {
        p->data_received(data, length);
    }
}

void CameraControl::signal_complete()
{
    std::chrono::duration<double> diff = std::chrono::steady_clock::now() - start_time;
    fprintf(stderr, "%s: all buffers received after %f s\n", __FUNCTION__, diff.count());
    for(auto p : capture_listeners) {
        p->capture_complete();
    }
    stopCapture();
}
