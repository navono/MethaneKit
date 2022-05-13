/******************************************************************************

Copyright 2022 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/DirectX12/ResourceDX.cpp
DirectX 12 specialization of the resource interface.

******************************************************************************/

#include "ResourceDX.h"
#include "BufferDX.h"
#include "TextureDX.h"
#include "SamplerDX.h"
#include "ContextDX.h"
#include "DescriptorManagerDX.h"

#include <Methane/Graphics/Context.h>
#include <Methane/Graphics/Texture.h>
#include <Methane/Graphics/ResourceBase.h>
#include <Methane/Instrumentation.h>
#include <Methane/Checks.hpp>

namespace Methane::Graphics
{

DescriptorHeapDX::Type IResourceDX::GetDescriptorHeapTypeByUsage(Resource& resource, Resource::Usage resource_usage)
{
    META_FUNCTION_TASK();
    const Resource::Type resource_type = resource.GetResourceType();
    switch (resource_usage)
    {
    case Resource::Usage::ShaderRead:
        return (resource_type == Resource::Type::Sampler)
               ? DescriptorHeapDX::Type::Samplers
               : DescriptorHeapDX::Type::ShaderResources;

    case Resource::Usage::ShaderWrite:
    case Resource::Usage::RenderTarget:
        return (resource_type == Resource::Type::Texture &&
                dynamic_cast<const Texture&>(resource).GetSettings().type == Texture::Type::DepthStencilBuffer)
               ? DescriptorHeapDX::Type::DepthStencil
               : DescriptorHeapDX::Type::RenderTargets;

    default:
        META_UNEXPECTED_ARG_DESCR_RETURN(resource_usage, DescriptorHeapDX::Type::Undefined,
                                         "resource usage does not map to descriptor heap");
    }
}

ResourceLocationDX::Id::Id(Resource::Usage usage, const ResourceLocation::Settings& settings)
    : usage(usage)
    , settings_ref(settings)
{
    META_FUNCTION_TASK();
}

bool ResourceLocationDX::Id::operator<(const Id& other) const noexcept
{
    META_FUNCTION_TASK();
    return std::tie(usage, settings_ref.get()) <
           std::tie(other.usage, other.settings_ref.get());
}

bool ResourceLocationDX::Id::operator==(const Id& other) const noexcept
{
    META_FUNCTION_TASK();
    return std::tie(usage, settings_ref.get()) ==
           std::tie(other.usage, other.settings_ref.get());
}

bool ResourceLocationDX::Id::operator!=(const Id& other) const noexcept
{
    META_FUNCTION_TASK();
    return std::tie(usage, settings_ref.get()) !=
           std::tie(other.usage, other.settings_ref.get());
}

ResourceLocationDX::ResourceLocationDX(const ResourceLocation& location, Resource::Usage usage)
    : ResourceLocation(location)
    , m_id(usage, GetSettings())
    , m_resource_dx(dynamic_cast<IResourceDX&>(GetResource()))
    , m_context_dx(dynamic_cast<const IContextDX&>(GetResource().GetContext()))
    , m_descriptor_opt(m_resource_dx.GetNativeViewDescriptor(*this))
{
    META_FUNCTION_TASK();
}

ResourceLocationDX::~ResourceLocationDX()
{
    META_FUNCTION_TASK();
}

[[nodiscard]] D3D12_GPU_VIRTUAL_ADDRESS ResourceLocationDX::GetNativeGpuAddress() const noexcept
{
    META_FUNCTION_TASK();
    return m_resource_dx.GetNativeGpuAddress() + GetOffset();
}

D3D12_CPU_DESCRIPTOR_HANDLE ResourceLocationDX::GetNativeCpuDescriptorHandle() const noexcept
{
    META_FUNCTION_TASK();
    return m_descriptor_opt ? m_descriptor_opt->heap.GetNativeCpuDescriptorHandle(m_descriptor_opt->index)
                            : D3D12_CPU_DESCRIPTOR_HANDLE{ 0U };
}

D3D12_GPU_DESCRIPTOR_HANDLE ResourceLocationDX::GetNativeGpuDescriptorHandle() const noexcept
{
    META_FUNCTION_TASK();
    return m_descriptor_opt ? m_descriptor_opt->heap.GetNativeGpuDescriptorHandle(m_descriptor_opt->index)
                            : D3D12_GPU_DESCRIPTOR_HANDLE{ 0U };
}

} // namespace Methane::Graphics