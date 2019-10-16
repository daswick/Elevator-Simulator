#ifndef COMMON_COMPONENTBASE_HPP
#define COMMON_COMPONENTBASE_HPP

namespace common {

class ComponentBase {
public:
	ComponentBase() = default;
	~ComponentBase() = default;

	virtual void start() = 0;
	virtual void stop() = 0;

	inline bool hasStarted() { return m_isStarted; }

protected:
	bool m_isStarted;
};

} /* common */

#endif /* COMMON_COMPONENTBASE_HPP */
