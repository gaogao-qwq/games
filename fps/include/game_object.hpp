#ifndef GAME_OBJECT_HPP_
#define GAME_OBJECT_HPP_ 1
#include <direction.hpp>
#include <raylib-cpp.hpp>

namespace fps::game_object {
using namespace fps::direction;

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
}  // namespace fps::game_object

#endif
