/******************************************************************************

Copyright 2019-2021 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Vulkan/Context.hpp
Vulkan template implementation of the base context interface.

******************************************************************************/

#pragma once

#include "IContext.h"
#include "Device.h"
#include "CommandQueue.h"
#include "DescriptorManager.h"

#include <Methane/Graphics/RHI/IRenderContext.h>
#include <Methane/Graphics/RHI/ICommandKit.h>
#include <Methane/Instrumentation.h>

#include <string>
#include <map>

namespace Methane::Graphics::Rhi
{

struct ICommandQueue;

} // namespace Methane::Graphics::Rhi

namespace Methane::Graphics::Vulkan
{

template<class ContextBaseT, typename = std::enable_if_t<std::is_base_of_v<Base::Context, ContextBaseT>>>
class Context
    : public ContextBaseT
    , public IContext
{
public:
    Context(Base::Device& device, tf::Executor& parallel_executor, const typename ContextBaseT::Settings& settings)
        : ContextBaseT(device, std::make_unique<DescriptorManager>(*this), parallel_executor, settings)
    { }

    void Release() override
    {
        META_FUNCTION_TASK();

        // Vulkan descriptor pools have to be released before destroying device
        // to release all descriptor sets using live device instance
        ContextBaseT::GetDescriptorManager().Release();

        ContextBaseT::Release();
    }

    // IContext interface

    const Device& GetVulkanDevice() const noexcept final
    {
        META_FUNCTION_TASK();
        return static_cast<const Device&>(ContextBaseT::GetBaseDevice());
    }

    CommandQueue& GetVulkanDefaultCommandQueue(Rhi::CommandListType type) final
    {
        META_FUNCTION_TASK();
        return dynamic_cast<CommandQueue&>(ContextBaseT::GetDefaultCommandKit(type).GetQueue());
    }

    DescriptorManager& GetVulkanDescriptorManager() const final
    {
        return static_cast<DescriptorManager&>(ContextBaseT::GetDescriptorManager());
    }
};

} // namespace Methane::Graphics::Vulkan
