#ifndef ELEVATOR_PERSISTEDDATA_HPP
#define ELEVATOR_PERSISTEDDATA_HPP

#include "../Common/Persistence.hpp"

#include <string>

namespace elevator {

class PersistedData : public common::Persistence {
public:
	PersistedData(std::string elevatorId);
	~PersistedData() = default;

	void start();
	void stop();

	int getPersistedFloor();
	void savePersistedFloor(int floorNumber);

private:
	std::string m_persistedFileName;
	const std::string k_persistedFloorTag = "floor_number";
};

} /* elevator */

#endif /* ELEVATOR_PERSISTEDDATA_HPP */
