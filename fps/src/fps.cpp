#include <deque>
#include <memory>
#include <raylib-cpp.hpp>
#include <vector>

namespace fps {

#define NORTH_VEC Vector2{0, -1}
#define EAST_VEC  Vector2{1, 0}
#define SOUTH_VEC Vector2{0, 1}
#define WEST_VEC  Vector2{-1, 0}

static float player_acceleration       = 32.0f;
static float player_deceleration       = 48.0f;
static float player_max_velocity       = 8.0f;
static float player_yaw_acceleration   = 12.0f;
static float player_pitch_acceleration = 8.0f;

typedef enum GameInputType {
	MOVING_FORWARD,
	MOVING_RIGHT,
	MOVING_BACKWARD,
	MOVING_LEFT,
} GameInputType;

class GameInput {
   public:
	GameInputType type;
};

typedef enum {
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST,
} Direction;

const char* direction_to_string(Direction direction) {
	switch (direction) {
		case Direction::NORTH:
			return "N";
		case Direction::NORTH_EAST:
			return "NE";
		case Direction::EAST:
			return "E";
		case Direction::SOUTH_EAST:
			return "SE";
		case Direction::SOUTH:
			return "S";
		case Direction::SOUTH_WEST:
			return "SW";
		case Direction::WEST:
			return "W";
		case Direction::NORTH_WEST:
			return "NW";
	}
	return "UNKNOWN";
}

Direction deg_to_direction(float deg) {
	if (deg < 0) deg = 360 + deg;
	if (deg >= 337.5f && deg < 22.5f) return Direction::NORTH;
	if (deg >= 22.5f && deg < 67.5f) return Direction::NORTH_EAST;
	if (deg >= 67.5f && deg < 112.5f) return Direction::EAST;
	if (deg >= 112.5f && deg < 157.5f) return Direction::SOUTH_EAST;
	if (deg >= 157.5f && deg < 202.5f) return Direction::SOUTH;
	if (deg >= 202.5f && deg < 247.5f) return Direction::SOUTH_WEST;
	if (deg >= 247.5f && deg < 292.5f) return Direction::WEST;
	if (deg >= 292.5 && deg < 337.5) return Direction::NORTH_WEST;
	return Direction::NORTH;
}

struct Resolution {
	int width;
	int height;

	Resolution(int width = 800, int height = 600)
		: width(width), height(height) {};
};

class GameConfig {
   public:
	Resolution resolution;
	double     fov;

	GameConfig(Resolution resolution = Resolution(800, 600), double fov = 60.0f)
		: resolution(resolution), fov(fov) {};
};

class Player {
   public:
	Direction        facing;
	raylib::Vector3  velocity;
	float            angle_deg;
	raylib::Camera3D camera;

	Player(raylib::Vector3 position  = raylib::Vector3(0.0f, 0.0f, 0.0f),
	       float           fov       = 60.0f,
	       Direction       facing    = Direction::NORTH,
	       raylib::Vector3 velocity  = Vector3{0.0f, 0.0f, 0.0f},
	       float           angle_deg = 0.0f)
		: facing(facing),
		  velocity(velocity),
		  angle_deg(angle_deg),
		  camera({
			  .position   = position + Vector3{0.0f, 2.0f, 0.0f},
			  .target     = position + Vector3{0.0f, 2.0f, -2.0f},
			  .up         = Vector3{0.0f, 1.0f, 0.0f},
			  .fovy       = fov,
			  .projection = CAMERA_PERSPECTIVE,
		  }) {};

	raylib::Vector3 position() const {
		return raylib::Vector3(camera.position);
	}

	void Draw() {
		DrawCube(position() - Vector3{0.0f, 1.0f, 0.0f}, 1.0f, 1.0f, 1.0f, RED);
	}

	void DrawCrosshair(int screenWidth, int screenHeight) {
		DrawRectangle(screenWidth / 2 - 10, screenHeight / 2 - 1, 20, 2, ColorAlpha(BLACK, 0.8f));
		DrawRectangle(screenWidth / 2 - 1, screenHeight / 2 - 10, 2, 20, ColorAlpha(BLACK, 0.8f));
	}

	void DrawTarget() {
		DrawCube(camera.target, 0.5f, 0.5f, 0.5f, BLUE);
	}
};

class GameObject {
   protected:
	raylib::Vector3 position;

   public:
	explicit GameObject(const raylib::Vector3& position) noexcept : position(position) {};
	virtual ~GameObject()     = default;
	virtual void Draw() const = 0;
};

class Cube : public GameObject {
   public:
	float         width;
	float         height;
	float         length;
	raylib::Color color;

	Cube(raylib::Vector3 position, float width, float height, float length, raylib::Color color)
		: GameObject(position), width(width), height(height), length(length), color(color) {};

	void Draw() const override {
		DrawCube(position, width, height, length, color);
	}
};

class Plane : public GameObject {
   public:
	float         width;
	float         height;
	raylib::Color color;

	Plane(raylib::Vector3 position, float width, float height, raylib::Color color)
		: GameObject(position), width(width), height(height), color(color) {};

	void Draw() const override {
		DrawPlane(position, Vector2{width, height}, color);
	}
};

class Grid : public GameObject {
   public:
	int   slices;
	float spacing;

	Grid(int slices, float spacing)
		: GameObject(Vector3{0.0f, 0.0f, 0.0f}), slices(slices), spacing(spacing) {};

	void Draw() const override {
		DrawGrid(slices, spacing);
	}
};

static auto startupTime = 0.0f;
static auto inputQueue  = std::deque<GameInputType>();
static auto gameConfig  = fps::GameConfig(Resolution(1280, 720));
static auto player      = fps::Player();
static auto editCamera  = raylib::Camera3D({
	 .position   = Vector3{0.0f, 20.0f, 0.0f},
	 .target     = Vector3{0.0f, 0.0f, 0.1f},
	 .up         = Vector3{0.0f, 1.0f, 0.0f},
	 .fovy       = 80.0f,
	 .projection = CAMERA_PERSPECTIVE,
});

static std::vector<std::unique_ptr<GameObject>> gameObjects = []() {
	std::vector<std::unique_ptr<GameObject>> vec;
	vec.emplace_back(std::make_unique<Grid>(10, 5.0f));
	// vec.emplace_back(std::make_unique<Plane>(Vector3{0.0f, 0.0f, 0.0f}, 32.0f, 32.0f, LIGHTGRAY));
	vec.emplace_back(std::make_unique<Cube>(Vector3{-16.0f, 1.0f, -16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{16.0f, 1.0f, -16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{-16.0f, 1.0f, 16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	vec.emplace_back(std::make_unique<Cube>(Vector3{16.0f, 1.0f, 16.0f}, 1.0f, 2.0f, 1.0f, VIOLET));
	return vec;
}();

static void handle_raw_input();
static void update();
static void update_player();

// TODO: implement me
static void handle_raw_input() {
}

static void update() {
	startupTime += GetFrameTime();
	// TODO: input queue
	update_player();
}

static void update_player() {
	float dt = GetFrameTime();

	if (!(IsKeyDown(KEY_W) && IsKeyDown(KEY_S))) {
		// forward
		if (IsKeyDown(KEY_W)) {
			if (player.velocity.z >= -player_max_velocity) {
				player.velocity.z = std::max(player.velocity.z - player_acceleration * dt, -player_max_velocity);
			}
		} else {
			if (player.velocity.z < 0.0f) {
				player.velocity.z = std::min(player.velocity.z + player_deceleration * dt, 0.0f);
			}
		}

		// backward
		if (IsKeyDown(KEY_S)) {
			if (player.velocity.z <= player_max_velocity) {
				player.velocity.z = std::min(player.velocity.z + player_acceleration * dt, player_max_velocity);
			}
		} else {
			if (player.velocity.z > 0.0f) {
				player.velocity.z = std::max(player.velocity.z - player_deceleration * dt, 0.0f);
			}
		}
	} else {
		player.velocity.z = 0.0f;
	}

	if (!(IsKeyDown(KEY_A) && IsKeyDown(KEY_D))) {
		// left
		if (IsKeyDown(KEY_A)) {
			if (player.velocity.x >= -player_max_velocity) {
				player.velocity.x = std::max(player.velocity.x - player_acceleration * dt, -player_max_velocity);
			}
		} else {
			if (player.velocity.x < 0.0f) {
				player.velocity.x = std::min(player.velocity.x + player_deceleration * dt, 0.0f);
			}
		}

		// right
		if (IsKeyDown(KEY_D)) {
			if (player.velocity.x <= player_max_velocity) {
				player.velocity.x = std::min(player.velocity.x + player_acceleration * dt, player_max_velocity);
			}
		} else {
			if (player.velocity.x > 0.0f) {
				player.velocity.x = std::max(player.velocity.x - player_deceleration * dt, 0.0f);
			}
		}
	} else {
		player.velocity.x = 0.0f;
	}

	Vector3 movement = {
		.x = (-player.velocity.z) * dt,  // forward-backward
		.y = player.velocity.x * dt,     // right-left
		.z = player.velocity.y * dt,     // up-down
	};

	// rotating camera
	Vector3 rotation = {
		.x = GetMouseDelta().x * dt * player_yaw_acceleration,    // yaw
		.y = GetMouseDelta().y * dt * player_pitch_acceleration,  // pitch
		.z = 0.0f,                                                // roll
	};
	float zoom = 0.0f;
	player.camera.Update(movement, rotation, zoom);

	Vector2 directionVector = raylib::Vector2(player.camera.target.x, player.camera.target.z) - raylib::Vector2(player.position().x, player.position().z);
	player.angle_deg        = Vector2Angle(NORTH_VEC, directionVector) * RAD2DEG;
	player.facing           = deg_to_direction(player.angle_deg);
}
}  // namespace fps

int main(void) {
	SetConfigFlags(FLAG_VSYNC_HINT);
	raylib::Window        window(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height, "fps");
	raylib::RenderTexture playerScreen(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height);
	raylib::RenderTexture editScreen(200, 100);
	raylib::Rectangle     playerScreenRect(0.0f, 0.0f, (float)fps::gameConfig.resolution.width, (float)-fps::gameConfig.resolution.height);
	raylib::Rectangle     editScreenRect(0, 0, 200.0f, -100.0f);

	// SetTargetFPS(60);
	DisableCursor();

	while (!window.ShouldClose()) {
		fps::update();

		// clang-format off
		playerScreen.BeginMode();
			ClearBackground(RAYWHITE);
			fps::player.camera.BeginMode();
				for (auto &obj : fps::gameObjects) obj->Draw();
				fps::player.Draw();
			fps::player.camera.EndMode();
			fps::player.DrawCrosshair(fps::gameConfig.resolution.width, fps::gameConfig.resolution.height);
		playerScreen.EndMode();

		editScreen.BeginMode();
			ClearBackground(RAYWHITE);
			DrawRectangleLinesEx(Rectangle{0.0f, 0.0f, 200.0f, 100.0f}, 1.0f, BLACK);
			fps::editCamera.BeginMode();
				for (auto &obj : fps::gameObjects) obj->Draw();
				fps::player.Draw();
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
				TextFormat("player facing degree: %f", fps::player.angle_deg),
				10, 60, 10, BLACK);
			raylib::DrawText(
				TextFormat("player facing: %s", fps::direction_to_string(fps::player.facing)),
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
