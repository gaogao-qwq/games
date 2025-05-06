#include <game_state.hpp>

namespace fps::game_state {
float                 startupTime = 0.0f;
std::deque<GameInput> inputQueue  = std::deque<GameInput>();
GameConfig            gameConfig  = GameConfig(Resolution(1280, 720));
Player                player      = Player();
raylib::Camera3D      editCamera  = raylib::Camera3D({
		  .position   = Vector3{0.0f, 20.0f, 0.0f},
		  .target     = Vector3{0.0f, 0.0f, 0.1f},
		  .up         = Vector3{0.0f, 1.0f, 0.0f},
		  .fovy       = 80.0f,
		  .projection = CAMERA_PERSPECTIVE,
});

std::vector<std::unique_ptr<GameObject>> gameObjects = []() {
	std::vector<std::unique_ptr<GameObject>> vec;
	vec.emplace_back(std::make_unique<Grid>(32, 1.0f));
	// vec.emplace_back(std::make_unique<Plane>(Vector3{0.0f, 0.0f, 0.0f}, 32.0f, 32.0f, LIGHTGRAY));
	vec.emplace_back(std::make_unique<Block>(Vector3{0.0f, -1.0f, 0.0f}, GREEN));
	vec.emplace_back(std::make_unique<Block>(Vector3{-1.0f, -1.0f, 0.0f}, GREEN));
	vec.emplace_back(std::make_unique<Block>(Vector3{0.0f, -1.0f, -1.0f}, GREEN));
	vec.emplace_back(std::make_unique<Block>(Vector3{-1.0f, -1.0f, -1.0f}, GREEN));
	vec.emplace_back(std::make_unique<Block>(Vector3{-2.0f, 0.0f, -1.0f}, GREEN));
	vec.emplace_back(std::make_unique<Block>(Vector3{-2.0f, -1.0f, -1.0f}, GREEN));
	vec.emplace_back(std::make_unique<Cube>(Vector3{-16.0f, 1.0f, -16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{16.0f, 1.0f, -16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{-16.0f, 1.0f, 16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{16.0f, 1.0f, 16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	// vec.emplace_back(std::make_unique<Block>());
	return vec;
}();
}
