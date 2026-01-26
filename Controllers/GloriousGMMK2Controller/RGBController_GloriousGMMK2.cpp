/*---------------------------------------------------------*\
| RGBController_GloriousGMMK2.cpp                           |
|                                                           |
|   RGBController for Glorious GMMK2 65% (stock firmware)   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "RGBController_GloriousGMMK2.h"

#define NA 0xFFFFFFFF

static const unsigned int gmmk2_65_ansi_matrix_map[5][16] =
{
    { NA,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14 },
    { NA, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29 },
    { NA, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, NA, 42, 43 },
    { NA, 44, NA, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57 },
    { NA, 58, 59, 60, NA, NA, 61, NA, NA, 62, 63, NA, NA, 64, 65, 66 },
};

static const char* gmmk2_65_ansi_led_names[67] =
{
    KEY_EN_ESCAPE,
    KEY_EN_1,
    KEY_EN_2,
    KEY_EN_3,
    KEY_EN_4,
    KEY_EN_5,
    KEY_EN_6,
    KEY_EN_7,
    KEY_EN_8,
    KEY_EN_9,
    KEY_EN_0,
    KEY_EN_MINUS,
    KEY_EN_EQUALS,
    KEY_EN_BACKSPACE,
    KEY_EN_DELETE,
    KEY_EN_TAB,
    KEY_EN_Q,
    KEY_EN_W,
    KEY_EN_E,
    KEY_EN_R,
    KEY_EN_T,
    KEY_EN_Y,
    KEY_EN_U,
    KEY_EN_I,
    KEY_EN_O,
    KEY_EN_P,
    KEY_EN_LEFT_BRACKET,
    KEY_EN_RIGHT_BRACKET,
    KEY_EN_ANSI_BACK_SLASH,
    KEY_EN_PAGE_UP,
    KEY_EN_CAPS_LOCK,
    KEY_EN_A,
    KEY_EN_S,
    KEY_EN_D,
    KEY_EN_F,
    KEY_EN_G,
    KEY_EN_H,
    KEY_EN_J,
    KEY_EN_K,
    KEY_EN_L,
    KEY_EN_SEMICOLON,
    KEY_EN_QUOTE,
    KEY_EN_ANSI_ENTER,
    KEY_EN_PAGE_DOWN,
    KEY_EN_LEFT_SHIFT,
    KEY_EN_Z,
    KEY_EN_X,
    KEY_EN_C,
    KEY_EN_V,
    KEY_EN_B,
    KEY_EN_N,
    KEY_EN_M,
    KEY_EN_COMMA,
    KEY_EN_PERIOD,
    KEY_EN_FORWARD_SLASH,
    KEY_EN_RIGHT_SHIFT,
    KEY_EN_UP_ARROW,
    KEY_EN_END,
    KEY_EN_LEFT_CONTROL,
    KEY_EN_LEFT_WINDOWS,
    KEY_EN_LEFT_ALT,
    KEY_EN_SPACE,
    KEY_EN_RIGHT_ALT,
    KEY_EN_RIGHT_FUNCTION,
    KEY_EN_LEFT_ARROW,
    KEY_EN_DOWN_ARROW,
    KEY_EN_RIGHT_ARROW,
};

RGBController_GloriousGMMK2::RGBController_GloriousGMMK2(GloriousGMMK2Controller* controller_ptr)
{
    controller                 = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "Glorious";
    type                        = DEVICE_TYPE_KEYBOARD;
    description                 = "Glorious GMMK2 65%";
    location                    = controller->GetLocation();
    serial                      = controller->GetSerial();

    mode direct;
    direct.name                 = "Direct";
    direct.value                = 0xFFFF;
    direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(direct);

    SetupZones();
}

RGBController_GloriousGMMK2::~RGBController_GloriousGMMK2()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != nullptr)
        {
            delete zones[zone_index].matrix_map;
        }
    }

    delete controller;
}

void RGBController_GloriousGMMK2::SetupZones()
{
    zone keyboard_zone;
    keyboard_zone.name               = ZONE_EN_KEYBOARD;
    keyboard_zone.type               = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min           = 67;
    keyboard_zone.leds_max           = 67;
    keyboard_zone.leds_count         = 67;

    keyboard_zone.matrix_map         = new matrix_map_type;
    keyboard_zone.matrix_map->height = 5;
    keyboard_zone.matrix_map->width  = 16;
    keyboard_zone.matrix_map->map    = (unsigned int*)&gmmk2_65_ansi_matrix_map;

    zones.push_back(keyboard_zone);

    for(unsigned int led_idx = 0; led_idx < 67; led_idx++)
    {
        led new_led;
        new_led.name = gmmk2_65_ansi_led_names[led_idx];
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_GloriousGMMK2::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_GloriousGMMK2::DeviceUpdateLEDs()
{
    controller->SetDirectColors(colors);
}

void RGBController_GloriousGMMK2::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GloriousGMMK2::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GloriousGMMK2::DeviceUpdateMode()
{
    controller->SetModeDirect();
}
