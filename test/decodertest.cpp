#include "node.h"
#include "encode.h"
#include "decode.h"
#include "catch.h"

#include <iostream>

using namespace keson;

const char* KESON_TEXT = R"(
	{
		people: [
			{ 
				name: Bengan
				age: 23
				hobbies: [Cars, Babes]
			},,
			{ 
				name: "G\xe5ndalf"
				"age": 123
				hobbies: ["Wizardry", "The number 23" Hobbits]
			}
			{ 
				name: "\u7A40"
				"age": 6
				hobbies: [Anime, 'Quotation marks that look like "']
			},
		]
		animals: [
			Dog,
			"Cat"
			Bat
		]
	}
)";

TEST_CASE("Decode")
{
	auto result = decode(KESON_TEXT);
	
	REQUIRE(std::holds_alternative<Node>(result));

	std::cout << encode(std::get<Node>(result), Flags_RELAXED_QUOTES | Flag_PRETTY_PRINT | Flag_INDENT_WITH_SPACES);
	std::cout << encode(std::get<Node>(result), Flags_JSON_STYLE_QUOTES | Flag_PRETTY_PRINT | Flag_INDENT_WITH_SPACES);
}