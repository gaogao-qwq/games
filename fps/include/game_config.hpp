#ifndef GAME_CONFIG_HPP_
#define GAME_CONFIG_HPP_ 1
#include <game_input.hpp>
#include <raylib-cpp.hpp>
#include <unordered_map>

namespace fps::game_config {
using namespace fps::game_input;

class Resolution {
   public:
	int width;
	int height;

	Resolution(int width = 800, int height = 600)
		: width(width), height(height) {};
};

class GameConfig {
   public:
	Resolution                                 resolution;
	double                                     fov;
	std::unordered_map<GameInputType, KeyCode> keybind;

	GameConfig(
		Resolution resolution = Resolution(800, 600), double fov = 60.0f,
		std::unordered_map<GameInputType, KeyCode> keybind = {
			{GameInputType::MOVING_FORWARD, KEY_W},
			{GameInputType::MOVING_RIGHT, KEY_D},
			{GameInputType::MOVING_BACKWARD, KEY_S},
			{GameInputType::MOVING_LEFT, KEY_A},
		}) : resolution(resolution), fov(fov), keybind(keybind) {};
};

extern float gravity_acceleration;
extern float gravity_max_velocity;
extern float player_acceleration;
extern float player_deceleration;
extern float player_max_velocity;
extern float player_yaw_acceleration;
extern float player_pitch_acceleration;
}  // namespace fps::game_config

#endif
