#include "dllmain.h"
#include <chrono>
#include <cmath>
#include <math.h>
#include <vector>

AmethystContext* amethyst;

std::vector<std::string> pathedNameToTokens(std::string pathedName) {
    std::vector<std::string> tokens;
    size_t start = 0, pos;

    while ((pos = pathedName.find('\\', start)) != std::string::npos) {
        tokens.push_back(pathedName.substr(start, pos - start));
        start = pos + 1;
    }

    tokens.push_back(pathedName.substr(start));
    return tokens;
}

UIControl* getFromPath(UIControl* root, std::string pathedName) {
    std::vector<std::string> path = pathedNameToTokens(pathedName);

    UIControl* current = root;

    for (size_t i = 0; i < path.size(); ++i) {
        bool found = false;

        for (auto& child : current->mChildren) {
            if (child->mName != path[i]) continue;
            current = child.get();
            found = true;
            break;
        }

        if (!found) return nullptr;
    }

    return current;
}

void exploreToggle(UIControl* self, int depth) {
    TextComponent* component = self->getComponent<TextComponent>();

    if (component) {
        if (component->mLabel == "AMETHYST!") {
            Log::Info("name: {}, text: '{}', path: '{}'", self->mName, component->mLabel, self->getPathedName());
        }
    }

    for (auto& child : self->mChildren) {
        exploreToggle(child.get(), depth + 1);
    }
}

void BeforeRenderUI(ScreenView* screenView, MinecraftUIRenderContext* ctx) {
    UIControl* rootControl = screenView->visualTree->mRootControlName;
    //exploreToggle(rootControl, 0);

    std::string text("");

    UIControl* slider = getFromPath(rootControl, "variables_button_mappings_and_controls\\safezone_screen_matrix\\inner_matrix\\safezone_screen_panel\\root_screen_panel\\stack_panel\\content_panel\\container\\settings_common.dialog_content\\content_area\\control\\scrolling_panel\\scroll_mouse\\scroll_view\\stack_panel\\background_and_viewport\\scrolling_view_port\\scrolling_content\\amethyst_section\\atlas_section\\atlas_slider\\option_generic_core\\two_line_layout\\settings_common.option_slider_control\\slider");
    if (slider) {
        SliderComponent* component = slider->getComponent<SliderComponent>();
        text = fmt::format("slider: {}%", component->mPercentage * 100);
    }

    UIControl* label = getFromPath(rootControl, "variables_button_mappings_and_controls\\safezone_screen_matrix\\inner_matrix\\safezone_screen_panel\\root_screen_panel\\stack_panel\\content_panel\\container\\settings_common.dialog_content\\content_area\\control\\scrolling_panel\\scroll_mouse\\scroll_view\\stack_panel\\background_and_viewport\\scrolling_view_port\\scrolling_content\\amethyst_section\\atlas_section\\test_label");
    if (label) {
        TextComponent* component = label->getComponent<TextComponent>();
        component->setText(text);
    }
}

SafetyHookInline _setChecked;

int64_t setChecked(ToggleComponent* self, int64_t a2, int64_t a3) {
    Log::Info("0x{:x}, {} {}", reinterpret_cast<uintptr_t>(self->vtable) - GetMinecraftBaseAddress(), a2, a3);
    return _setChecked.call<int64_t>(self, a2, a3);
}

ModFunction void Initialize(AmethystContext* _amethyst)
{
    amethyst = _amethyst;
    amethyst->mEventManager.beforeRenderUI.AddListener(&BeforeRenderUI); 

    /*amethyst->mHookManager.RegisterFunction<&ToggleComponent::setChecked>("40 53 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B D9 F3 0F 11 4C 24 ? 48 8B 49");
    amethyst->mHookManager.CreateHook<&ToggleComponent::setChecked>(_setChecked, &setChecked);*/
}

// screen_controls_and_settings\variables_button_mappings_and_controls\safezone_screen_matrix\inner_matrix\safezone_screen_panel\root_screen_panel\stack_panel\content_panel\container\settings_common.dialog_content\content_area\control\scrolling_panel\scroll_mouse\scroll_view\stack_panel\background_and_viewport\scrolling_view_port\scrolling_content\amethyst_section\atlas_section\atlas_slider\option_generic_core\two_line_layout\settings_common.option_slider_control\slider