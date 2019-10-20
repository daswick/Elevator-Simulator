#include "Persistence.hpp"

#include "SafePrint.hpp"

#include <fstream>
#include <iterator>
#include <sys/stat.h>

namespace common {

void Persistence::loadPersistedData(std::string fileName) {
	if (m_isDataLoaded) {
		return;
	}

	if (mkdir(k_persistPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		if (errno != EEXIST) {
			SAFEPRINT("Persistence failed to create directory " + k_persistPath + " with errno " + std::to_string(errno));
			return;
		}
	}

	std::lock_guard<std::mutex> guard(m_dataMutex);
	std::string filePath = k_persistPath + fileName;
	std::ifstream dataFile;
	dataFile.open(filePath);

	std::string fileLine;
	while (std::getline(dataFile, fileLine)) {
		int delimPos = fileLine.find("=");
		std::string key = fileLine.substr(0, delimPos);
		std::string value = fileLine.substr(delimPos+1);
		m_dataMap[key] = value;
	}
	dataFile.close();

	m_isDataLoaded = true;
}

void Persistence::savePersistedData(std::string fileName) {
	if (!m_isDataLoaded) {
		return;
	}

	{
		std::lock_guard<std::mutex> guard(m_dirtyMutex);

		if (m_dirtyMap.size() == 0) {
			return;
		}
		m_dirtyMap.clear();
	}

	{
		std::lock_guard<std::mutex> guard(m_dataMutex);
		std::string filePath = k_persistPath + fileName;
		std::ofstream dataFile;
		dataFile.open(filePath);
		
		for (auto dataPair : m_dataMap) {
			std::string data = dataPair.first + "=" + dataPair.second;
			dataFile << data << "\n";
		}
		dataFile.close();
	}
}

void Persistence::persistData(std::string key, std::string value) {
	std::lock_guard<std::mutex> guard(m_dataMutex);
	m_dataMap[key] = value;
	m_dirtyMap[key] = true;
}

void Persistence::deletePersistedData(std::string key) {
	{
		std::lock_guard<std::mutex> guard(m_dataMutex);
		auto it = m_dataMap.find(key);
		if (it != m_dataMap.end()) {
			m_dataMap.erase(it);
		}
	}

	{
		std::lock_guard<std::mutex> guard(m_dirtyMutex);
		auto dirtyIt = m_dirtyMap.find(key);
		if (dirtyIt != m_dirtyMap.end()) {
			m_dirtyMap.erase(dirtyIt);
		}
	}
}

bool Persistence::getPersistedData(std::string key, std::string& value) {
	std::lock_guard<std::mutex> guard(m_dataMutex);
	
	if (m_dataMap.find(key) == m_dataMap.end()) {
		return false;
	} else {
		value = m_dataMap[key];
		return true;
	}
}

} /* common */
