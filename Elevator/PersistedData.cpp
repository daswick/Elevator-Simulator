#ifndef ELEVATOR_PERSISTEDDATA_CPP
#define ELEVATOR_PERSISTEDDATA_CPP

#include "PersistedData.hpp"

namespace elevator {

PersistedData::PersistedData(std::string elevatorId) {
	m_persistedFileName = "elevator_data_" + elevatorId + ".txt";
}

PersistedData::~PersistedData() {
	// Intentionally left empty
}

void PersistedData::start() {
	loadPersistedData(m_persistedFileName);
}

void PersistedData::stop() {
	savePersistedData(m_persistedFileName);	
}

int PersistedData::getPersistedFloor() {
	std::string value;
	
	if (getPersistedData(k_persistedFloorTag, value)) {
		return stoi(value);
	} else {
		return -1;
	}
	
	return 0;
}

void PersistedData::savePersistedFloor(int floorNumber) {
	persistData(k_persistedFloorTag, std::to_string(floorNumber));
}

} /* elevator */

#endif
