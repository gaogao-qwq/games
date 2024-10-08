#include "sim.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "op_queue.h"
#include "raylib.h"
#include "util.h"

static DebugInfo debugInfo = {
#ifdef DEBUG
	.showBrushSize = true,
	.showBrushCursorPosition = true,
	.showFrameTime = true,
	.showFPS = true,
	.showBrushInfo = true,
	.showCanvasPrefabInfo = false,
	.showOpQueueInfo = true
#else
	.showBrushSize = false,
	.showBrushCursorPosition = false,
	.showFrameTime = false,
	.showFPS = false,
	.showBrushInfo = false,
	.showCanvasPrefabInfo = false,
	.showOpQueueInfo = false
#endif
};

static const int screenWidth = PARTICLE_SIZE * CANVAS_SIZE;
static const int screenHeight = PARTICLE_SIZE * CANVAS_SIZE;
static const float updateFrameTime = 1.0 / (float)TARGET_TICKRATE;

static float accumulatedFrameTime = 0.0;
static BrushCursor brushCursor = {{0}, SAND_COLOR, 4, 0, NULL, PARTICLE_SAND};
static Canvas canvas;
static OpQueue *opQueue;
static CanvasPrefab canvasPrefab;

int main(void) {
	InitWindow(screenWidth, screenHeight, "Sim");
	InitCanvas(CANVAS_SIZE, CANVAS_SIZE);
	opQueue = MakeEmptyOpQueue();
	SetExitKey(KEY_ESCAPE);
	SetConfigFlags(FLAG_VSYNC_HINT);

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

// clang-format off
void MainLoop() {
	// Update
	UpdateBrushCursor(&brushCursor);
	HandleBrushOperation();
	HandleOperation();

	accumulatedFrameTime += GetFrameTime();
	while (accumulatedFrameTime >= updateFrameTime) {
		UpdateGameTick();
		accumulatedFrameTime -= updateFrameTime;
	}

	// Draw
	BeginDrawing();
		ClearBackground(BLACK);
		DrawCanvasPrefab(canvasPrefab);
		DrawBrushCursor(brushCursor);
		DrawDebugInfo(brushCursor);
	EndDrawing();
}
// clang-format on

void UpdateGameTick() {
	UpdateParticles(&canvas);
	UpdateCanvasPrefab(&canvas);
}

void SwitchBrushType(BrushCursor *cursor, ParticleType type) {
	printf("%d\n", type);
	cursor->type = type;
	cursor->color = GetParticleByType(type).color;
}

Particle GetParticleByType(ParticleType type) {
	switch (type) {
		case PARTICLE_BORDER:
			return BORDER;
		case PARTICLE_AIR:
			return AIR;
		case PARTICLE_SAND:
			return SAND;
		case PARTICLE_WATER:
			return WATER;
		case PARTICLE_STONE:
			return STONE;
		case PARTICLE_WOOD:
			return WOOD;
		default:
			return AIR;
	}
}

void SwapParticle(Particle *a, Particle *b) {
	Particle tmp = *a;
	*a = *b, *b = tmp;
}

void HandleOperation() {
	if (!opQueue->len) return;

	QueueNode *node = opQueue->front;
	switch (node->op.type) {
		case OP_BRUSH_DRAW:
			BrushDraw(brushCursor, &canvas);
	}
	OpQueuePop(opQueue);
}

void UpdateBrushCursor(BrushCursor *cursor) {
	float wheel = GetMouseWheelMove();
	if (wheel != 0.0) {
		free(cursor->points);
		cursor->points = NULL;
		cursor->p_count = 0;
		if (wheel < 0.0) {
			cursor->size -= cursor->size > 1 ? 1 : 0;
		} else {
			++cursor->size;
		}
	}

	Vector2 mousePosition = GetMousePosition();
	int x = (int)roundf(mousePosition.x);
	int y = (int)roundf(mousePosition.y);
	int modx = x % PARTICLE_SIZE;
	int mody = y % PARTICLE_SIZE;
	x += roundf(modx / (float)PARTICLE_SIZE) ? PARTICLE_SIZE - modx : -modx;
	y += roundf(mody / (float)PARTICLE_SIZE) ? PARTICLE_SIZE - mody : -mody;
	x -= cursor->size / 2 * PARTICLE_SIZE;
	y -= cursor->size / 2 * PARTICLE_SIZE;
	cursor->position = (IntVec2){x, y};

	// Guard clause
	if (cursor->points != NULL) return;

	IntVec2 *brush_points =
		malloc(sizeof(IntVec2) * cursor->size * cursor->size);
	float radius = cursor->size / 2.0;
	// Odd size & even size
	if (cursor->size & 1) {
		for (int i = 0; i < radius; ++i) {
			for (int j = 0; j < radius; ++j) {
				if (floorf(Hypotenuse(radius - i, radius - j)) > radius)
					continue;
				brush_points[cursor->p_count++] = (IntVec2){i, j};
				if (i != (int)radius) {
					brush_points[cursor->p_count++] =
						(IntVec2){cursor->size - i - 1, j};
				}
				if (j != (int)radius) {
					brush_points[cursor->p_count++] =
						(IntVec2){i, cursor->size - j - 1};
				}
				if (i != (int)radius && j != (int)radius) {
					brush_points[cursor->p_count++] =
						(IntVec2){cursor->size - i - 1, cursor->size - j - 1};
				}
			}
		}
	} else {
		for (int i = 0; i < radius; ++i) {
			for (int j = 0; j < radius; ++j) {
				if (roundf(Hypotenuse(radius - i, radius - j)) > radius)
					continue;
				brush_points[cursor->p_count++] = (IntVec2){i, j};
				brush_points[cursor->p_count++] =
					(IntVec2){cursor->size - i - 1, j};
				brush_points[cursor->p_count++] =
					(IntVec2){i, cursor->size - j - 1};
				brush_points[cursor->p_count++] =
					(IntVec2){cursor->size - i - 1, cursor->size - j - 1};
			}
		}
	}
	cursor->points = brush_points;
}

void HandleBrushOperation() {
	switch (GetKeyPressed()) {
		case KEY_ONE:
			SwitchBrushType(&brushCursor, PARTICLE_AIR);
			break;
		case KEY_TWO:
			SwitchBrushType(&brushCursor, PARTICLE_SAND);
			break;
		case KEY_THREE:
			SwitchBrushType(&brushCursor, PARTICLE_WATER);
			break;
		case KEY_FOUR:
			SwitchBrushType(&brushCursor, PARTICLE_STONE);
			break;
		case KEY_FIVE:
			SwitchBrushType(&brushCursor, PARTICLE_WOOD);
			break;
		default:
			break;
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		OpQueuePush(opQueue, (Operation){.type = OP_BRUSH_DRAW, .data = NULL});
	}
}

void UpdateSand(Canvas *canvas, size_t r, size_t c) {
	if (r + 1 >= canvas->height) return;
	if (canvas->particles[r + 1][c].type == PARTICLE_AIR) {
		// Down
		SwapParticle(&canvas->particles[r][c], &canvas->particles[r + 1][c]);
		canvas->particles[r + 1][c].updated = true;
	} else if (canvas->particles[r + 1][c].type == PARTICLE_WATER) {
		SwapParticle(&canvas->particles[r][c], &canvas->particles[r + 1][c]);
		canvas->particles[r + 1][c].updated = true;
		UpdateWater(canvas, r, c);
	} else if (c > 0 && canvas->particles[r + 1][c - 1].type == PARTICLE_AIR) {
		// Left down
		SwapParticle(&canvas->particles[r][c],
					 &canvas->particles[r + 1][c - 1]);
		canvas->particles[r + 1][c - 1].updated = true;
	} else if (c > 0 &&
			   canvas->particles[r + 1][c - 1].type == PARTICLE_WATER) {
		SwapParticle(&canvas->particles[r][c],
					 &canvas->particles[r + 1][c - 1]);
		canvas->particles[r + 1][c - 1].updated = true;
		UpdateWater(canvas, r, c);
	} else if (c + 1 < canvas->width &&
			   canvas->particles[r + 1][c + 1].type == PARTICLE_AIR) {
		// Right down
		SwapParticle(&canvas->particles[r][c],
					 &canvas->particles[r + 1][c + 1]);
		canvas->particles[r + 1][c + 1].updated = true;
	} else if (c + 1 < canvas->width &&
			   canvas->particles[r + 1][c + 1].type == PARTICLE_WATER) {
		SwapParticle(&canvas->particles[r][c],
					 &canvas->particles[r + 1][c + 1]);
		canvas->particles[r + 1][c + 1].updated = true;
		UpdateWater(canvas, r, c);
	}
}

void UpdateWater(Canvas *canvas, size_t r, size_t c) {
	// Down
	if (r + 1 < canvas->height &&
		canvas->particles[r + 1][c].type == PARTICLE_AIR) {
		SwapParticle(&canvas->particles[r][c], &canvas->particles[r + 1][c]);
		canvas->particles[r + 1][c].updated = true;
		return;
	}

	// Left down or Right down
	if (r + 1 < canvas->height && c > 0 && c + 1 < canvas->width &&
		canvas->particles[r + 1][c - 1].type == PARTICLE_AIR &&
		canvas->particles[r + 1][c + 1].type == PARTICLE_AIR) {
		return;
	}

	// Left down
	if (r + 1 < canvas->height && c > 0 &&
		canvas->particles[r + 1][c - 1].type == PARTICLE_AIR) {
		SwapParticle(&canvas->particles[r][c],
					 &canvas->particles[r + 1][c - 1]);
		canvas->particles[r + 1][c - 1].updated = true;
		return;
	}

	// Right down
	if (r + 1 < canvas->height && c + 1 < canvas->width &&
		canvas->particles[r + 1][c + 1].type == PARTICLE_AIR) {
		SwapParticle(&canvas->particles[r][c],
					 &canvas->particles[r + 1][c + 1]);
		canvas->particles[r + 1][c + 1].updated = true;
		return;
	}

	// Left
	if (c > 0 && canvas->particles[r][c - 1].type == PARTICLE_AIR) {
		canvas->particles[r + 1][c + 1].updated = true;
		SwapParticle(&canvas->particles[r][c], &canvas->particles[r][c - 1]);
		canvas->particles[r][c - 1].updated = true;
		return;
	}

	// Right
	if (c + 1 < canvas->width &&
		canvas->particles[r][c + 1].type == PARTICLE_AIR) {
		SwapParticle(&canvas->particles[r][c], &canvas->particles[r][c + 1]);
		canvas->particles[r][c + 1].updated = true;
		return;
	}
}

void UpdateParticles(Canvas *canvas) {
	for (size_t r = canvas->height - 1; r != SIZE_MAX; --r) {
		for (size_t c = 0; c < canvas->width; ++c) {
			if (canvas->particles[r][c].updated) continue;
			switch (canvas->particles[r][c].type) {
				case PARTICLE_SAND:
					UpdateSand(canvas, r, c);
					break;
				case PARTICLE_WATER:
					UpdateWater(canvas, r, c);
					break;
				default:
					break;
			}
		}
	}

	for (size_t r = 0; r < canvas->height; ++r) {
		for (size_t c = 0; c < canvas->width; ++c) {
			canvas->particles[r][c].updated = false;
		}
	}
}

void UpdateCanvasPrefab(Canvas *canvas) {
	size_t i, j, r, c, width, height, idx = 0;
	bool flagw, flagh;
	ParticleType type;

	canvasPrefab.len = 0;
	if (canvasPrefab.recs == NULL) {
		canvasPrefab.recs = malloc(sizeof(Rectangle));
		canvasPrefab.colors = malloc(sizeof(Color));
	}

	bool **vis = malloc(sizeof(bool *) * canvas->height);
	for (size_t i = 0; i < canvas->width; ++i)
		vis[i] = calloc(canvas->width, sizeof(bool));

	for (i = 0; i < canvas->height; ++i) {
		for (j = 0; j < canvas->width; ++j) {
			if (vis[i][j] || canvas->particles[i][j].flag & PARTICLE_INVISIBLE)
				continue;

			vis[i][j] = true;
			type = canvas->particles[i][j].type;
			width = 1, height = 1, flagw = false, flagh = false;

			while (true) {
				if (i + height >= canvas->height) flagh = true;
				if (j + width >= canvas->width) flagw = true;
				if (flagh && flagw) break;

				if (!flagw) {
					for (r = i, c = j + width; r < i + height; ++r) {
						if (vis[r][c] || canvas->particles[r][c].type != type) {
							flagw = true;
							break;
						}
					}
					if (!flagw) {
						++width;
						for (r = i; r < i + height; ++r) vis[r][c] = true;
					};
				}

				if (!flagh) {
					for (r = i + height, c = j; c < j + width; ++c) {
						if (vis[r][c] || canvas->particles[r][c].type != type) {
							flagh = true;
							break;
						}
					}
					if (!flagh) {
						++height;
						for (c = j; c < j + width; ++c) vis[r][c] = true;
					};
				}
			}

			if (idx >= canvasPrefab.len) {
				canvasPrefab.len = idx + 1;
				canvasPrefab.recs = realloc(
					canvasPrefab.recs, sizeof(Rectangle) * canvasPrefab.len);
				canvasPrefab.colors = realloc(canvasPrefab.colors,
											  sizeof(Color) * canvasPrefab.len);
			}

			canvasPrefab.recs[idx] =
				(Rectangle){j * PARTICLE_SIZE, i * PARTICLE_SIZE,
							width * PARTICLE_SIZE, height * PARTICLE_SIZE};
			canvasPrefab.colors[idx++] = canvas->particles[i][j].color;

			if (width == canvas->width) i = height - 1;
			if (height == canvas->height) j = width - 1;
		}
	}

	for (size_t i = 0; i < canvas->width; ++i) free(vis[i]);
	free(vis);
}

void DrawBrushCursor(BrushCursor cursor) {
	for (size_t i = 0; i < cursor.p_count; ++i) {
		DrawRectangleRec(
			(Rectangle){cursor.position.x + cursor.points[i].x * PARTICLE_SIZE,
						cursor.position.y + cursor.points[i].y * PARTICLE_SIZE,
						PARTICLE_SIZE, PARTICLE_SIZE},
			cursor.color);
	}
}

void BrushDraw(BrushCursor cursor, Canvas *canvas) {
	for (size_t i = 0; i < cursor.p_count; ++i) {
		size_t r = (cursor.position.y + (cursor.points[i].y * PARTICLE_SIZE)) /
				   PARTICLE_SIZE;
		size_t c = (cursor.position.x + (cursor.points[i].x * PARTICLE_SIZE)) /
				   PARTICLE_SIZE;
		if (r < canvas->height && c < canvas->width) {
			canvas->particles[r][c] = GetParticleByType(cursor.type);
		}
	}
}

void InitCanvas(size_t width, size_t height) {
	canvas.width = width, canvas.height = height;
	canvas.particles = malloc(sizeof(Particle *) * height);
	for (int i = 0; i < CANVAS_SIZE; ++i) {
		canvas.particles[i] = malloc(sizeof(Particle) * width);
	}

	for (size_t r = 0; r < height; ++r) {
		for (size_t c = 0; c < width; ++c) {
			if (r == 0 || c == 0 || r == height - 1 || c == width - 1) {
				canvas.particles[r][c] = BORDER;
			} else {
				canvas.particles[r][c] = AIR;
			}
		}
	}
}

void DrawCanvasPrefab(CanvasPrefab canvasPrefab) {
	for (size_t i = 0; i < canvasPrefab.len; ++i)
		DrawRectangleRec(canvasPrefab.recs[i], canvasPrefab.colors[i]);
}

void DrawDebugInfo(BrushCursor cursor) {
	if (debugInfo.showBrushSize) {
		char brushSizeText[64];
		sprintf(brushSizeText, "Brush size: %lu", cursor.size);
		DrawText(brushSizeText, 50, 50, 10, RAYWHITE);
	}

	if (debugInfo.showBrushCursorPosition) {
		char brushCursorLocationText[64];
		sprintf(brushCursorLocationText, "Brush position: x: %d, y: %d",
				cursor.position.x, cursor.position.y);
		DrawText(brushCursorLocationText, 50, 60, 10, RAYWHITE);
	}

	if (debugInfo.showFrameTime) {
		char frameTimeText[64];
		sprintf(frameTimeText, "Frame time: %f", GetFrameTime());
		DrawText(frameTimeText, 50, 70, 10, RAYWHITE);
	}

	if (debugInfo.showFPS) {
		char FPSText[64];
		sprintf(FPSText, "FPS: %d", GetFPS());
		DrawText(FPSText, 50, 80, 10, RAYWHITE);
	}

	if (debugInfo.showBrushInfo) {
		char brushInfoText[64];
		switch (cursor.type) {
			case PARTICLE_BORDER:
				sprintf(brushInfoText, "Brush type: Border");
				break;
			case PARTICLE_AIR:
				sprintf(brushInfoText, "Brush type: Air");
				break;
			case PARTICLE_SAND:
				sprintf(brushInfoText, "Brush type: Sand");
				break;
			case PARTICLE_WATER:
				sprintf(brushInfoText, "Brush type: Water");
				break;
			case PARTICLE_STONE:
				sprintf(brushInfoText, "Brush type: Stone");
				break;
			case PARTICLE_WOOD:
				sprintf(brushInfoText, "Brush type: Wood");
				break;
			default:
				sprintf(brushInfoText, "Brush type: Unknown");
				break;
		}
		DrawText(brushInfoText, 50, 90, 10, RAYWHITE);
	}

	if (debugInfo.showOpQueueInfo) {
		char OpQueueInfoText[64];
		sprintf(OpQueueInfoText, "OpQueue len: %lu", opQueue->len);
		DrawText(OpQueueInfoText, 50, 100, 10, RAYWHITE);
	}

	if (debugInfo.showCanvasPrefabInfo) {
		char canvasPrefabInfoText[64];
		sprintf(canvasPrefabInfoText, "Canvas prefab recs count: %lu",
				canvasPrefab.len);
		DrawText(canvasPrefabInfoText, 50, 110, 10, RAYWHITE);
		for (size_t i = 0; i < canvasPrefab.len; ++i) {
			DrawRectangleLinesEx(canvasPrefab.recs[i], 0.5, RED);
		}
	}
}
