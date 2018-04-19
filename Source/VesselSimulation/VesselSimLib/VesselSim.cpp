#include "VesselSimLib/VesselSim.h"
#include "VesselSimulation.h"

vsl::VesselSim::VesselSim() : VesselManager() {
	init();
}

void vsl::VesselSim::init() {
	VesselManager::init();
}

void vsl::VesselSim::update(float _dt) {
	VesselManager::update(_dt);
}

void vsl::VesselSim::setGravity(float _gravity) {
	m_gravity = _gravity;
}

float vsl::VesselSim::getGravity() {
	return m_gravity;
}