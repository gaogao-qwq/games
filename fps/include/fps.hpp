#ifndef FPS_HPP_
#define FPS_HPP_ 1
#define RCAMERA_IMPLEMENTATION

#include <direction.hpp>
#include <game_config.hpp>
#include <game_input.hpp>
#include <game_object.hpp>
#include <game_state.hpp>
#include <game_ui.hpp>
#include <raylib-cpp.hpp>

namespace fps {
using namespace fps::direction;
using namespace fps::game_config;
using namespace fps::game_input;
using namespace fps::game_object;
using namespace fps::game_state;
using namespace fps::game_ui;

extern void update();
extern void handle_raw_input();
extern void update_player();
extern void handle_raw_input();
}  // namespace fps

#endif
