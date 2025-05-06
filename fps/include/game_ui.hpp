#ifndef GAME_UI_HPP_
#define GAME_UI_HPP_ 1
#include <raylib-cpp.hpp>

namespace fps::game_ui {
class GameUI {
   public:
	static void DrawDebugInfo();
	static void DrawCrossHair();
};
}  // namespace fps::game_ui

#endif
