#ifndef COMMON_COMPONENTBASE_HPP
#define COMMON_COMPONENTBASE_HPP

namespace common {

class ComponentBase {
public:
	ComponentBase() = default;
	~ComponentBase() = default;

	virtual void start() = 0;
	virtual void stop() = 0;
};

} /* common */

#endif

