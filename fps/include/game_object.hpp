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
	virtual ~GameObject()                                        = default;
	virtual void draw() const                                    = 0;
	virtual void draw_bounding() const                           = 0;
	virtual bool collide(const raylib::BoundingBox& other) const = 0;
};

class Cube : public GameObject {
   public:
	float         width;
	float         height;
	float         length;
	raylib::Color color;

	Cube(raylib::Vector3 position, float width, float height, float length, raylib::Color color)
		: GameObject(position), width(width), height(height), length(length), color(color) {};

	void draw() const override {
		DrawCube(position, width, height, length, color);
	}

	void draw_bounding() const override {}

	bool collide(const raylib::BoundingBox& other) const override { return false; }
};

class Plane : public GameObject {
   public:
	float         width;
	float         height;
	raylib::Color color;

	Plane(raylib::Vector3 position, float width, float height, raylib::Color color)
		: GameObject(position), width(width), height(height), color(color) {};

	void draw() const override {
		DrawPlane(position, Vector2{width, height}, color);
	}

	void draw_bounding() const override {}

	bool collide(const raylib::BoundingBox& other) const override { return false; }
};

class Grid : public GameObject {
   public:
	int   slices;
	float spacing;

	Grid(int slices, float spacing)
		: GameObject(Vector3{0.0f, 0.0f, 0.0f}), slices(slices), spacing(spacing) {};

	void draw() const override {
		DrawGrid(slices, spacing);
	}

	void draw_bounding() const override {}

	bool collide(const raylib::BoundingBox& other) const override { return false; }
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

	void draw() const override {
		DrawCube(position + Vector3{0.5f, 0.5f, 0.5f}, 1.0f, 1.0f, 1.0f, color);
	}

	void draw_bounding() const override {
		bounding.Draw(RED);
	}

	bool collide(const raylib::BoundingBox& other) const override {
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

	float facing_angle() {
		Vector2 facingVec2 = raylib::Vector2(camera.target.x, camera.target.z) - raylib::Vector2(camera.position.x, camera.position.z);
		return Vector2Angle(NORTH_VEC, facingVec2) * RAD2DEG;
	}

	Direction facing_direction() {
		return deg_to_direction(facing_angle());
	}

	void draw_bounding_box() {
		bounding.Draw(RED);
	}

	void draw_target() {
		DrawCube(camera.target, 0.5f, 0.5f, 0.5f, BLUE);
	}

	void move_forward(float distance);
	void move_right(float distance);
	void move_up(float distance);
	void update();
};
}  // namespace fps::game_object

#endif
