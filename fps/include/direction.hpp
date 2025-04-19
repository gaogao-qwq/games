#ifndef DIRECTION_HPP_
#define DIRECTION_HPP_ 1

namespace fps::direction {

#define NORTH_VEC Vector2{0, -1}
#define EAST_VEC  Vector2{1, 0}
#define SOUTH_VEC Vector2{0, 1}
#define WEST_VEC  Vector2{-1, 0}

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

const char* direction_to_string(Direction direction);
Direction deg_to_direction(float deg);

}  // namespace fps

#endif
