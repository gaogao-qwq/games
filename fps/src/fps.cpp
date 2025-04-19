#include <fps.hpp>

namespace fps {
float player_acceleration       = 32.0f;
float player_deceleration       = 48.0f;
float player_max_velocity       = 8.0f;
float player_yaw_acceleration   = 12.0f;
float player_pitch_acceleration = 8.0f;
float startupTime               = 0.0f;

std::deque<GameInput> inputQueue = std::deque<GameInput>();
GameConfig            gameConfig = fps::GameConfig(Resolution(1280, 720));
Player                player     = fps::Player();
raylib::Camera3D      editCamera = raylib::Camera3D({
		 .position   = Vector3{0.0f, 20.0f, 0.0f},
		 .target     = Vector3{0.0f, 0.0f, 0.1f},
		 .up         = Vector3{0.0f, 1.0f, 0.0f},
		 .fovy       = 80.0f,
		 .projection = CAMERA_PERSPECTIVE,
});

std::vector<std::unique_ptr<GameObject>> gameObjects = []() {
	std::vector<std::unique_ptr<GameObject>> vec;
	vec.emplace_back(std::make_unique<Grid>(10, 5.0f));
	// vec.emplace_back(std::make_unique<Plane>(Vector3{0.0f, 0.0f, 0.0f}, 32.0f, 32.0f, LIGHTGRAY));
	vec.emplace_back(std::make_unique<Cube>(Vector3{-16.0f, 1.0f, -16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{16.0f, 1.0f, -16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{-16.0f, 1.0f, 16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{16.0f, 1.0f, 16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	return vec;
}();

void update() {
	startupTime += GetFrameTime();
	// TODO: input queue
	update_player();
	printf("update\n");
}

// TODO: implement me
void handle_raw_input() {
}

void update_player() {
	float dt = GetFrameTime();

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
