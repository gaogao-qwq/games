#ifndef FPS_HPP_
#define FPS_HPP_ 1

#include <deque>
#include <direction.hpp>
#include <game_object.hpp>
#include <memory>
#include <player.hpp>
#include <raylib-cpp.hpp>

namespace fps {
using namespace fps::direction;
using namespace fps::game_object;

typedef enum GameInputType {
	MOVING_FORWARD,
	MOVING_RIGHT,
	MOVING_BACKWARD,
	MOVING_LEFT,
} GameInputType;

class GameInput {
   public:
	GameInputType type;

	GameInput(GameInputType type) : type(type) {}
};

struct Resolution {
	int width;
	int height;

	Resolution(int width = 800, int height = 600)
		: width(width), height(height) {};
};

class GameConfig {
   public:
	Resolution resolution;
	double     fov;

	GameConfig(Resolution resolution = Resolution(800, 600), double fov = 60.0f)
		: resolution(resolution), fov(fov) {};
};

extern float player_acceleration;
extern float player_deceleration;
extern float player_max_velocity;
extern float player_yaw_acceleration;
extern float player_pitch_acceleration;

extern float                                    startupTime;
extern std::deque<GameInput>                    inputQueue;
extern GameConfig                               gameConfig;
extern Player                                   player;
extern raylib::Camera3D                         editCamera;
extern std::vector<std::unique_ptr<GameObject>> gameObjects;

void update();
void handle_raw_input();
void update_player();
}  // namespace fps

#endif
