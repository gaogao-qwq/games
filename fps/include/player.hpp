#ifndef PLAYER_HPP_
#define PLAYER_HPP_ 1
#include <direction.hpp>
#include <raylib-cpp.hpp>

namespace fps {
class Player {
   public:
	fps::direction::Direction facing;
	raylib::Vector3           velocity;
	float                     angle_deg;
	raylib::Camera3D          camera;

	Player(raylib::Vector3           position  = raylib::Vector3(0.0f, 0.0f, 0.0f),
	       float                     fov       = 60.0f,
	       fps::direction::Direction facing    = fps::direction::Direction::NORTH,
	       raylib::Vector3           velocity  = Vector3{0.0f, 0.0f, 0.0f},
	       float                     angle_deg = 0.0f)
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
}  // namespace fps::player

#endif
