#include "game.h"
#include "raylib.h"

static DebugInfo debugInfo = {
#ifdef DEBUG
	.showPlayerHitbox = true, .showPlayerVelocity = true, .showFPS = true
#else
	.showPlayerHitbox = false, .showPlayerVelocity = false, .showFPS = false
#endif
};

static const i32 screenWidth = 800;
static const i32 screenHeight = 600;

static Player player = {
	.location = {.x = screenHeight / 2.f, .y = screenWidth / 2.f},
	.rect = {.x = 480, .y = 310, .width = 40, .height = 40},
	.hitbox = {.x = 480, .y = 310, .width = 40, .height = 40},
	.velocity = {.vt = 0, .vr = 0, .vb = 0, .vl = 0},
};

static EnvItem envItems[] = {
	{.rect = {.x = -1000, .y = -1000, .width = 2000, .height = 2000},
	 .blocking = false,
	 .moving = false,
	 .color = LIGHTGRAY},
	{.rect = {.x = 0, .y = 400, .width = 1000, .height = 200},
	 .blocking = true,
	 .moving = false,
	 .color = GRAY},
	{.rect = {.x = 0, .y = 0, .width = 100, .height = 400},
	 .blocking = true,
	 .moving = false,
	 .color = GRAY},
	{.rect = {.x = 150, .y = 300, .width = 100, .height = 10},
	 .blocking = true,
	 .moving = false,
	 .color = GRAY}};
static i32 envItemsLength = sizeof(envItems) / sizeof(envItems[0]);

static Camera2D camera = {
	.offset = {.x = screenWidth / 2.f, .y = screenHeight / 2.f},
	.target = {.x = screenHeight / 2.f, .y = screenWidth / 2.f},
	.rotation = 0.f,
	.zoom = 1.f};

void MainLoop() {
	// Update
	//----------------------------------------------------------------------
	f32 fps = GetFPS();
	f32 deltaTime = GetFrameTime();
	f32 wheel = GetMouseWheelMove();

	UpdateDebugInfo();
	UpdatePlayer(&player, envItems, envItemsLength, deltaTime);
	UpdateCamera2D(&camera, player.location, wheel);

	// clang-format off
		// Draw
		//----------------------------------------------------------------------
		BeginDrawing();

			ClearBackground(RAYWHITE);

			BeginMode2D(camera);

				for (i32 i = 0; i < envItemsLength; ++i) {
					DrawRectangleRec(envItems[i].rect, envItems[i].color);
				}
				DrawRectangleRec(player.rect, RED);
				if (debugInfo.showPlayerHitbox) {
					DrawRectangleLines(player.hitbox.x, player.hitbox.y, player.hitbox.width, player.hitbox.height, BLUE);
				}

			EndMode2D();

			UpdateText(player, fps);

		EndDrawing();
	// clang-format on
}

//------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------
i32 main(void) {
	// Initialization
	//--------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "Hmmm...");
	SetConfigFlags(64);
	SetExitKey(KEY_ESCAPE);
	SetTargetFPS(60);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(MainLoop, 0, 1);
#else
	while (!WindowShouldClose()) {
		MainLoop();
	}
#endif

	CloseWindow();
	return 0;
}

void UpdatePlayer(Player *player, EnvItem *envItems, i32 envItemsLength,
				  f32 delta) {
	Vector2 *p = &(player->location);
	Velocity *v = &(player->velocity);
	Rectangle h = player->hitbox;

	// Calculate current velocity
	if (IS_MOVING_UP) {
		v->vt += PLAYER_ACCELERATION;
		if (v->vt > PLAYER_MAX_SPEED) v->vt = PLAYER_MAX_SPEED;
	} else {
		v->vt -= PLAYER_DECELERATION;
		if (v->vt < 0) v->vt = 0;
	}

	if (IS_MOVING_RIGHT) {
		v->vr += PLAYER_ACCELERATION;
		if (v->vr > PLAYER_MAX_SPEED) v->vr = PLAYER_MAX_SPEED;
	} else {
		v->vr -= PLAYER_DECELERATION;
		if (v->vr < 0) v->vr = 0;
	}

	if (IS_MOVING_DOWN) {
		v->vb += PLAYER_ACCELERATION;
		if (v->vb > PLAYER_MAX_SPEED) v->vb = PLAYER_MAX_SPEED;
	} else {
		v->vb -= PLAYER_DECELERATION;
		if (v->vb < 0) v->vb = 0;
	}

	if (IS_MOVING_LEFT) {
		v->vl += PLAYER_ACCELERATION;
		if (v->vl > PLAYER_MAX_SPEED) v->vl = PLAYER_MAX_SPEED;
	} else {
		v->vl -= PLAYER_DECELERATION;
		if (v->vl < 0) v->vl = 0;
	}

	// Move
	h.y -= delta * v->vt;
	h.x += delta * v->vr;
	h.x -= delta * v->vl;
	h.y += delta * v->vb;

	// Collision check
	bool hitObstacle = false;
	for (i32 i = 0; i < envItemsLength; ++i) {
		EnvItem *ei = envItems + i;
		if (ei->blocking && CheckCollisionRecs(h, ei->rect)) {
			Rectangle crec = GetCollisionRec(h, ei->rect);
			hitObstacle = true;
			break;
		}
	}
	// Update player location
	if (!hitObstacle) p->x = h.x, p->y = h.y;
	// Sync rect & hitbox location
	player->rect.x = p->x, player->rect.y = p->y;
	player->hitbox.x = p->x, player->hitbox.y = p->y;
}

void UpdateCamera2D(Camera2D *camera, Vector2 target, f32 wheel) {
	camera->target = target;
	if (wheel != 0) camera->zoom += (wheel * CAMERA_ZOOM_INCREMENT);
	if (camera->zoom < CAMERA_MIN_ZOOM) camera->zoom = CAMERA_MIN_ZOOM;
	if (camera->zoom > CAMERA_MAX_ZOOM) camera->zoom = CAMERA_MAX_ZOOM;
}

void UpdateDebugInfo() {
	if (IsKeyPressed(KEY_H)) {
		debugInfo.showPlayerHitbox = !debugInfo.showPlayerHitbox;
		TraceLog(LOG_INFO, "Toggle showPlayerHitbox to %s",
				 GetBoolalpha(debugInfo.showPlayerHitbox));
	}
	if (IsKeyPressed(KEY_V)) {
		debugInfo.showPlayerVelocity = !debugInfo.showPlayerVelocity;
		TraceLog(LOG_INFO, "Toggle showPlayerSpeed to %s",
				 GetBoolalpha(debugInfo.showPlayerVelocity));
	}
	if (IsKeyPressed(KEY_F)) {
		debugInfo.showFPS = !debugInfo.showFPS;
		TraceLog(LOG_INFO, "Toggle showFPS to %s",
				 GetBoolalpha(debugInfo.showFPS));
	}
}

void UpdateText(Player player, f32 fps) {
	if (debugInfo.showPlayerVelocity) {
		char playerVelocityText[50];
		f32 v = GetHypotenuse(player.velocity.vt - player.velocity.vb,
							  player.velocity.vr - player.velocity.vl);
		sprintf(playerVelocityText, "Player velocity: %f", v);
		DrawText(playerVelocityText, 20, 30, 10, BLACK);
	}
	if (debugInfo.showFPS) {
		char FPSText[50];
		sprintf(FPSText, "Current FPS: %f", fps);
		DrawText(FPSText, 20, 20, 10, BLACK);
	}
}
