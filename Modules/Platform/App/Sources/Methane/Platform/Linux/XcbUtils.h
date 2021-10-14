/******************************************************************************

Copyright 2021 Evgeny Gorodetskiy

Licensed under the Apache License, Version 2.0 (the "License"),
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************

FILE: Methane/Platform/Linux/XcbUtils.h
X11/XCB utility functions.

******************************************************************************/

#pragma once

#include <Methane/Platform/Mouse.h>
#include <Methane/Platform/Keyboard.h>

#include <string_view>
#include <array>

#include <xcb/xcb.h>

struct _XDisplay; // X11 display

#ifndef xcb_window_t
using xcb_window_t = uint32_t;
#endif

namespace Methane::Platform::Linux
{

enum class SystemColor : size_t
{
    Background,
    DefaultText,
    ButtonBorderNormal,
    ButtonBorderSelected,
    ButtonBackgroundNormal,
    ButtonBackgroundHovered,
    ButtonBackgroundError,
    ButtonBackgroundErrorHovered,
    ButtonBackgroundWarning,
    ButtonBackgroundWarningHovered,
    ButtonBackgroundPressed,
};

struct RgbColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct WMSizeHints
{
    uint32_t flags;
    int32_t  x, y;
    int32_t  width, height;
    int32_t  min_width, min_height;
    int32_t  max_width, max_height;
    int32_t  width_inc, height_inc;
    int32_t  min_aspect_num, min_aspect_den;
    int32_t  max_aspect_num, max_aspect_den;
    int32_t  base_width, base_height;
    uint32_t win_gravity;
};

enum class NetWmState : uint32_t
{
    Remove, // 0: _NET_WM_STATE_REMOVE
    Add,    // 1: _NET_WM_STATE_ADD
    Toggle  // 2: _NET_WM_STATE_TOGGLE
};

uint32_t PackXcbColor(const RgbColor& color);
uint32_t GetXcbSystemColor(SystemColor color_type);
std::pair<Mouse::Button, int> ConvertXcbMouseButton(xcb_button_t button);
Keyboard::Key ConvertXcbKey(_XDisplay* display, xcb_window_t window, xcb_keycode_t key_detail, uint16_t key_state);
void XcbCheck(xcb_void_cookie_t cookie, xcb_connection_t *connection, std::string_view error_message);
void XcbMeasureText(xcb_connection_t* connection, xcb_font_t font, std::string_view text, uint32_t& width, uint32_t& height, uint32_t& ascent);
xcb_intern_atom_reply_t* GetXcbInternAtomReply(xcb_connection_t* connection, std::string_view name) noexcept;
xcb_atom_t GetXcbInternAtom(xcb_connection_t* xcb_connection, std::string_view name) noexcept;
void SetXcbWindowStringProperty(xcb_connection_t* connection, xcb_window_t window, xcb_atom_enum_t property_id, const std::string_view& value);

template<typename T, size_t atoms_count>
void SetXcbWindowAtomProperty(xcb_connection_t* connection, xcb_window_t window,
                              xcb_atom_t property_id, xcb_atom_enum_t property_type,
                              const std::array<T, atoms_count>& values)
{
    constexpr size_t type_size = sizeof(T) * 8;
    constexpr uint8_t format = static_cast<uint8_t>(std::min(type_size, size_t(32)));
    constexpr uint32_t data_length = values.size() * type_size / format;
    XcbCheck(xcb_change_property_checked(connection, XCB_PROP_MODE_REPLACE, window, property_id, property_type, format, data_length, values.data()),
             connection, "failed to set window property");
}

template<typename T, size_t values_count>
void SetXcbWindowAtomProperty(xcb_connection_t* connection, xcb_window_t window,
                              std::string_view property_atom_name, xcb_atom_enum_t property_type,
                              const std::array<T, values_count>& values)
{
    const xcb_atom_t property_atom = GetXcbInternAtom(connection, property_atom_name.data());
    SetXcbWindowAtomProperty<T, values_count>(connection, window, property_atom, property_type, values);
}

template<typename T>
std::optional<T> GetXcbWindowPropertyValue(xcb_connection_t* connection, xcb_window_t window, xcb_atom_t atom)
{
    xcb_get_property_cookie_t cookie = xcb_get_property(connection, false, window, atom, XCB_ATOM_ATOM, 0, 32);
    xcb_get_property_reply_t* reply = xcb_get_property_reply(connection, cookie, nullptr);
    const std::optional <T> value_opt = reply ? std::optional<T>(*reinterpret_cast<T*>(xcb_get_property_value(reply))) : std::nullopt;
    free(reply);
    return value_opt;
}

} // namespace Methane::Platform::Linux
