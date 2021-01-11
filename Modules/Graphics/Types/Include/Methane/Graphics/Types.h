/******************************************************************************

Copyright 2019-2020 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Types.h
Methane primitive graphics types.

******************************************************************************/

#pragma once

#include <Methane/Data/Types.h>

#include <cml/vector.h>
#include <cml/matrix.h>
#include <cml/mathlib/constants.h>

#include <hlsl++.h>

#include <string>
#include <cstdint>

#define SHADER_STRUCT_ALIGNMENT 256
#define SHADER_STRUCT_ALIGN alignas(SHADER_STRUCT_ALIGNMENT)
#define SHADER_FIELD_ALIGN  alignas(16)
#define SHADER_FIELD_PACK   alignas(4)

namespace Methane::Graphics
{

using Depth = float;
using Stencil = uint8_t;
using DepthStencil = std::pair<Depth, Stencil>;

using Timestamp = Data::Timestamp;
using TimeDelta = Data::TimeDelta;
using Frequency = Data::Frequency;

enum class PixelFormat : uint32_t
{
    Unknown = 0U,
    RGBA8,
    RGBA8Unorm,
    RGBA8Unorm_sRGB,
    BGRA8Unorm,
    BGRA8Unorm_sRGB,
    R32Float,
    R32Uint,
    R32Sint,
    R16Float,
    R16Uint,
    R16Sint,
    R16Unorm,
    R16Snorm,
    R8Uint,
    R8Sint,
    R8Unorm,
    R8Snorm,
    A8Unorm,
    Depth32Float
};

using PixelFormats = std::vector<PixelFormat>;

[[nodiscard]] Data::Size GetPixelSize(PixelFormat pixel_format);
[[nodiscard]] bool IsSrgbColorSpace(PixelFormat pixel_format) noexcept;

enum class Compare : uint32_t
{
    Never = 0,
    Always,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
    Equal,
    NotEqual,
};

} // namespace Methane::Graphics
