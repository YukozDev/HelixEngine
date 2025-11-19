#pragma once
#include <HelixCore/Build/Build.h>

#ifdef _MSC_VER
	#define HX_BREAKPOINT() __debugbreak()
#else
	#define HX_BREAKPOINT() __builtin_trap()
#endif

#ifdef HX_DEBUG
	#define hxAssert(condition, ...) do { if(!condition) { HX_BREAKPOINT(); } } while(0)
#else
	#define hxAssert(condition, ...) ((void)0)
#endif


