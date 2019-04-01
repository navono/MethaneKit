/******************************************************************************

Copyright 2019 Evgeny Gorodetskiy

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

*******************************************************************************

FILE: Methane/Platform/Keyboard.cpp
Platform abstraction of keyboard events.

******************************************************************************/

#include <Methane/Platform/Keyboard.h>

#include <map>
#include <sstream>
#include <cassert>

using namespace Methane::Platform::Keyboard;

const Modifier::Values Modifier::values;
static const std::string s_keys_separator = "+";

std::string KeyConverter::ToString() const
{
    static const std::map<Key, std::string> s_name_by_key =
    {
        // Printable keys
        { Key::Space,           "SPACE"         },
        { Key::Apostrophe,      "'"             },
        { Key::Comma,           ","             },
        { Key::Minus,           "-"             },
        { Key::Period,          "."             },
        { Key::Slash,           "/"             },
        { Key::Num0,            "0"             },
        { Key::Num1,            "1"             },
        { Key::Num2,            "2"             },
        { Key::Num3,            "3"             },
        { Key::Num4,            "4"             },
        { Key::Num5,            "5"             },
        { Key::Num6,            "6"             },
        { Key::Num7,            "7"             },
        { Key::Num8,            "8"             },
        { Key::Num9,            "9"             },
        { Key::Semicolon,       ";"             },
        { Key::Equal,           "="             },
        { Key::A,               "A"             },
        { Key::B,               "B"             },
        { Key::C,               "C"             },
        { Key::D,               "D"             },
        { Key::E,               "E"             },
        { Key::F,               "F"             },
        { Key::G,               "G"             },
        { Key::H,               "H"             },
        { Key::I,               "I"             },
        { Key::J,               "J"             },
        { Key::K,               "K"             },
        { Key::L,               "L"             },
        { Key::M,               "M"             },
        { Key::N,               "N"             },
        { Key::O,               "O"             },
        { Key::P,               "P"             },
        { Key::Q,               "Q"             },
        { Key::R,               "R"             },
        { Key::S,               "S"             },
        { Key::T,               "T"             },
        { Key::U,               "U"             },
        { Key::V,               "V"             },
        { Key::W,               "W"             },
        { Key::X,               "X"             },
        { Key::Y,               "Y"             },
        { Key::Z,               "Z"             },
        { Key::LeftBracket,     "["             },
        { Key::BackSlash,       "\\"            },
        { Key::RightBracket,    "]"             },
        { Key::GraveAccent,     "`"             },
        { Key::World1,          "W1"            },
        { Key::World2,          "W2"            },
        
        // Function Keys
        { Key::Escape,          "ESC"           },
        { Key::Enter,           "ENTER"         },
        { Key::Tab,             "TAB"           },
        { Key::Backspace,       "BACKSPACE"     },
        { Key::Insert,          "INSERT"        },
        { Key::Delete,          "DELETE"        },
        { Key::Right,           "RIGHT"         },
        { Key::Left,            "LEFT"          },
        { Key::Down,            "DOWN"          },
        { Key::Up,              "UP"            },
        { Key::PageUp,          "PAGEUP"        },
        { Key::PageDown,        "PAGEDOWN"      },
        { Key::Home,            "HOME"          },
        { Key::End,             "END"           },
        { Key::CapsLock,        "CAPSLOCK"      },
        { Key::ScrollLock,      "SCROLLOCK"     },
        { Key::NumLock,         "NUMLOCK"       },
        { Key::PrintScreen,     "PRINTSCREEN"   },
        { Key::Pause,           "PAUSE"         },
        { Key::F1,              "F1"            },
        { Key::F2,              "F2"            },
        { Key::F3,              "F3"            },
        { Key::F4,              "F4"            },
        { Key::F5,              "F5"            },
        { Key::F6,              "F6"            },
        { Key::F7,              "F7"            },
        { Key::F8,              "F8"            },
        { Key::F9,              "F9"            },
        { Key::F10,             "F10"           },
        { Key::F11,             "F11"           },
        { Key::F12,             "F12"           },
        { Key::F13,             "F13"           },
        { Key::F14,             "F14"           },
        { Key::F15,             "F15"           },
        { Key::F16,             "F16"           },
        { Key::F17,             "F17"           },
        { Key::F18,             "F18"           },
        { Key::F19,             "F19"           },
        { Key::F20,             "F20"           },
        { Key::F21,             "F21"           },
        { Key::F22,             "F22"           },
        { Key::F23,             "F23"           },
        { Key::F24,             "F24"           },
        { Key::F25,             "F25"           },
        { Key::KeyPad0,         "KEYPAD0"       },
        { Key::KeyPad1,         "KEYPAD0"       },
        { Key::KeyPad2,         "KEYPAD0"       },
        { Key::KeyPad3,         "KEYPAD0"       },
        { Key::KeyPad4,         "KEYPAD0"       },
        { Key::KeyPad5,         "KEYPAD0"       },
        { Key::KeyPad6,         "KEYPAD0"       },
        { Key::KeyPad7,         "KEYPAD0"       },
        { Key::KeyPad8,         "KEYPAD0"       },
        { Key::KeyPad9,         "KEYPAD0"       },
        { Key::KeyPadDecimal,   "KEYPAD."       },
        { Key::KeyPadDivide,    "KEYPAD/"       },
        { Key::KeyPadMultiply,  "KEYPAD*"       },
        { Key::KeyPadSubtract,  "KEYPAD-"       },
        { Key::KeyPadAdd,       "KEYPAD+"       },
        { Key::KeyPadEnter,     "KEYPAD_ENTER"  },
        { Key::KeyPadEqual,     "KEYPAD="       },
        
        // Control keys
        { Key::LeftShift,       "LEFT_SHIFT"    },
        { Key::LeftControl,     "LEFT_CONTROL"  },
        { Key::LeftAlt,         "LEFT_ALT"      },
        { Key::LeftSuper,       "LEFT_SUPER"    },
        { Key::RightShift,      "RIGHT_SHIFT"   },
        { Key::RightControl,    "RIGHT_CONTROL" },
        { Key::RightAlt,        "RIGHT_ALT"     },
        { Key::RightSuper,      "RIGHT_SUPER"   },
        { Key::Menu,            "MENU"          }
    };

    auto key_and_name_it = s_name_by_key.find(m_key);
    if (key_and_name_it == s_name_by_key.end())
    {
        assert(0);
        return "";
    }
    return key_and_name_it->second;
};

State::State(std::initializer_list<Key> pressed_keys)
{
    for (Key pressed_key : pressed_keys)
    {
        SetKey(pressed_key, KeyState::Pressed);
    }
}

State::State(const State& other)
    : m_key_states(other.m_key_states)
    , m_modifiers_mask(other.m_modifiers_mask)
{
}

void State::SetKey(Key key, KeyState state)
{
    switch (key)
    {
    case Key::LeftShift:
    case Key::RightShift:
        UpdateModifiersMask(Modifier::Shift,     state == KeyState::Pressed);
        break;
    case Key::LeftControl:
    case Key::RightControl:
        UpdateModifiersMask(Modifier::Control,   state == KeyState::Pressed);
        break;
    case Key::LeftAlt:
    case Key::RightAlt:
        UpdateModifiersMask(Modifier::Alt,       state == KeyState::Pressed);
        break;
    case Key::LeftSuper:
    case Key::RightSuper:
        UpdateModifiersMask(Modifier::Super,     state == KeyState::Pressed);
        break;
    case Key::CapsLock:
        UpdateModifiersMask(Modifier::CapsLock,  state == KeyState::Pressed);
        break;
    case Key::NumLock:
        UpdateModifiersMask(Modifier::NumLock,   state == KeyState::Pressed);
        break;
    default:
        {
            size_t key_index = static_cast<size_t>(key);
            if (key_index < m_key_states.size())
            {
                m_key_states[key_index] = state;
            }
        }
    }
}

void State::UpdateModifiersMask(Modifier::Value modifier, bool add_modifier)
{
    if (add_modifier)
        m_modifiers_mask |= modifier;
    else
        m_modifiers_mask &= ~modifier;
}

Keys State::GetPressedKeys() const
{
    Keys pressed_keys;
    for (size_t key_index = 0; key_index < m_key_states.size(); ++key_index)
    {
        if (m_key_states[key_index] != KeyState::Pressed)
            continue;

        const Key key = static_cast<Key>(key_index);
        pressed_keys.insert(key);
    }
    return pressed_keys;
}

std::string Modifier::ToString(Value modifier)
{
    switch(modifier)
    {
        case None:      return "None";
        case Shift:     return "Shift";
        case Control:   return "Control";
        case Alt:       return "Alt";
        case Super:     return "Super";
        case CapsLock:  return "CapsLock";
        case NumLock:   return "NumLock";
        case All:       return "All";
    }
}

std::string Modifier::ToString(Modifier::Mask modifiers_mask)
{
    std::stringstream ss;
    bool first_modifier = true;
    for(Value modifier : values)
    {
        if (modifiers_mask & modifier)
        {
            if (!first_modifier)
            {
                ss << s_keys_separator;
            }
            ss << ToString(modifier);
            first_modifier = false;
        }
    }
    return ss.str();
}

std::string State::ToString() const
{
    std::stringstream ss;
    const std::string modifiers_str = Modifier::ToString(m_modifiers_mask);
    if (!modifiers_str.empty())
    {
        ss << modifiers_str;
    }
    
    bool is_first_key = true;
    for (size_t key_index = 0; key_index < m_key_states.size(); ++key_index)
    {
        if (m_key_states[key_index] != KeyState::Pressed)
            continue;
        
        if (!is_first_key || !modifiers_str.empty())
        {
            ss << s_keys_separator;
        }
        
        const Key key = static_cast<Key>(key_index);
        ss << KeyConverter(key).ToString();
        is_first_key = false;
    }
    
    return ss.str();
}
