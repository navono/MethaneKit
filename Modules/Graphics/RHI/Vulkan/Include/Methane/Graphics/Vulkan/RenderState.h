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

FILE: Methane/Graphics/Vulkan/RenderState.h
Vulkan implementation of the render state interface.

******************************************************************************/

#pragma once

#include <Methane/Graphics/Base/RenderState.h>

#include <vulkan/vulkan.hpp>

namespace Methane::Graphics::Vulkan
{

struct IContext;

class RenderState final
    : public Base::RenderState
{
public:
    RenderState(const Base::RenderContext& context, const Settings& settings);
    
    // IRenderState interface
    void Reset(const Settings& settings) override;

    // Base::RenderState interface
    void Apply(Base::RenderCommandList& render_command_list, Groups state_groups) override;

    // IObject interface
    bool SetName(std::string_view name) override;

    const vk::Pipeline& GetNativePipeline() const noexcept { return m_vk_unique_pipeline.get(); }

private:
    const IContext&    m_vk_context;
    vk::UniquePipeline m_vk_unique_pipeline;
};

} // namespace Methane::Graphics::Vulkan
