#include <game_state.hpp>
#include <game_ui.hpp>

namespace fps::game_ui {
using namespace fps::game_state;

void GameUI::DrawDebugInfo() {
	raylib::DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, BLACK);
	raylib::DrawText(
		TextFormat("player pos: (%f %f %f)", player.position().x, player.position().y, player.position().z),
		10, 30, 20, BLACK);
	raylib::DrawText(
		TextFormat("player camera target: (%f %f %f)", player.camera.target.x, player.camera.target.y, player.camera.target.z),
		10, 50, 20, BLACK);
	raylib::DrawText(
		TextFormat("player facing degree: %f", player.facing_angle()),
		10, 70, 20, BLACK);
	raylib::DrawText(
		TextFormat("player facing: %s", direction_to_string(player.facing_direction())),
		10, 90, 20, BLACK);
	raylib::DrawText(
		TextFormat("player velocity: (%f %f %f)", player.velocity.x, player.velocity.y, player.velocity.z),
		10, 110, 20, BLACK);
	raylib::DrawText(TextFormat("startup time: %.2f", startupTime), 10, 130, 20, BLACK);
}

void GameUI::DrawCrossHair() {
	int screenWidth  = gameConfig.resolution.width;
	int screenHeight = gameConfig.resolution.height;
	DrawRectangle(screenWidth / 2 - 10, screenHeight / 2 - 1, 20, 2, ColorAlpha(BLACK, 0.8f));
	DrawRectangle(screenWidth / 2 - 1, screenHeight / 2 - 10, 2, 20, ColorAlpha(BLACK, 0.8f));
}
}  // namespace fps::game_ui
