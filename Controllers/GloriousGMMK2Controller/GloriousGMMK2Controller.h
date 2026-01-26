/*---------------------------------------------------------*\
| GloriousGMMK2Controller.h                                 |
|                                                           |
|   Driver for Glorious GMMK2 65% (stock firmware)          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <hidapi.h>
#include <string>
#include <vector>
#include <mutex>
#include "RGBController.h"

class GloriousGMMK2Controller
{
public:
    GloriousGMMK2Controller(hid_device* feature_handle, hid_device* output_handle, const char* path);
    ~GloriousGMMK2Controller();

    std::string GetLocation();
    std::string GetDeviceName();
    std::string GetSerial();

    bool HasOutputHandle() const;

    void SetDirectColors(const std::vector<RGBColor>& colors);
    void SetModeDirect();

private:
    hid_device* feature_dev;
    hid_device* output_dev;
    std::string location;
    std::mutex  device_mutex;
};
