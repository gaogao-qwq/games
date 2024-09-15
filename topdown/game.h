#ifndef GAME_DEF_H_
#define GAME_DEF_H_ 1

#include <raylib.h>
#include <rlgl.h>
#include <stdio.h>
#include <math.h>

typedef signed char            		i8;
typedef unsigned char          		u8;
typedef signed short int       		i16;
typedef unsigned short int     		u16;
typedef signed int             		i32;
typedef unsigned int           		u32;
typedef float                  		f32;
typedef signed long long int   		i64;
typedef unsigned long long int 		u64;
typedef double                 		f64;

#define PLAYER_ACCELERATION 20.f
#define PLAYER_DECELERATION 30.f
#define PLAYER_MAX_SPEED 300.f
#define CAMERA_ZOOM_INCREMENT 0.125f
#define CAMERA_MAX_ZOOM 4.f
#define CAMERA_MIN_ZOOM 1.f

#define IS_MOVING_UP (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
#define IS_MOVING_RIGHT (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
#define IS_MOVING_DOWN (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
#define IS_MOVING_LEFT (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
#define IS_MOVING ((bool) (IS_MOVING_UP || IS_MOVING_RIGHT || IS_MOVING_DOWN || IS_MOVING_LEFT))
#define IS_ON_TOP(ay, by) ((bool) ((f32)ay < (f32)by))
#define IS_ON_RIGHT(ax, bx) ((bool) ((f32)ax > (f32)bx))
#define IS_ON_BOTTOM(ay, by) ((bool) ((f32)ay > (f32)by))
#define IS_ON_LEFT(ax, bx) ((bool) ((f32)ax < (f32)bx))

typedef enum Direction {
	top = 0,
	right = 1,
	bottom = 2,
	left = 3,
} Direction;

typedef struct Velocity {
	f32 vt; 		// Top
	f32 vr; 		// Right
	f32 vb; 		// Bottom
	f32 vl; 		// Left
} Velocity;

typedef struct Player {
	Vector2 location;
	Rectangle rect;
	Rectangle hitbox;
	Velocity velocity;
} Player;

typedef struct EnvItem {
	Rectangle rect;
	bool blocking;
	bool moving;
	Color color;
} EnvItem;

typedef struct DebugInfo {
	bool showPlayerHitbox;
	bool showPlayerVelocity;
	bool showFPS;
} DebugInfo;

void MainLoop();

//------------------------------------------------------------------------------
// Update functions
//------------------------------------------------------------------------------
void UpdatePlayer(Player *player, EnvItem *envItems, i32 envItemsLength,
				  f32 delta);
void UpdateCamera2D(Camera2D *camera, Vector2 target, f32 wheel);
void UpdateDebugInfo();
void UpdateText(Player player, f32 fps);

//------------------------------------------------------------------------------
// Utility functions
//------------------------------------------------------------------------------
const char *GetBoolalpha(bool value); 		// Get String format bool
f32 GetHypotenuse(f32 a, f32 b);      		// Get hypotenuse by given legs

#endif
