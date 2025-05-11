#include <fps.hpp>

#include "game_state.hpp"

namespace fps {
using namespace fps::game_input;

void update() {
	startupTime += GetFrameTime();
	handle_raw_input();
	if (IsKeyPressed(KEY_LEFT_ALT) && IsCursorHidden()) {
		EnableCursor();
	}
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && IsCursorOnScreen()) {
		DisableCursor();
	}
	player.update();
}

void handle_raw_input() {
	for (auto [inputType, key] : gameConfig.keybind) {
		if (IsKeyUp(key)) continue;
		inputQueue.emplace_back(inputType, startupTime);
	}
}
}  // namespace fps
