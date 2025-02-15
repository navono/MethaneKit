/******************************************************************************

Copyright 2023 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Null/Device.h
Null implementation of the device interface.

******************************************************************************/

#pragma once

#include <Methane/Graphics/Base/Device.h>

namespace Methane::Graphics::Null
{

class Device final
    : public Base::Device
{
public:
    using Base::Device::Device;

    // IDevice interface
    [[nodiscard]] Ptr<Rhi::IRenderContext> CreateRenderContext(const Platform::AppEnvironment& env, tf::Executor& parallel_executor, const Rhi::RenderContextSettings& settings) override;
};

} // namespace Methane::Graphics::Null
