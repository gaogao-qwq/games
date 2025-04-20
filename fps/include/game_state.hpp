#ifndef GAME_STATE_HPP_
#define GAME_STATE_HPP_ 1
#include <deque>
#include <game_config.hpp>
#include <game_object.hpp>
#include <memory>

namespace fps::game_state {
using namespace fps::game_object;
using namespace fps::game_config;

extern float                                    startupTime;
extern std::deque<GameInput>                    inputQueue;
extern GameConfig                               gameConfig;
extern Player                                   player;
extern raylib::Camera3D                         editCamera;
extern std::vector<std::unique_ptr<GameObject>> gameObjects;
}  // namespace fps::game_state

#endif
