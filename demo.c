#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#define CIMGUI_DEFINE_ENUMS_AND_STRUCTS
#include "cimgui.h"
#include "sokol_imgui.h"
#define SOKOL_IMPL
#include "sokol_gamepad.h"

#define DEBUG_FONT_SIZE 14
#include "fa6.h"
#include "fa6_data.h"
#include "berkeley_mono.h"

static struct {
    sg_pass_action pass_action;
} state;

static void init(void) {
    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });

    sgamepad_init();
    
    simgui_setup(&(simgui_desc_t){ 
        .no_default_font = 1,
    });

	ImFontConfig font_config = {
        .FontDataOwnedByAtlas = true,
        .SizePixels = DEBUG_FONT_SIZE,
        .RasterizerMultiply = 1.5f,
        .RasterizerDensity = 1.0,
        .GlyphMinAdvanceX = 1.0f,
        .GlyphMaxAdvanceX = 100.f,
        .PixelSnapH = true,
        .OversampleH = 2,
        .OversampleV = 2,
    };

    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    
    ImGuiIO* io = igGetIO();
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io->ConfigWindowsMoveFromTitleBarOnly = true;

    ImFontAtlas_AddFontFromMemoryCompressedTTF(io->Fonts, (void*)BerkeleyMono_compressed_data, BerkeleyMono_compressed_size, DEBUG_FONT_SIZE, &font_config, ImFontAtlas_GetGlyphRangesDefault(io->Fonts));
    
    font_config.MergeMode = true;
    font_config.GlyphOffset.y = 2.0;
    
    ImFontAtlas_AddFontFromMemoryCompressedTTF(io->Fonts, (void*)fa_solid_900_compressed_data, fa_solid_900_compressed_size, DEBUG_FONT_SIZE, &font_config, icons_ranges);

    {
        ImVec4 *colors = igGetStyle()->Colors;
        colors[ImGuiCol_Text]                   = (ImVec4){1.00f, 1.00f, 0.97f, 1.00f};
        colors[ImGuiCol_TextDisabled]           = (ImVec4){0.50f, 0.50f, 0.44f, 1.00f};
        
        colors[ImGuiCol_WindowBg]               = (ImVec4){0.04f, 0.01f, 0.05f, 0.90f};
        colors[ImGuiCol_ChildBg]                = (ImVec4){0.00f, 0.00f, 0.00f, 0.00f};
        colors[ImGuiCol_PopupBg]                = (ImVec4){0.04f, 0.01f, 0.05f, 0.90f};
        
        colors[ImGuiCol_Border]                 = (ImVec4){1.00f ,0.81f, 0.00f, 0.24f};
        colors[ImGuiCol_BorderShadow]           = (ImVec4){0.04f, 0.01f, 0.05f, 0.33f};
        
        colors[ImGuiCol_FrameBg]                = (ImVec4){0.09f, 0.14f, 0.41f, 0.54f};
        colors[ImGuiCol_FrameBgHovered]         = (ImVec4){0.09f, 0.14f, 0.41f, 0.90f};
        colors[ImGuiCol_FrameBgActive]          = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        
        colors[ImGuiCol_TitleBg]                = (ImVec4){0.09f, 0.14f, 0.41f, 0.90f};
        colors[ImGuiCol_TitleBgActive]          = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        colors[ImGuiCol_TitleBgCollapsed]       = (ImVec4){0.09f, 0.14f, 0.41f, 0.90f};
        
        colors[ImGuiCol_MenuBarBg]              = (ImVec4){0.09f, 0.14f, 0.41f, 0.50f};
        colors[ImGuiCol_ScrollbarBg]            = (ImVec4){0.00f, 0.00f, 0.00f, 0.00f};
        colors[ImGuiCol_ScrollbarGrab]          = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        colors[ImGuiCol_ScrollbarGrabHovered]   = (ImVec4){0.17f, 0.91f, 0.96f, 1.00f};
        colors[ImGuiCol_ScrollbarGrabActive]    = (ImVec4){0.17f, 0.91f, 0.96f, 1.00f};
        
        colors[ImGuiCol_CheckMark]              = (ImVec4){1.00f ,0.81f, 0.00f, 1.00f};
        
        colors[ImGuiCol_SliderGrab]             = (ImVec4){1.00f ,0.81f, 0.00f, 1.00f};
        colors[ImGuiCol_SliderGrabActive]       = (ImVec4){0.17f, 0.91f, 0.96f, 1.00f};
        
        colors[ImGuiCol_Button]                 = (ImVec4){0.09f, 0.14f, 0.41f, 0.54f};
        colors[ImGuiCol_ButtonHovered]          = (ImVec4){0.04f, 0.01f, 0.05f, 0.25f};
        colors[ImGuiCol_ButtonActive]           = (ImVec4){0.04f, 0.01f, 0.05f, 1.00f};
        
        colors[ImGuiCol_Header]                 = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        colors[ImGuiCol_HeaderHovered]          = (ImVec4){0.04f, 0.01f, 0.05f, 0.80f};
        colors[ImGuiCol_HeaderActive]           = (ImVec4){0.04f, 0.01f, 0.05f, 0.80f};

        colors[ImGuiCol_Separator]              = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        colors[ImGuiCol_SeparatorHovered]       = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        colors[ImGuiCol_SeparatorActive]        = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        
        colors[ImGuiCol_ResizeGrip]             = (ImVec4){0.28f, 0.28f, 0.28f, 0.29f};
        colors[ImGuiCol_ResizeGripHovered]      = (ImVec4){0.44f, 0.44f, 0.44f, 0.29f};
        colors[ImGuiCol_ResizeGripActive]       = (ImVec4){0.40f, 0.44f, 0.47f, 1.00f};
        
        colors[ImGuiCol_Tab]                    = (ImVec4){0.09f, 0.14f, 0.41f, 1.00f};
        colors[ImGuiCol_TabHovered]             = (ImVec4){0.00f, 0.00f, 0.00f, 1.00f};
        colors[ImGuiCol_TabSelected]            = (ImVec4){0.00f, 0.00f, 0.00f, 1.00f};
        colors[ImGuiCol_TabSelectedOverline]    = (ImVec4){1.00f ,0.81f, 0.00f, 1.00f};
        colors[ImGuiCol_TabDimmed]              = (ImVec4){0.00f, 0.00f, 0.00f, 1.00f};
        colors[ImGuiCol_TabDimmedSelected]      = (ImVec4){0.00f, 0.00f, 0.00f, 1.00f};
        
        colors[ImGuiCol_DockingPreview]         = (ImVec4){1.00f ,0.81f, 0.00f, 1.00f};
        colors[ImGuiCol_DockingEmptyBg]         = (ImVec4){0.04f, 0.01f, 0.05f, 0.10f};
        
        colors[ImGuiCol_PlotLines]              = (ImVec4){1.00f, 0.00f, 0.35f, 1.00f};
        colors[ImGuiCol_PlotLinesHovered]       = (ImVec4){1.00f, 0.00f, 0.35f, 1.00f};
        colors[ImGuiCol_PlotHistogram]          = (ImVec4){1.00f, 0.00f, 0.35f, 1.00f};
        colors[ImGuiCol_PlotHistogramHovered]   = (ImVec4){1.00f, 0.00f, 0.35f, 1.00f};
        
        colors[ImGuiCol_TableHeaderBg]          = (ImVec4){0.00f, 0.00f, 0.00f, 0.52f};
        colors[ImGuiCol_TableBorderStrong]      = (ImVec4){0.00f, 0.00f, 0.00f, 0.52f};
        colors[ImGuiCol_TableBorderLight]       = (ImVec4){0.28f, 0.28f, 0.28f, 0.29f};
        colors[ImGuiCol_TableRowBg]             = (ImVec4){0.00f, 0.00f, 0.00f, 0.00f};
        colors[ImGuiCol_TableRowBgAlt]          = (ImVec4){1.00f, 1.00f, 1.00f, 0.06f};
        
        colors[ImGuiCol_TextSelectedBg]         = (ImVec4){0.20f, 0.22f, 0.23f, 1.00f};
        colors[ImGuiCol_DragDropTarget]         = (ImVec4){1.00f ,0.81f, 0.00f, 1.00f};
        colors[ImGuiCol_NavHighlight]           = (ImVec4){1.00f ,0.81f, 0.00f, 1.00f};
        colors[ImGuiCol_NavWindowingHighlight]  = (ImVec4){1.00f, 0.00f, 0.00f, 0.70f};
        colors[ImGuiCol_NavWindowingDimBg]      = (ImVec4){1.00f, 0.00f, 0.00f, 0.20f};
        colors[ImGuiCol_ModalWindowDimBg]       = (ImVec4){1.00f, 0.00f, 0.00f, 0.35f};

        ImGuiStyle *style = igGetStyle();
        style->WindowPadding                     = (ImVec2){8.00f, 8.00f};
        style->FramePadding                      = (ImVec2){5.00f, 2.00f};
        style->CellPadding                       = (ImVec2){6.00f, 6.00f};
        style->ItemSpacing                       = (ImVec2){6.00f, 6.00f};
        style->ItemInnerSpacing                  = (ImVec2){6.00f, 6.00f};
        style->TouchExtraPadding                 = (ImVec2){0.00f, 0.00f};
        style->IndentSpacing                     = 25;
        style->ScrollbarSize                     = 15;
        style->GrabMinSize                       = 10;
        style->WindowBorderSize                  = 2;
        style->ChildBorderSize                   = 0;
        style->PopupBorderSize                   = 0.5;
        style->FrameBorderSize                   = 0.5;
        style->WindowRounding                    = 0;
        style->ChildRounding                     = 0;
        style->FrameRounding                     = 0;
        style->PopupRounding                     = 0;
        style->ScrollbarRounding                 = 5;
        style->GrabRounding                      = 0;
        style->TabRounding                       = 0;
        style->LogSliderDeadzone                 = 0;
        style->TabBorderSize                     = 0;
        style->DockingSeparatorSize              = 0;

    }

    // initial clear color
    state.pass_action = (sg_pass_action) {
        .colors[0] = { .load_action = SG_LOADACTION_CLEAR, .clear_value = { 1.0f, 1.0f, 1.0f, 1.0 } }
    };
}

static void frame(void) {
    simgui_new_frame(&(simgui_frame_desc_t){
        .width = sapp_width(),
        .height = sapp_height(),
        .delta_time = sapp_frame_duration(),
        .dpi_scale = sapp_dpi_scale(),
    });
    sgamepad_record_state();

    sgamepad_gamepad_state pad = {0};
    sgamepad_get_gamepad_state(0, &pad);

    ImGuiIO* io = igGetIO();
    io->BackendFlags |= ImGuiBackendFlags_HasGamepad;
    
    #define CLAMP(X, A, B) ((X < B)? (B) : ((X > A)? A : X))
    #define IG_MAP_BUTTON(NAV_NO, BUTTON) {ImGuiIO_AddKeyEvent(io, NAV_NO, (pad.digital_inputs & BUTTON) > 0);}
    #define IG_MAP_ANALOG(NAV_NO, AXIS, V0, V1) { float v = AXIS; v = (v - V0) / (V1 - V0); if (v > 1.0f) v = 1.0f; ImGuiIO_AddKeyAnalogEvent(io, NAV_NO, v > 0.1f, CLAMP(v, 0.0, 1.0)); }
    IG_MAP_BUTTON(ImGuiKey_GamepadStart,           SGAMEPAD_GAMEPAD_START);
    IG_MAP_BUTTON(ImGuiKey_GamepadBack,            SGAMEPAD_GAMEPAD_BACK);
    IG_MAP_BUTTON(ImGuiKey_GamepadFaceLeft,        SGAMEPAD_GAMEPAD_X);
    IG_MAP_BUTTON(ImGuiKey_GamepadFaceRight,       SGAMEPAD_GAMEPAD_B);
    IG_MAP_BUTTON(ImGuiKey_GamepadFaceUp,          SGAMEPAD_GAMEPAD_Y);
    IG_MAP_BUTTON(ImGuiKey_GamepadFaceDown,        SGAMEPAD_GAMEPAD_A);
    IG_MAP_BUTTON(ImGuiKey_GamepadDpadLeft,        SGAMEPAD_GAMEPAD_DPAD_LEFT);
    IG_MAP_BUTTON(ImGuiKey_GamepadDpadRight,       SGAMEPAD_GAMEPAD_DPAD_RIGHT);
    IG_MAP_BUTTON(ImGuiKey_GamepadDpadUp,          SGAMEPAD_GAMEPAD_DPAD_UP);
    IG_MAP_BUTTON(ImGuiKey_GamepadDpadDown,        SGAMEPAD_GAMEPAD_DPAD_DOWN);
    IG_MAP_ANALOG(ImGuiKey_GamepadL1,              pad.left_shoulder, +0.3f, +0.9f);
    IG_MAP_ANALOG(ImGuiKey_GamepadR1,              pad.right_shoulder, +0.3f, +0.9f);
    IG_MAP_ANALOG(ImGuiKey_GamepadL2,              pad.left_trigger, +0.3f, +0.9f);
    IG_MAP_ANALOG(ImGuiKey_GamepadR2,              pad.right_trigger, +0.3f, +0.9f);
    IG_MAP_BUTTON(ImGuiKey_GamepadL3,              SGAMEPAD_GAMEPAD_LEFT_THUMB);
    IG_MAP_BUTTON(ImGuiKey_GamepadR3,              SGAMEPAD_GAMEPAD_RIGHT_THUMB);
    IG_MAP_ANALOG(ImGuiKey_GamepadLStickDown, pad.left_stick.normalized_y, +0.3f, +0.9f);
    IG_MAP_ANALOG(ImGuiKey_GamepadLStickUp, pad.left_stick.normalized_y, -0.3f, -0.9f);
    IG_MAP_ANALOG(ImGuiKey_GamepadLStickLeft, pad.left_stick.normalized_x, +0.3f, +0.9f);
    IG_MAP_ANALOG(ImGuiKey_GamepadLStickRight, pad.left_stick.normalized_x, -0.3f, -0.9f);
    #undef IG_MAP_BUTTON
    #undef IG_MAP_ANALOG
    

    /*=== UI CODE STARTS HERE ===*/
    igSetNextWindowPos((ImVec2){10,10}, ImGuiCond_Once, (ImVec2){0,0});
    igSetNextWindowSize((ImVec2){400, 100}, ImGuiCond_Once);
    igBegin(ICON_FA_MUSIC "Hello Dear ImGui!" ICON_FA_AUSTRAL_SIGN, 0, ImGuiWindowFlags_None);
    igColorEdit3(ICON_FA_AUSTRAL_SIGN " Background", &state.pass_action.colors[0].clear_value.r, ImGuiColorEditFlags_None);

    bool bruh = true;
    igShowDemoWindow(&bruh);
    igEnd();
    /*=== UI CODE ENDS HERE ===*/

    sg_begin_pass(&(sg_pass){ .action = state.pass_action, .swapchain = sglue_swapchain() });
    simgui_render();
    sg_end_pass();
    sg_commit();
}

static void cleanup(void) {
    simgui_shutdown();
    sg_shutdown();
}

static void event(const sapp_event* ev) {
    simgui_handle_event(ev);
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    return (sapp_desc){
        .init_cb = init,
        .frame_cb = frame,
        .cleanup_cb = cleanup,
        .event_cb = event,
        .window_title = "DD",
        .width = 800,
        .height = 600,
        .logger.func = slog_func,
        
        .high_dpi = true,
        .enable_clipboard = true,
        // .fullscreen = true,
    };
}
