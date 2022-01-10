#include "node.h"
#include "encode.h"
#include "catch.h"

#include <iostream>

using namespace keson;

TEST_CASE("Node")
{
	Node person;

	person["name"] = "Bengan";
	person["age"] = 23;
	
	Node hobbies;
	hobbies.push_back("cars");
	hobbies.push_back("babes");
	person["hobbies"] = hobbies;

	Node& friends = person["friends"];
	Node& pal = friends.push_back();
	pal["name"] = "The Sten-Ake";
	pal["age"] = 56;

	friends[1]["name"] = "Sara";
	friends[1]["age"] = 2.75;

	std::string encoded = encode(person, Flag_PREFER_SINGLE_QUOTES | Flag_QUOTE_KEYS | Flag_QUOTE_STRING_VALUES);
	CHECK(encoded == "{'name':'Bengan','age':'23','hobbies':['cars','babes'],'friends':[{'name':'The Sten-Ake','age':'56'},{'name':'Sara','age':'2.75'}]}");
}
