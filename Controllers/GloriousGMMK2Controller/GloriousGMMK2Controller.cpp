/*---------------------------------------------------------*\
| GloriousGMMK2Controller.cpp                               |
|                                                           |
|   Driver for Glorious GMMK2 65% (stock firmware)          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "GloriousGMMK2Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

GloriousGMMK2Controller::GloriousGMMK2Controller(hid_device* feature_handle, hid_device* output_handle, const char* path)
{
    feature_dev    = feature_handle;
    output_dev     = output_handle;
    location       = path;
}

GloriousGMMK2Controller::~GloriousGMMK2Controller()
{
    if(feature_dev != nullptr)
    {
        hid_close(feature_dev);
    }

    if(output_dev != nullptr)
    {
        hid_close(output_dev);
    }
}

std::string GloriousGMMK2Controller::GetLocation()
{
    return "HID: " + location;
}

std::string GloriousGMMK2Controller::GetDeviceName()
{
    wchar_t name[128];
    int ret = hid_get_product_string(feature_dev != nullptr ? feature_dev : output_dev, name, 128);
    if(ret != 0)
    {
        return "Glorious GMMK2";
    }

    return StringUtils::wstring_to_string(name);
}

std::string GloriousGMMK2Controller::GetSerial()
{
    wchar_t serial[128];
    hid_device* dev = feature_dev != nullptr ? feature_dev : output_dev;
    if(dev == nullptr)
    {
        return "";
    }

    int ret = hid_get_serial_number_string(dev, serial, 128);
    if(ret != 0)
    {
        return "";
    }

    return StringUtils::wstring_to_string(serial);
}

bool GloriousGMMK2Controller::HasOutputHandle() const
{
    return output_dev != nullptr;
}

void GloriousGMMK2Controller::SetModeDirect()
{
    std::lock_guard<std::mutex> lock(device_mutex);

    LOG_WARNING("[Glorious GMMK2] Direct mode set requested but protocol is not implemented yet");
}

void GloriousGMMK2Controller::SetDirectColors(const std::vector<RGBColor>& /*colors*/)
{
    std::lock_guard<std::mutex> lock(device_mutex);

    LOG_WARNING("[Glorious GMMK2] LED update requested but protocol is not implemented yet");
}
