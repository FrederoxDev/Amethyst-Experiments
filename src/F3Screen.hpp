#pragma once
#include <minecraft/src-client/common/client/gui/ScreenView.hpp>
#include <minecraft/src-client/common/client/renderer/screen/MinecraftUIRenderContext.hpp>
#include <minecraft/src-client/common/client/game/IClientInstance.hpp>
#include <minecraft/src-client/common/client/player/LocalPlayer.hpp>
#include <minecraft/src/common/world/level/dimension/Dimension.hpp>
#include <minecraft/src-deps/core/math/Color.hpp>
#include <amethyst/runtime/events/RenderingEvents.hpp>

void RenderF3(AfterRenderUIEvent& event) {
	LocalPlayer* player = event.ctx.mClient->getLocalPlayer();
	if (!player) return;

	const Dimension& dimension = player->getDimensionConst();
	dimension.getId();

	std::string text = fmt::format("Dimension Info:\n  dimension: {}\n  dimension ID: {}", dimension.getId(), dimension.getDimensionId().runtimeID);
	RectangleArea rect{ 2.0f, 100.0f, 40.0f, 100.0f };

	TextMeasureData textData(1.0f, 1, false, false, false);
	CaretMeasureData caretData(0, false);

	event.ctx.drawDebugText(&rect, &text, &mce::Color::BLACK, 1.0f, ui::TextAlignment::Left, &textData, &caretData);
	event.ctx.flushText(0.0);
}