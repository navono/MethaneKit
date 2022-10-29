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

FILE: Methane/Graphics/Core.h
Methane graphics core interfaces: all headers under one umbrella.

******************************************************************************/

#pragma once

#include "IDevice.h"
#include "IRenderContext.h"
#include "IShader.h"
#include "IProgram.h"
#include "IProgramBindings.h"
#include "RenderPass.h"
#include "IRenderState.h"
#include "IResource.h"
#include "IBuffer.h"
#include "ITexture.h"
#include "Sampler.h"
#include "CommandKit.h"
#include "CommandQueue.h"
#include "TransferCommandList.h"
#include "RenderCommandList.h"
#include "ParallelRenderCommandList.h"
