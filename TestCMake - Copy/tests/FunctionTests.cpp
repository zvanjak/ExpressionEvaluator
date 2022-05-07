//#define CATCH_CONFIG_MAIN

#include "../catch/catch.hpp"

#include "Function.h"

TEST_CASE("factorials are computed", "[factorial]") {
	REQUIRE(func(1,1) == 2);
}