#ifndef COMMON_PERSISTENCE_HPP
#define COMMON_PERSISTENCE_HPP

#include <map>
#include <mutex>
#include <string>

namespace common {

class Persistence {
public:
	Persistence() = default;
	~Persistence() = default;

protected:
	void loadPersistedData(std::string fileName);
	void savePersistedData(std::string fileName);

	void persistData(std::string key, std::string value);
	void deletePersistedData(std::string key);
	bool getPersistedData(std::string key, std::string& value);

private:
	const std::string k_persistPath = "data/";

	std::mutex m_dataMutex;
	std::mutex m_dirtyMutex;

	std::map<std::string, std::string> m_dataMap;
	std::map<std::string, bool> m_dirtyMap;

	bool m_isDataLoaded = false;
};

} /* common */

#endif /* COMMON_PERSISTENCE_HPP */
