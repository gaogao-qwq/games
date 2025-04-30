#ifndef GAME_OBJECT_HPP_
#define GAME_OBJECT_HPP_ 1
#include <raylib.h>

#include <direction.hpp>
#include <game_config.hpp>
#include <raylib-cpp.hpp>

namespace fps::game_object {
using namespace fps::direction;
using namespace fps::game_config;

class GameObject {
   protected:
	raylib::Vector3 position;

   public:
	explicit GameObject(const raylib::Vector3& position) noexcept : position(position) {};
	virtual ~GameObject()             = default;
	virtual void Draw() const         = 0;
	virtual void DrawBounding() const = 0;
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

	void DrawBounding() const override {}
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

	void DrawBounding() const override {}
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

	void DrawBounding() const override {}
};

class Block : public GameObject {
   public:
	raylib::Color       color;
	raylib::BoundingBox bounding;

	Block(raylib::Vector3 position, raylib::Color color)
		: GameObject(Vector3{std::floor(position.x), std::floor(position.y), std::floor(position.z)}),
		  color(color),
		  bounding(BoundingBox{
			  .min = Vector3(position),
			  .max = Vector3(position + Vector3{1.0f, 1.0f, 1.0f}),
		  }) {}

	void Draw() const override {
		DrawCube(position + Vector3{0.5f, 0.5f, 0.5f}, 1.0f, 1.0f, 1.0f, color);
	}

	void DrawBounding() const override {
		bounding.Draw(RED);
	}

	bool Collide(const raylib::BoundingBox& other) {
		return bounding.CheckCollision(other);
	}
};

class Player {
   public:
	raylib::Vector3     velocity;
	raylib::BoundingBox bounding;
	raylib::Camera3D    camera;

	Player(raylib::Vector3     position = raylib::Vector3(0.0f, 0.0f, 0.0f),
	       float               fov      = 60.0f,
	       raylib::Vector3     velocity = Vector3{0.0f, 0.0f, 0.0f},
	       raylib::BoundingBox bounding = BoundingBox{Vector3{-0.5f, 0.0f, -0.5f}, Vector3{0.5f, 2.0f, 0.5f}})
		: velocity(velocity),
		  bounding(bounding),
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

	float FacingAngle() {
		Vector2 facingVec2 = raylib::Vector2(camera.target.x, camera.target.z) - raylib::Vector2(camera.position.x, camera.position.z);
		return Vector2Angle(NORTH_VEC, facingVec2) * RAD2DEG;
	}

	Direction FacingDirection() {
		return deg_to_direction(FacingAngle());
	}

	void Update() {
		float dt = GetFrameTime();

		if (IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) {
			if (velocity.z < 0.0f) {
				velocity.z = std::min(velocity.z + player_deceleration * 1.5f * dt, 0.0f);
			} else if (velocity.z > 0.0f) {
				velocity.z = std::max(velocity.z - player_deceleration * 1.5f * dt, 0.0f);
			}
		} else {
			// forward
			if (IsKeyDown(KEY_W)) {
				if (velocity.z >= -player_max_velocity) {
					velocity.z = std::max(velocity.z - player_acceleration * dt, -player_max_velocity);
				}
			} else {
				if (velocity.z < 0.0f) {
					velocity.z = std::min(velocity.z + player_deceleration * dt, 0.0f);
				}
			}

			// backward
			if (IsKeyDown(KEY_S)) {
				if (velocity.z <= player_max_velocity) {
					velocity.z = std::min(velocity.z + player_acceleration * dt, player_max_velocity);
				}
			} else {
				if (velocity.z > 0.0f) {
					velocity.z = std::max(velocity.z - player_deceleration * dt, 0.0f);
				}
			}
		}

		if (IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) {
			if (velocity.x < 0.0f) {
				velocity.x = std::min(velocity.x + player_deceleration * 1.5f * dt, 0.0f);
			} else if (velocity.x > 0.0f) {
				velocity.x = std::max(velocity.x - player_deceleration * 1.5f * dt, 0.0f);
			}
		} else {
			// left
			if (IsKeyDown(KEY_A)) {
				if (velocity.x >= -player_max_velocity) {
					velocity.x = std::max(velocity.x - player_acceleration * dt, -player_max_velocity);
				}
			} else {
				if (velocity.x < 0.0f) {
					velocity.x = std::min(velocity.x + player_deceleration * dt, 0.0f);
				}
			}

			// right
			if (IsKeyDown(KEY_D)) {
				if (velocity.x <= player_max_velocity) {
					velocity.x = std::min(velocity.x + player_acceleration * dt, player_max_velocity);
				}
			} else {
				if (velocity.x > 0.0f) {
					velocity.x = std::max(velocity.x - player_deceleration * dt, 0.0f);
				}
			}
		}

		Vector3 cameraMovement = {
			.x = -velocity.z * dt,  // forward-backward
			.y = velocity.x * dt,   // right-left
			.z = velocity.y * dt,   // up-down
		};
		Vector3 cameraRotation = {
			.x = GetMouseDelta().x * dt * player_yaw_acceleration,    // yaw
			.y = GetMouseDelta().y * dt * player_pitch_acceleration,  // pitch
			.z = 0.0f,                                                // roll
		};
		float zoom = 0.0f;

		camera.GetPosition();
		camera.Update(cameraMovement, cameraRotation, zoom);
		bounding.min = raylib::Vector3(camera.position) + Vector3{-0.5f, -2.0f, -0.5f};
		bounding.max = raylib::Vector3(camera.position) + Vector3{0.5f, 0.0f, 0.5f};
	}

	void DrawBoundingBox() {
		bounding.Draw(RED);
	}

	void DrawCrosshair(int screenWidth, int screenHeight) {
		DrawRectangle(screenWidth / 2 - 10, screenHeight / 2 - 1, 20, 2, ColorAlpha(BLACK, 0.8f));
		DrawRectangle(screenWidth / 2 - 1, screenHeight / 2 - 10, 2, 20, ColorAlpha(BLACK, 0.8f));
	}

	void DrawTarget() {
		DrawCube(camera.target, 0.5f, 0.5f, 0.5f, BLUE);
	}
};
}  // namespace fps::game_object

#endif
