#pragma once

#include <cmath>
#include <iostream>
#include "src/Axis/axis.h"
#include "src/Cube/Move/move.h"

using std::cout, std::endl;

static float smoothstep(float x) {
	return x * x * (3.0f - 2.0f * x);
}

class Rotation {
	public:
		Move move;

		float currentAngle = 0.0f;
		float speed = 700.0f; 

		float t = 0.0f;
		float duration = 0.2f;

		Rotation() = default;

		Rotation(const Move& m) {
			setMove(m);
		}

		void setMove(const Move& m) {
			move = m;
			currentAngle = 0.0f;
		}

		float getTargetAngle() const {
			return 90.0f * move.amount * std::abs(move.dir);
		}

		void update(float dt) {
			t += dt / duration;
			if (t > 1.0f) t = 1.0f;

			float eased = smoothstep(t);

			currentAngle = -1 * eased * getTargetAngle() * move.dir;
		}

		bool finished() const {
			return std::abs(currentAngle) >= std::abs(getTargetAngle());
		}
};