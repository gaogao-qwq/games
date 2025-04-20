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
	float dt = GetFrameTime();

	while (!inputQueue.empty()) {
		auto type = inputQueue.front().type;
		inputQueue.pop_front();
	}

	if (!(IsKeyDown(KEY_W) && IsKeyDown(KEY_S))) {
		// forward
		if (IsKeyDown(KEY_W)) {
			if (player.velocity.z >= -player_max_velocity) {
				player.velocity.z = std::max(player.velocity.z - player_acceleration * dt, -player_max_velocity);
			}
		} else {
			if (player.velocity.z < 0.0f) {
				player.velocity.z = std::min(player.velocity.z + player_deceleration * dt, 0.0f);
			}
		}

		// backward
		if (IsKeyDown(KEY_S)) {
			if (player.velocity.z <= player_max_velocity) {
				player.velocity.z = std::min(player.velocity.z + player_acceleration * dt, player_max_velocity);
			}
		} else {
			if (player.velocity.z > 0.0f) {
				player.velocity.z = std::max(player.velocity.z - player_deceleration * dt, 0.0f);
			}
		}
	} else {
		player.velocity.z = 0.0f;
	}

	if (!(IsKeyDown(KEY_A) && IsKeyDown(KEY_D))) {
		// left
		if (IsKeyDown(KEY_A)) {
			if (player.velocity.x >= -player_max_velocity) {
				player.velocity.x = std::max(player.velocity.x - player_acceleration * dt, -player_max_velocity);
			}
		} else {
			if (player.velocity.x < 0.0f) {
				player.velocity.x = std::min(player.velocity.x + player_deceleration * dt, 0.0f);
			}
		}

		// right
		if (IsKeyDown(KEY_D)) {
			if (player.velocity.x <= player_max_velocity) {
				player.velocity.x = std::min(player.velocity.x + player_acceleration * dt, player_max_velocity);
			}
		} else {
			if (player.velocity.x > 0.0f) {
				player.velocity.x = std::max(player.velocity.x - player_deceleration * dt, 0.0f);
			}
		}
	} else {
		player.velocity.x = 0.0f;
	}

	Vector3 movement = {
		.x = (-player.velocity.z) * dt,  // forward-backward
		.y = player.velocity.x * dt,     // right-left
		.z = player.velocity.y * dt,     // up-down
	};

	// rotating camera
	Vector3 rotation = {
		.x = GetMouseDelta().x * dt * player_yaw_acceleration,    // yaw
		.y = GetMouseDelta().y * dt * player_pitch_acceleration,  // pitch
		.z = 0.0f,                                                // roll
	};
	float zoom = 0.0f;
	player.camera.Update(movement, rotation, zoom);

	Vector2 directionVector = raylib::Vector2(player.camera.target.x, player.camera.target.z) - raylib::Vector2(player.position().x, player.position().z);
	player.angle_deg        = Vector2Angle(NORTH_VEC, directionVector) * RAD2DEG;
	player.facing           = deg_to_direction(player.angle_deg);
}
}  // namespace fps
