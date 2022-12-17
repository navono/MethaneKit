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

FILE: Methane/Graphics/RHI/ProgramBindings.h
Methane ProgramBindings PIMPL wrappers for direct calls to final implementation.

******************************************************************************/

#pragma once

#include "Pimpl.h"

#include <Methane/Graphics/RHI/IProgramBindings.h>

namespace Methane::Graphics::Rhi
{

class Program;

class ProgramBindings
{
public:
    using IArgumentBindingCallback = IProgramArgumentBindingCallback;
    using IArgumentBinding = IProgramArgumentBinding;
    using ApplyBehavior = ProgramBindingsApplyBehavior;
    using ApplyBehaviorMask = ProgramBindingsApplyBehaviorMask;
    using UnboundArgumentsException = ProgramBindingsUnboundArgumentsException;
    using ResourceViewsByArgument = std::unordered_map<IProgram::Argument, IResource::Views, IProgram::Argument::Hash>;

    META_PIMPL_DEFAULT_CONSTRUCT_METHODS_DECLARE(ProgramBindings);

    ProgramBindings(const Ptr<IProgramBindings>& interface_ptr);
    ProgramBindings(IProgramBindings& interface_ref);
    ProgramBindings(const Program& program, const ResourceViewsByArgument& resource_views_by_argument, Data::Index frame_index = 0U);
    ProgramBindings(const ProgramBindings& other_program_bindings, const ResourceViewsByArgument& replace_resource_views_by_argument = {},
                    const Opt<Data::Index>& frame_index = {});

    void Init(const Program& program, const ResourceViewsByArgument& resource_views_by_argument, Data::Index frame_index = 0U);
    void InitCopy(const ProgramBindings& other_program_bindings, const ResourceViewsByArgument& replace_resource_views_by_argument = {},
                  const Opt<Data::Index>& frame_index = {});
    void Release();

    bool IsInitialized() const META_PIMPL_NOEXCEPT;
    IProgramBindings& GetInterface() const META_PIMPL_NOEXCEPT;

    // IObject interface methods
    bool SetName(std::string_view name) const;
    std::string_view GetName() const META_PIMPL_NOEXCEPT;

    // IProgramBindings interface methods
    [[nodiscard]] Program                 GetProgram() const;
    [[nodiscard]] IArgumentBinding&       Get(const ProgramArgument& shader_argument) const;
    [[nodiscard]] const ProgramArguments& GetArguments() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] Data::Index             GetFrameIndex() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] Data::Index             GetBindingsIndex() const META_PIMPL_NOEXCEPT;
    [[nodiscard]] explicit operator       std::string() const;

private:
    class Impl;

    UniquePtr<Impl> m_impl_ptr;
};

} // namespace Methane::Graphics::Rhi
