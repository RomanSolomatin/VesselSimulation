#include "VesselSimLib/VesselBuilding.h"
#include "VesselSimulation.h"

vsl::IShip* vsl::VesselBuilding::orderVessel(std::string type) {
	return makeVessel(type);
}