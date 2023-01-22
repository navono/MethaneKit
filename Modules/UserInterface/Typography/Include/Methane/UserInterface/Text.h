/******************************************************************************

Copyright 2020 Evgeny Gorodetskiy

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

FILE: Methane/UserInterface/Text.h
Methane text rendering primitive.

******************************************************************************/

#pragma once

#include "Font.h"

#include <Methane/UserInterface/Item.h>
#include <Methane/Graphics/RHI/CommandListDebugGroup.h>
#include <Methane/Graphics/RHI/RenderState.h>
#include <Methane/Graphics/RHI/RenderPass.h>
#include <Methane/Graphics/RHI/ViewState.h>
#include <Methane/Graphics/RHI/ProgramBindings.h>
#include <Methane/Graphics/RHI/Buffer.h>
#include <Methane/Graphics/RHI/BufferSet.h>
#include <Methane/Graphics/RHI/Texture.h>
#include <Methane/Graphics/RHI/Sampler.h>
#include <Methane/Graphics/Color.hpp>
#include <Methane/Data/Receiver.hpp>
#include <Methane/Data/EnumMask.hpp>

#include <string_view>

namespace Methane::Graphics::Rhi
{
class RenderContext;
class RenderCommandList;
}

namespace Methane::UserInterface
{

enum class TextWrap : uint32_t
{
    None = 0U,
    Anywhere,
    Word
};

enum class TextHorizontalAlignment : uint32_t
{
    Left = 0U,
    Right,
    Center,
    Justify
};

enum class TextVerticalAlignment : uint32_t
{
    Top = 0U,
    Bottom,
    Center
};

struct TextLayout
{
    TextWrap                wrap                 = TextWrap::Anywhere;
    TextHorizontalAlignment horizontal_alignment = TextHorizontalAlignment::Left;
    TextVerticalAlignment   vertical_alignment   = TextVerticalAlignment::Top;

    [[nodiscard]]
    bool operator==(const TextLayout& other) const noexcept
    {
        return std::tie(wrap, horizontal_alignment, vertical_alignment) ==
               std::tie(other.wrap, other.horizontal_alignment, other.vertical_alignment);
    }
};

template<typename StringType>
struct TextSettings // NOSONAR
{
    std::string name;
    StringType  text;
    UnitRect    rect;
    TextLayout  layout;
    Color4F     color { 1.F, 1.F, 1.F, 1.F };
    bool        incremental_update = true;
    bool        adjust_vertical_content_offset = true;

    // Minimize number of vertex/index buffer re-allocations on dynamic text updates by reserving additional size with multiplication of required size
    Data::Size  mesh_buffers_reservation_multiplier = 2U;

    // Text render state object name for using as a key in graphics object cache
    // NOTE: State name should be different in case of render state incompatibility between Text objects
    std::string state_name = "Screen Text Render State";

    TextSettings& SetName(std::string_view new_name) noexcept                                         { name = new_name; return *this; }
    TextSettings& SetText(const StringType& new_text) noexcept                                        { text = new_text; return *this; }
    TextSettings& SetRect(const UnitRect& new_rect) noexcept                                          { rect = new_rect; return *this; }
    TextSettings& SetLayout(const TextLayout& new_layout) noexcept                                    { layout = new_layout; return *this; }
    TextSettings& SetColor(const Color4F& new_color) noexcept                                         { color = new_color; return *this; }
    TextSettings& SetIncrementalUpdate(bool new_incremental_update) noexcept                          { incremental_update = new_incremental_update; return *this; }
    TextSettings& SetAdjustVerticalContentOffset(bool new_adjust_offset) noexcept                     { adjust_vertical_content_offset = new_adjust_offset; return *this; }
    TextSettings& SetMeshBuffersReservationMultiplier(Data::Size new_reservation_multiplier) noexcept { mesh_buffers_reservation_multiplier = new_reservation_multiplier; return *this; }
    TextSettings& SetStateName(std::string_view new_state_name) noexcept                              { state_name = new_state_name; return *this; }
};

class TextMesh;

class Text // NOSONAR - class destructor is required
    : protected Data::Receiver<IFontCallback> //NOSONAR
{
public:
    using Wrap                = TextWrap;
    using HorizontalAlignment = TextHorizontalAlignment;
    using VerticalAlignment   = TextVerticalAlignment;
    using Layout              = TextLayout;

    template<typename StringType>
    using Settings      = TextSettings<StringType>;
    using SettingsUtf8  = Settings<std::string>;
    using SettingsUtf32 = Settings<std::u32string>;

    Text(Context& ui_context, const rhi::RenderPattern& render_pattern, const Font& font, const SettingsUtf8& settings);
    Text(Context& ui_context, const Font& font, const SettingsUtf8& settings);
    Text(Context& ui_context, const rhi::RenderPattern& render_pattern, const Font& font, SettingsUtf32 settings);
    Text(Context& ui_context, const Font& font, SettingsUtf32 settings);
    ~Text() override;

    [[nodiscard]] const UnitRect&       GetFrameRect() const noexcept { return m_frame_rect; }
    [[nodiscard]] const SettingsUtf32&  GetSettings() const noexcept  { return m_settings; }
    [[nodiscard]] const std::u32string& GetTextUtf32() const noexcept { return m_settings.text; }
    [[nodiscard]] std::string           GetTextUtf8() const;

    void SetText(std::string_view text);
    void SetText(std::u32string_view text);
    void SetTextInScreenRect(std::string_view text, const UnitRect& ui_rect);
    void SetTextInScreenRect(std::u32string_view text, const UnitRect& ui_rect);
    void SetColor(const gfx::Color4F& color);
    void SetLayout(const Layout& layout);
    void SetWrap(Wrap wrap);
    void SetHorizontalAlignment(HorizontalAlignment alignment);
    void SetVerticalAlignment(VerticalAlignment alignment);
    void SetIncrementalUpdate(bool incremental_update) noexcept { m_settings.incremental_update = incremental_update; }

    bool SetFrameRect(const UnitRect& ui_rect);

    void Update(const gfx::FrameSize& render_attachment_size);
    void Draw(const rhi::RenderCommandList& cmd_list, const rhi::CommandListDebugGroup* debug_group_ptr = nullptr);

protected:
    // IFontCallback interface
    void OnFontAtlasTextureReset(Font& font, const rhi::Texture* old_atlas_texture_ptr, const rhi::Texture* new_atlas_texture_ptr) override;
    void OnFontAtlasUpdated(Font&) override { /* not handled in this class */ }

    virtual void OnFrameRectUpdated(const UnitRect&) { /* implemented in derived class */ }

private:
    struct CommonResourceRefs
    {
        const rhi::RenderContext& render_context;
        const rhi::RenderState&   render_state;
        const rhi::Buffer&        const_buffer;
        const rhi::Texture&       atlas_texture;
        const rhi::Sampler&       atlas_sampler;
        const TextMesh&           text_mesh;
    };

    class FrameResources
    {
    public:
        enum class DirtyResource : uint32_t
        {
            Mesh,
            Uniforms,
            Atlas,
        };

        using DirtyResourceMask = Data::EnumMask<DirtyResource>;

        FrameResources(uint32_t frame_index, const CommonResourceRefs& common_resources);

        void SetDirty(DirtyResourceMask dirty_mask) noexcept;

        [[nodiscard]] bool IsDirty(DirtyResource resource) const noexcept;
        [[nodiscard]] bool IsDirty() const noexcept;
        [[nodiscard]] bool IsInitialized() const noexcept;
        [[nodiscard]] bool IsAtlasInitialized() const noexcept;

        [[nodiscard]] const rhi::BufferSet&       GetVertexBufferSet() const noexcept;
        [[nodiscard]] const rhi::Buffer&          GetIndexBuffer() const noexcept;
        [[nodiscard]] const rhi::ProgramBindings& GetProgramBindings() const noexcept;

        bool UpdateAtlasTexture(const rhi::Texture& new_atlas_texture); // returns true if program bindings were updated, false if bindings have to be initialized
        void UpdateMeshBuffers(const rhi::RenderContext& render_context, const TextMesh& text_mesh, std::string_view text_name, Data::Size reservation_multiplier);
        void UpdateUniformsBuffer(const rhi::RenderContext& render_context, const TextMesh& text_mesh, std::string_view text_name);
        void InitializeProgramBindings(const rhi::RenderState& state, const rhi::Buffer& const_buffer_ptr,
                                       const rhi::Sampler& atlas_sampler_ptr, std::string_view text_name);

    private:
        uint32_t             m_frame_index;
        DirtyResourceMask    m_dirty_mask { ~0U };
        rhi::BufferSet       m_vertex_buffer_set;
        rhi::Buffer          m_index_buffer;
        rhi::Buffer          m_uniforms_buffer;
        rhi::Texture         m_atlas_texture;
        rhi::ProgramBindings m_program_bindings;
    };

    void InitializeFrameResources();
    void MakeFrameResourcesDirty(FrameResources::DirtyResourceMask resource);
    FrameResources& GetCurrentFrameResources();

    void UpdateTextMesh();
    void UpdateConstantsBuffer();

    struct UpdateRectResult
    {
        bool rect_changed = false;
        bool size_changed = false;
    };

    UpdateRectResult UpdateRect(const UnitRect& ui_rect, bool reset_content_rect);
    FrameRect GetAlignedViewportRect() const;
    void UpdateViewport(const gfx::FrameSize& render_attachment_size);

    Context&                    m_ui_context;
    SettingsUtf32               m_settings;
    UnitRect                    m_frame_rect;
    FrameSize                   m_render_attachment_size = FrameSize::Max();
    Font                        m_font;
    UniquePtr<TextMesh>         m_text_mesh_ptr;
    rhi::RenderState            m_render_state;
    rhi::ViewState              m_view_state;
    rhi::Buffer                 m_const_buffer;
    rhi::Sampler                m_atlas_sampler;
    std::vector<FrameResources> m_frame_resources;
    bool                        m_is_viewport_dirty = true;
    bool                        m_is_const_buffer_dirty = true;
};

} // namespace Methane::Graphics
