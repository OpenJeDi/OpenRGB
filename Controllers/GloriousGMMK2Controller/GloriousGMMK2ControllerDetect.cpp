/*---------------------------------------------------------*\
| GloriousGMMK2ControllerDetect.cpp                         |
|                                                           |
|   Detector for Glorious GMMK2 65%                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <hidapi.h>
#include "Detector.h"
#include "GloriousGMMK2Controller.h"
#include "RGBController_GloriousGMMK2.h"
#include "LogManager.h"

#define GLORIOUS_VID                     0x320F
#define GLORIOUS_GMMK2_65_ANSI_PID       0x5045
#define GLORIOUS_GMMK2_FEATURE_USAGE_PAGE 0xFF01
#define GLORIOUS_GMMK2_FEATURE_USAGE      0x0001
#define GLORIOUS_GMMK2_OUTPUT_USAGE_PAGE  0xFF00
#define GLORIOUS_GMMK2_OUTPUT_USAGE       0xFF00

static hid_device* FindOutputInterface(const hid_device_info* info)
{
    hid_device_info* device_info = hid_enumerate(info->vendor_id, info->product_id);
    hid_device_info* current     = device_info;
    hid_device*      output_dev  = nullptr;

    while(current != nullptr)
    {
        if(current->usage_page == GLORIOUS_GMMK2_OUTPUT_USAGE_PAGE && current->usage == GLORIOUS_GMMK2_OUTPUT_USAGE)
        {
            output_dev = hid_open_path(current->path);
            break;
        }

        current = current->next;
    }

    hid_free_enumeration(device_info);
    return output_dev;
}

void DetectGloriousGMMK2(hid_device_info* info, const std::string& /*name*/)
{
    hid_device* feature_dev = hid_open_path(info->path);

    if(feature_dev)
    {
        hid_device* output_dev = FindOutputInterface(info);

        if(output_dev == nullptr)
        {
            LOG_WARNING("[Glorious GMMK2] Output interface not found, LED writes will be disabled");
        }

        GloriousGMMK2Controller* controller = new GloriousGMMK2Controller(feature_dev, output_dev, info->path);
        RGBController_GloriousGMMK2* rgb_controller = new RGBController_GloriousGMMK2(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_HID_DETECTOR_PU("Glorious GMMK2 65% (ANSI)", DetectGloriousGMMK2, GLORIOUS_VID, GLORIOUS_GMMK2_65_ANSI_PID, GLORIOUS_GMMK2_FEATURE_USAGE_PAGE, GLORIOUS_GMMK2_FEATURE_USAGE);
