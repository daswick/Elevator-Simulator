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

	// Attempt to create the directory the persistence uses
	// If the directory is created successfully or already exists, continue
	if (mkdir(k_persistPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
		if (errno != EEXIST) {
			SAFEPRINT("Persistence failed to create directory " + k_persistPath + " with errno " + std::to_string(errno));
			return;
		}
	}

	// To emulate "storage", open a file with all of the stored data
	// If the file does not exist, this does nothing
	std::lock_guard<std::mutex> guard(m_dataMutex);
	std::string filePath = k_persistPath + fileName;
	std::ifstream dataFile;
	dataFile.open(filePath);

	// The stored data is in the format "data_name=value", so parse it as such
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

	// If no values exist in the dirty map, no values were updated during runtime
	// Thus, there is no need to write to the files, so return
	{
		std::lock_guard<std::mutex> guard(m_dirtyMutex);

		if (m_dirtyMap.size() == 0) {
			return;
		}
		m_dirtyMap.clear();
	}

	// Write all of the data in its format to the output file
	// This will create the file if it does not exist
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
	// Rather than write to a file every time, just keep a dirty map
	// On shutdown, all of the dirty values are updated in their respective file
	std::lock_guard<std::mutex> guard(m_dataMutex);
	m_dataMap[key] = value;
	m_dirtyMap[key] = true;
}

void Persistence::deletePersistedData(std::string key) {
	// Attempt to find the key in the regular and dirty set
	// If it exists, erase it. Otherwise, do nothing
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
	
	// To ensure the program knows if the key loaded correctly,
	// Return true if it was found and update the reference parameter
	if (m_dataMap.find(key) == m_dataMap.end()) {
		return false;
	} else {
		value = m_dataMap[key];
		return true;
	}
}

} /* common */
