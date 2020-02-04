/******************************************************************************

Copyright 2019-2020 Evgeny Gorodetskiy

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

FILE: Methane/Graphics/DirectX12/CommandListDX.h
DirectX 12 base template implementation of the command list interface.

******************************************************************************/

#pragma once

#include <Methane/Graphics/CommandListBase.h>
#include <Methane/Graphics/Windows/Helpers.h>

#include <wrl.h>
#include <d3d12.h>
#include <pix.h>
#include <nowide/convert.hpp>

#include <cassert>

namespace Methane::Graphics
{

namespace wrl = Microsoft::WRL;

class CommandQueueDX;
class RenderPassDX;

template<class CommandListBaseT, typename = std::enable_if_t<std::is_base_of_v<CommandListBase, CommandListBaseT>>>
class CommandListDX : public CommandListBaseT
{
public:
    template<typename... ConstructArgs>
    CommandListDX(ConstructArgs&... construct_args)
        : CommandListBaseT(construct_args...)
    {
        ITT_FUNCTION_TASK();

        const wrl::ComPtr<ID3D12Device>& cp_device = GetCommandQueueDX().GetContextDX().GetDeviceDX().GetNativeDevice();
        assert(!!cp_device);

        ThrowIfFailed(cp_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cp_command_allocator)));
        ThrowIfFailed(cp_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cp_command_allocator.Get(), nullptr, IID_PPV_ARGS(&m_cp_command_list)));
        m_cp_command_list.As(&m_cp_command_list_4);
    }

    // CommandList interface

    void PushDebugGroup(const std::string& name) override
    {
        ITT_FUNCTION_TASK();
        PIXBeginEvent(m_cp_command_list.Get(), 0, nowide::widen(name).c_str());
    }

    void PopDebugGroup() override
    {
        ITT_FUNCTION_TASK();
        PIXEndEvent(m_cp_command_list.Get());
    }

    void Commit(bool present_drawable) override
    {
        ITT_FUNCTION_TASK();

        CommandListBaseT::Commit(present_drawable);

        m_cp_command_list->Close();
        m_is_committed = true;
    }

    // CommandListBase interface

    void SetResourceBarriers(const ResourceBase::Barriers& resource_barriers) override
    {
        ITT_FUNCTION_TASK();

        if (resource_barriers.empty())
            return;

        std::vector<D3D12_RESOURCE_BARRIER> dx_resource_barriers;
        for (const ResourceBase::Barrier& resource_barrier : resource_barriers)
        {
            dx_resource_barriers.push_back(ResourceDX::GetNativeResourceBarrier(resource_barrier));
        }

        assert(m_cp_command_list);
        m_cp_command_list->ResourceBarrier(static_cast<UINT>(dx_resource_barriers.size()), dx_resource_barriers.data());
    }

    void Execute(uint32_t frame_index) override
    {
        ITT_FUNCTION_TASK();
        CommandListBaseT::Execute(frame_index);

        // NOTE: In DirectX there's no need for tracking command list completion, so it's completed right away
        CommandListBaseT::Complete(frame_index);
    }

    // CommandList interface

    void Reset(const std::string& debug_group) override
    {
        ITT_FUNCTION_TASK();
        if (!m_is_committed)
            return;

        m_is_committed = false;

        ThrowIfFailed(m_cp_command_allocator->Reset());
        ThrowIfFailed(m_cp_command_list->Reset(m_cp_command_allocator.Get(), nullptr));

        CommandListBase::Reset(debug_group);
    }

    // Object interface
    void SetName(const std::string& name) override
    {
        ITT_FUNCTION_TASK();

        assert(m_cp_command_list);
        m_cp_command_list->SetName(nowide::widen(name).c_str());

        assert(m_cp_command_allocator);
        m_cp_command_allocator->SetName(nowide::widen(name + " allocator").c_str());

        CommandListBaseT::SetName(name);
    }

    const wrl::ComPtr<ID3D12GraphicsCommandList>& GetNativeCommandList() const   { return m_cp_command_list; }
    wrl::ComPtr<ID3D12GraphicsCommandList>&       GetNativeCommandList()         { return m_cp_command_list; }

    const wrl::ComPtr<ID3D12GraphicsCommandList4>& GetNativeCommandList4() const { return m_cp_command_list_4; }
    wrl::ComPtr<ID3D12GraphicsCommandList4>&       GetNativeCommandList4()       { return m_cp_command_list_4; }

protected:
    CommandQueueDX& GetCommandQueueDX() { return static_cast<CommandQueueDX&>(GetCommandQueueBase()); }

    wrl::ComPtr<ID3D12CommandAllocator>       m_cp_command_allocator;
    wrl::ComPtr<ID3D12GraphicsCommandList>    m_cp_command_list;
    wrl::ComPtr<ID3D12GraphicsCommandList4>   m_cp_command_list_4;    // extended interface for the same command list (may be unavailable on older Windows)
    bool                                      m_is_committed = false;
};

} // namespace Methane::Graphics
