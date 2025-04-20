#include <optional>
#ifndef GAME_INPUT_HPP_
#define GAME_INPUT_HPP_ 1
#include <deque>

namespace fps::game_input {
typedef int KeyCode;

typedef enum GameInputType {
	MOVING_FORWARD,
	MOVING_RIGHT,
	MOVING_BACKWARD,
	MOVING_LEFT,
} GameInputType;

class GameInput {
   public:
	GameInputType type;
	float         time;

	GameInput(GameInputType type, float time = 0.0f) : type(type), time(time) {}
};

class InputQueue {
   private:
	std::deque<GameInput> _queue;

   public:
	InputQueue() : _queue(std::deque<GameInput>()) {};
	~InputQueue() = default;

	bool empty() const noexcept {
		return _queue.empty();
	}

	size_t length() const noexcept {
		return _queue.size();
	}

	void pop() {
		_queue.pop_front();
	}

	void push(const GameInput &input) {
		_queue.push_back(input);
	}

	GameInput &emplace(GameInputType type, float time) {
		return _queue.emplace_back(type, time);
	}

	std::optional<GameInput> findByType(GameInputType type) {
		for (auto input : _queue) {
			if (input.type == type) return std::optional(input);
		}
		return std::nullopt;
	}
};
}  // namespace fps::game_input

#endif
