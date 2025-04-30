#include <fps.hpp>

#include "game_state.hpp"

namespace fps {
using namespace fps::game_input;

void update() {
	startupTime += GetFrameTime();
	handle_raw_input();
	update_player();
}

void handle_raw_input() {
	for (auto [inputType, key] : gameConfig.keybind) {
		if (IsKeyUp(key)) continue;
		inputQueue.emplace_back(inputType, startupTime);
	}
}

void update_player() {
	while (!inputQueue.empty()) {
		auto type = inputQueue.front().type;
		inputQueue.pop_front();
	}
	player.Update();
}
}  // namespace fps
