#include <direction.hpp>

namespace fps::direction {
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
}  // namespace fps::direction
