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

FILE: Methane/Graphics/Metal/RenderPattern.mm
Metal implementation of the render pattern interface.

******************************************************************************/

#include <Methane/Graphics/Metal/RenderPattern.hh>
#include <Methane/Graphics/Metal/RenderPass.hh>
#include <Methane/Graphics/Base/RenderContext.h>

#include <Methane/Instrumentation.h>

namespace Methane::Graphics::Metal
{

Ptr<Rhi::IRenderPass> RenderPattern::CreateRenderPass(const Rhi::RenderPassSettings& settings)
{
    return std::make_shared<RenderPass>(*this, settings);
}

} // namespace Methane::Graphics::Metal