#include "node.h"
#include "encode.h"
#include "decode.h"
#include "catch.h"

#include <iostream>

using namespace keson;

TEST_CASE("Preserves utf-8")
{
	Node v = "\xe7\xa9\x85";
	CHECK(encode(v) == "\"\xe7\xa9\x85\"");
}


TEST_CASE("Escapes stuff")
{
	Node v = "\\\n\t\f\r\v\"\'\x12";
	CHECK(encode(v) == "\"\\\\\\n\\t\\f\\r\\v\\\"'\\x12\""); // eugh
}
