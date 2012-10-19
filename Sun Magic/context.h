#pragma once

namespace SunMagic {

class SunMagicContext {

// All classes can use this context for information.
public:
	static enum STATE {
		NONE,
		ONE,
		TWO,
		THREE
	};
};

}