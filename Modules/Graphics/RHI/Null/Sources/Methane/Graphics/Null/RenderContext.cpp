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

FILE: Methane/Graphics/Null/RenderContext.cpp
Null implementation of the render context interface.

******************************************************************************/

#include <Methane/Graphics/Null/RenderContext.h>
#include <Methane/Graphics/Null/CommandQueue.h>
#include <Methane/Graphics/Null/Shader.h>
#include <Methane/Graphics/Null/Program.h>
#include <Methane/Graphics/Null/RenderPass.h>
#include <Methane/Graphics/Null/RenderState.h>
#include <Methane/Graphics/Null/RenderPattern.h>
#include <Methane/Graphics/Null/Buffer.h>
#include <Methane/Graphics/Null/Texture.h>
#include <Methane/Graphics/Null/Sampler.h>

namespace Methane::Graphics::Null
{

RenderContext::RenderContext(const Methane::Platform::AppEnvironment&, Device& device, tf::Executor& parallel_executor, const Rhi::RenderContextSettings& settings)
    : Context(device, parallel_executor, settings)
{
}

RenderContext::~RenderContext()
{
    try
    {
        RenderContext::Release();
    }
    catch(const std::exception& e)
    {
        META_UNUSED(e);
        META_LOG("WARNING: Unexpected error during Query destruction: {}", e.what());
        assert(false);
    }
}

Ptr<Rhi::ICommandQueue> RenderContext::CreateCommandQueue(Rhi::CommandListType type) const
{
    META_FUNCTION_TASK();
    return std::make_shared<CommandQueue>(*this, type);
}

Ptr<Rhi::IShader> RenderContext::CreateShader(Rhi::ShaderType type, const Rhi::ShaderSettings& settings) const
{
    META_FUNCTION_TASK();
    return std::make_shared<Shader>(type, *this, settings);
}

Ptr<Rhi::IProgram> RenderContext::CreateProgram(const Rhi::ProgramSettings& settings) const
{
    META_FUNCTION_TASK();
    return std::make_shared<Program>(*this, settings);
}

Ptr<Rhi::IBuffer> RenderContext::CreateBuffer(const Rhi::BufferSettings& settings) const
{
    META_FUNCTION_TASK();
    return std::make_shared<Buffer>(*this, settings);
}

Ptr<Rhi::ITexture> RenderContext::CreateTexture(const Rhi::TextureSettings& settings) const
{
    META_FUNCTION_TASK();
    return std::make_shared<Texture>(*this, settings);
}

Ptr<Rhi::ISampler> RenderContext::CreateSampler(const Rhi::SamplerSettings& settings) const
{
    META_FUNCTION_TASK();
    return std::make_shared<Sampler>(*this, settings);
}

Ptr<Rhi::IRenderState> RenderContext::CreateRenderState(const Rhi::RenderStateSettings& settings) const
{
    META_FUNCTION_TASK();
    return std::make_shared<RenderState>(*this, settings);
}

Ptr<Rhi::IRenderPattern> RenderContext::CreateRenderPattern(const Rhi::RenderPatternSettings& settings)
{
    META_FUNCTION_TASK();
    return std::make_shared<RenderPattern>(*this, settings);
}

void RenderContext::Present()
{
    Context<Base::RenderContext>::Present();
    Context<Base::RenderContext>::OnCpuPresentComplete();
    UpdateFrameBufferIndex();
}

} // namespace Methane::Graphics::Null
