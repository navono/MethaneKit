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

FILE: Methane/Graphics/Base/Buffer.h
Base implementation of the buffer interface.

******************************************************************************/

#pragma once

#include "Resource.h"
#include "Context.h"

#include <Methane/Graphics/RHI/IBuffer.h>

namespace Methane::Graphics::Base
{

class Buffer
    : public Rhi::IBuffer
    , public Resource
{
public:
    Buffer(const Context& context, const Settings& settings,
               State initial_state = State::Undefined, Opt<State> auto_transition_source_state_opt = {});

    // IResource interface
    Data::Size GetDataSize(Data::MemoryState size_type = Data::MemoryState::Reserved) const noexcept override;

    // Buffer interface
    const Settings& GetSettings() const noexcept final { return m_settings; }
    uint32_t GetFormattedItemsCount() const noexcept final;

private:
    Settings m_settings;
};

} // namespace Methane::Graphics::Base
