#include <fps.hpp>

int main(void) {
	raylib::Window        window(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height, "fps", FLAG_VSYNC_HINT);
	raylib::RenderTexture playerScreen(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height);
	raylib::RenderTexture editScreen(200, 100);
	raylib::Rectangle     playerScreenRect(0.0f, 0.0f, (float)fps::gameConfig.resolution.width, (float)-fps::gameConfig.resolution.height);
	raylib::Rectangle     editScreenRect(0, 0, 200.0f, -100.0f);

	SetWindowMonitor(0);
	DisableCursor();

	while (!window.ShouldClose()) {
		fps::update();

		// clang-format off
		playerScreen.BeginMode();
			ClearBackground(Color{185, 237, 255, 255});
			fps::player.camera.BeginMode();
				for (auto &obj : fps::gameObjects) {
					obj->draw_bounding();
					obj->draw();
				}
			fps::player.camera.EndMode();
		playerScreen.EndMode();

		editScreen.BeginMode();
			ClearBackground(RAYWHITE);
			DrawRectangleLinesEx(Rectangle{0.0f, 0.0f, 200.0f, 100.0f}, 1.0f, BLACK);
			fps::editCamera.BeginMode();
				for (auto &obj : fps::gameObjects) obj->draw();
				fps::player.draw_bounding_box();
				fps::player.draw_target();
			fps::editCamera.EndMode();
		editScreen.EndMode();

		window.BeginDrawing();
			window.ClearBackground(RAYWHITE);

			DrawTextureRec(playerScreen.texture, playerScreenRect, Vector2{0.0f, 0.0f}, RAYWHITE);
			DrawTextureRec(editScreen.texture, editScreenRect, Vector2{fps::gameConfig.resolution.width - 210.0f, 10.0f}, RAYWHITE);

			fps::GameUI::DrawDebugInfo();
			fps::GameUI::DrawCrossHair();
		window.EndDrawing();
		// clang-format on
	}
}
