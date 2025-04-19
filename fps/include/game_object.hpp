#ifndef GAME_OBJECT_HPP_
#define GAME_OBJECT_HPP_ 1
#include <raylib-cpp.hpp>

namespace fps::game_object {
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
}  // namespace fps::game_object

#endif
