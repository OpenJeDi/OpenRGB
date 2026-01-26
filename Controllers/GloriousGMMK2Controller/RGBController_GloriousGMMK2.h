/*---------------------------------------------------------*\
| RGBController_GloriousGMMK2.h                             |
|                                                           |
|   RGBController for Glorious GMMK2 65% (stock firmware)   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GloriousGMMK2Controller.h"

enum class GMMK2Layout
{
    ANSI,
    ISO,
};

class RGBController_GloriousGMMK2 : public RGBController
{
public:
    RGBController_GloriousGMMK2(GloriousGMMK2Controller* controller_ptr, GMMK2Layout layout);
    ~RGBController_GloriousGMMK2();

    void SetupZones() override;
    void ResizeZone(int zone, int new_size) override;

    void DeviceUpdateLEDs() override;
    void UpdateZoneLEDs(int zone) override;
    void UpdateSingleLED(int led) override;

    void DeviceUpdateMode() override;

private:
    GloriousGMMK2Controller* controller;
    GMMK2Layout               layout;
};
