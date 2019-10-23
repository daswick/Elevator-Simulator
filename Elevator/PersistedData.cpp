#include "PersistedData.hpp"

namespace elevator {

PersistedData::PersistedData(std::string elevatorId) {
	m_persistedFileName = "elevator_data_" + elevatorId + ".txt";
}

void PersistedData::start() {
	// Load any persisted data to map
	loadPersistedData(m_persistedFileName);
}

void PersistedData::stop() {
	// Save any changes
	savePersistedData(m_persistedFileName);	
}

int PersistedData::getPersistedFloor() {
	std::string value;
	
	// If the value was persisted, return it. Otherwise, return -1
	if (getPersistedData(k_persistedFloorTag, value)) {
		return stoi(value);
	} else {
		return -1;
	}
	
	return 0;
}

void PersistedData::savePersistedFloor(int floorNumber) {
	// Save the current floor to the persisted data map
	persistData(k_persistedFloorTag, std::to_string(floorNumber));
}

} /* elevator */
