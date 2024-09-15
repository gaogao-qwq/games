#ifndef SIM_H_
#define SIM_H_ 1

#include <stdlib.h>

#include "raylib.h"

// clang-format off
#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

#define TARGET_TICKRATE 64

#define PARTICLE_SIZE 2
#define CANVAS_SIZE   300

#define BORDER_COLOR (Color){255, 255, 255, 255}     // White
#define AIR_COLOR    (Color){0, 0, 0, 0}             // Transparent
#define SAND_COLOR   (Color){255, 255, 51, 255}      // Yellow
#define WATER_COLOR  (Color){0, 121, 241, 255}       // Blue
#define STONE_COLOR  (Color){128, 128, 128, 255}     // Grey
#define WOOD_COLOR   (Color){139, 69, 19, 255}       // Brown
//
// Particle properties
#define PARTICLE_INVISIBLE            (1 << 0)
#define PARTICLE_AFFECTED_BY_GRAVITY  (1 << 1)
#define PARTICLE_FLAMMABLE            (1 << 2)
#define PARTICLE_EXPLOSIVE            (1 << 3)

typedef enum {
	PARTICLE_BORDER,
	PARTICLE_AIR,
	PARTICLE_SAND,
	PARTICLE_WATER,
	PARTICLE_STONE,
	PARTICLE_WOOD,
} ParticleType;

typedef struct {
	bool showBrushSize;
	bool showBrushCursorPosition;
	bool showFrameTime;
	bool showFPS;
	bool showBrushInfo;
	bool showCanvasPrefabInfo;
} DebugInfo;

typedef struct {
	int x, y;
} IntVec2;

typedef struct {
	IntVec2 position;  // brush cursor location
	Color color;       // brush particle color
	size_t size;       // brush size
	size_t p_count;    // brush particle points count
	IntVec2 *points;   // brush particle points
	ParticleType type; // brush particle type
} BrushCursor;

typedef struct {
	ParticleType type;
	Color color;
	int flag;
	bool updated;
} Particle;
inline Particle GetParticleByType(ParticleType type);
inline void SwapParticle(Particle *a, Particle *b);
inline bool IsBorder(Particle particle) { return particle.type == PARTICLE_BORDER; }
inline bool IsAir(Particle particle) { return particle.type == PARTICLE_AIR; }
inline bool IsSand(Particle particle) { return particle.type == PARTICLE_SAND; }
inline bool IsWater(Particle particle) { return particle.type == PARTICLE_WATER; }
inline bool IsStone(Particle particle) { return particle.type == PARTICLE_STONE; }
inline bool IsWood(Particle particle) { return particle.type == PARTICLE_WOOD; }

#define BORDER (Particle){PARTICLE_BORDER, BORDER_COLOR, 0,                            false}
#define AIR    (Particle){PARTICLE_AIR,    AIR_COLOR,    PARTICLE_INVISIBLE,           false}
#define SAND   (Particle){PARTICLE_SAND,   SAND_COLOR,   PARTICLE_AFFECTED_BY_GRAVITY, false}
#define WATER  (Particle){PARTICLE_WATER,  WATER_COLOR,  PARTICLE_AFFECTED_BY_GRAVITY, false}
#define STONE  (Particle){PARTICLE_STONE,  STONE_COLOR,  0,                            false}
#define WOOD   (Particle){PARTICLE_WOOD,   WOOD_COLOR,   PARTICLE_FLAMMABLE,           false}

typedef struct {
	Rectangle *recs;
	Color *colors;
	size_t len;
} CanvasPrefab;

typedef struct {
	Particle **particles;
	size_t width, height;
} Canvas;

void MainLoop();

void UpdateGameTick();

void UpdateBrushCursor(BrushCursor *cursor);

void UpdateBrushOperation(BrushCursor *cursor);

void UpdateSand(Canvas *canvas, size_t r, size_t c);

void UpdateWater(Canvas *canvas, size_t r, size_t c);

void UpdateParticles(Canvas *canvas);

void UpdateCanvasPrefab(Canvas *canvas);

void DrawBrushCursor(BrushCursor cursor);

void BrushDraw(BrushCursor cursor, Canvas *canvas);

void InitCanvas(size_t width, size_t height);

void DrawCanvasPrefab(CanvasPrefab canvasPrefab);

void DrawDebugInfo(BrushCursor cursor);

#endif
