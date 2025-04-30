#include <fps.hpp>

int main(void) {
	SetConfigFlags(FLAG_VSYNC_HINT);
	raylib::Window        window(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height, "fps");
	raylib::RenderTexture playerScreen(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height);
	raylib::RenderTexture editScreen(200, 100);
	raylib::Rectangle     playerScreenRect(0.0f, 0.0f, (float)fps::gameConfig.resolution.width, (float)-fps::gameConfig.resolution.height);
	raylib::Rectangle     editScreenRect(0, 0, 200.0f, -100.0f);

	DisableCursor();

	while (!window.ShouldClose()) {
		fps::update();

		// clang-format off
		playerScreen.BeginMode();
			ClearBackground(Color{185, 237, 255, 255});
			fps::player.camera.BeginMode();
				for (auto &obj : fps::gameObjects) {
					obj->DrawBounding();
					obj->Draw();
				}
			fps::player.camera.EndMode();
			fps::player.DrawCrosshair(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height);
		playerScreen.EndMode();

		editScreen.BeginMode();
			ClearBackground(RAYWHITE);
			DrawRectangleLinesEx(Rectangle{0.0f, 0.0f, 200.0f, 100.0f}, 1.0f, BLACK);
			fps::editCamera.BeginMode();
				for (auto &obj : fps::gameObjects) obj->Draw();
				fps::player.DrawBoundingBox();
				fps::player.DrawTarget();
			fps::editCamera.EndMode();
		editScreen.EndMode();

		window.BeginDrawing();
			window.ClearBackground(RAYWHITE);

			DrawTextureRec(playerScreen.texture, playerScreenRect, Vector2{0.0f, 0.0f}, RAYWHITE);
			DrawTextureRec(editScreen.texture, editScreenRect, Vector2{fps::gameConfig.resolution.width - 210.0f, 10.0f}, RAYWHITE);

			raylib::DrawText(
				TextFormat("player pos: (%f %f %f)", fps::player.position().x, fps::player.position().y, fps::player.position().z),
				10, 40, 10, BLACK);
			raylib::DrawText(
				TextFormat("player camera target: (%f %f %f)", fps::player.camera.target.x, fps::player.camera.target.y, fps::player.camera.target.z),
				10, 50, 10, BLACK);
			raylib::DrawText(
				TextFormat("player facing degree: %f", fps::player.FacingAngle()),
				10, 60, 10, BLACK);
			raylib::DrawText(
				TextFormat("player facing: %s", fps::direction_to_string(fps::player.FacingDirection())),
				10, 70, 10, BLACK);
			raylib::DrawText(
				TextFormat("player velocity: (%f %f %f)", fps::player.velocity.x, fps::player.velocity.y, fps::player.velocity.z),
				10, 80, 10, BLACK);
			raylib::DrawText(TextFormat("startup time: %.2f", fps::startupTime), 10, 90, 10, BLACK);
			window.DrawFPS();
		window.EndDrawing();
		// clang-format on
	}
}
