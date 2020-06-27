/******************************************************************************

Copyright 2020 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/Metal/ReleasePoolMT.mm
Metal GPU release pool for deferred objects release.

******************************************************************************/

#include "ReleasePoolMT.hh"

#include <Methane/Graphics/RenderContextBase.h>
#include <Methane/Instrumentation.h>

#import <Metal/Metal.h>

namespace Methane::Graphics
{

struct ResourceMT::ReleasePoolMT::BufferContainerMT : ResourceMT::ReleasePoolMT::IResourceContainerMT
{
    id <MTLBuffer> buffer;

    BufferContainerMT(const BufferMT& buffer) : buffer(buffer.GetNativeBuffer())
    { }
};

struct ResourceMT::ReleasePoolMT::TextureContainerMT : ResourceMT::ReleasePoolMT::IResourceContainerMT
{
    id <MTLTexture> texture;

    TextureContainerMT(const TextureMT& texture) : texture(texture.GetNativeTexture())
    { }
};

UniquePtr <ResourceMT::ReleasePoolMT::IResourceContainerMT> ResourceMT::ReleasePoolMT::IResourceContainerMT::Create(ResourceMT& resource)
{
    switch (resource.GetResourceType())
    {
    case Resource::Type::Buffer:
        return std::make_unique<BufferContainerMT>(static_cast<BufferMT&>(resource));
    case Resource::Type::Texture:
        return std::make_unique<TextureContainerMT>(static_cast<TextureMT&>(resource));
    case Resource::Type::Sampler:
        return nullptr;
    default:
        assert(0);
    }
    return nullptr;
}

Ptr <ResourceBase::ReleasePool> ResourceBase::ReleasePool::Create()
{
    META_FUNCTION_TASK();
    return std::make_shared<ResourceMT::ReleasePoolMT>();
}

ResourceMT::ReleasePoolMT::ReleasePoolMT()
    : ResourceBase::ReleasePool()
{
    META_FUNCTION_TASK();
}

} // namespace Methane::Graphics

