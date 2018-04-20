#pragma once

#include "CoreMinimal.h"

namespace vsl {
	class VESSELSIMULATION_API Stepper {
	public:
		Stepper();
		void init(float _init, float _speed);

		void step(float _dt);

		float get();
		void setSpeed(float _speed);
		float getRequested();
		void setRequested(float _requested);

		void reset(float _value);

	private:
		float m_current;
		float m_vel;
		float m_requested;
	};
}
