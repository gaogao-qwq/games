// clang-format off
#include <game_config.hpp>
#include <game_object.hpp>
#include <game_state.hpp>
#include <rcamera.h>
// clang-format on

namespace fps::game_object {
using namespace fps::game_config;
using namespace fps::game_state;

void Player::move_forward(float distance) {
	raylib::Vector3 forward = GetCameraForward(&camera);
	forward.y               = 0;
	forward                 = forward.Normalize() * distance;
	camera.position         = raylib::Vector3(camera.position) + forward;
	camera.target           = raylib::Vector3(camera.target) + forward;
	bounding.min            = raylib::Vector3(camera.position) + Vector3{-0.5f, -2.0f, -0.5f};
	bounding.max            = raylib::Vector3(camera.position) + Vector3{0.5f, 0.0f, 0.5f};
}

void Player::move_right(float distance) {
	raylib::Vector3 right = GetCameraRight(&camera);
	right.y               = 0;
	right                 = right.Normalize() * distance;
	camera.position       = raylib::Vector3(camera.position) + right;
	camera.target         = raylib::Vector3(camera.target) + right;
	bounding.min          = raylib::Vector3(camera.position) + Vector3{-0.5f, -2.0f, -0.5f};
	bounding.max          = raylib::Vector3(camera.position) + Vector3{0.5f, 0.0f, 0.5f};
}

void Player::move_up(float distance) {
	raylib::Vector3 up = GetCameraUp(&camera);
	up *= velocity.y * distance;
	camera.position = raylib::Vector3(camera.position) + up;
	camera.target   = raylib::Vector3(camera.target) + up;
	bounding.min    = raylib::Vector3(camera.position) + Vector3{-0.5f, -2.0f, -0.5f};
	bounding.max    = raylib::Vector3(camera.position) + Vector3{0.5f, 0.0f, 0.5f};
}

void Player::update() {
	float           dt               = GetFrameTime();
	raylib::Vector3 forward          = GetCameraForward(&camera);
	raylib::Vector3 right            = GetCameraRight(&camera);
	raylib::Vector3 up               = GetCameraUp(&camera);
	raylib::Vector3 pos              = camera.position;
	float           cameraYawAngle   = -(GetMouseDelta().x * player_yaw_acceleration) * DEG2RAD;
	float           cameraPitchAngle = -(GetMouseDelta().y * player_pitch_acceleration) * DEG2RAD;
	bool            collision        = false;
	bool            touchGround      = [this]() {
        auto checkBox = BoundingBox{raylib::Vector3(bounding.min) - Vector3{0.0f, 0.1f, 0.0f}, Vector3{bounding.max.x, bounding.min.y, bounding.max.z}};
        for (auto &obj : gameObjects) {
            if (obj->collide(checkBox)) return true;
        }
        return false;
	}();

	if (IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) {
		// decelerate forward-backward 1.5 times faster when pressing w and s at the same time
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
		// decelerate left-right 1.5 times faster when pressing a and d at the same time
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

	if (!touchGround) {
		velocity.y = std::max(velocity.y - gravity_acceleration * dt, -gravity_max_velocity);
	} else {
		if (IsKeyDown(KEY_SPACE)) {
			velocity.y = gravity_max_velocity;
		} else {
			velocity.y = 0;
		}
	}

	forward.y = 0;
	forward   = forward.Normalize() * -velocity.z * dt;
	right.y   = 0;
	right     = right.Normalize() * velocity.x * dt;
	up *= velocity.y * dt;

	for (auto &obj : gameObjects) {
		Vector3 nextmin = (pos + forward + right + up) + Vector3{-0.5f, -2.0f, -0.5f};
		Vector3 nextmax = (pos + forward + right + up) + Vector3{0.5f, 0.0f, 0.5f};
		if (obj->collide(BoundingBox{nextmin, nextmax})) {
			collision = true;
			break;
		}
	}

	// update camera movement
	if (!collision) {
		camera.position = raylib::Vector3(camera.position) + forward + right + up;
		camera.target   = raylib::Vector3(camera.target) + forward + right + up;
		bounding.min    = raylib::Vector3(camera.position) + Vector3{-0.5f, -2.0f, -0.5f};
		bounding.max    = raylib::Vector3(camera.position) + Vector3{0.5f, 0.0f, 0.5f};
	}

	// update camera rotation
	CameraPitch(&camera, cameraPitchAngle, true, false, false);
	CameraYaw(&camera, cameraYawAngle, false);
}
}  // namespace fps::game_object
