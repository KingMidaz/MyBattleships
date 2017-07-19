#pragma once

#include <stdio.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <unordered_set>
#include "rapidjson/document.h"

#include "Ships.h"
#include "Structs.h"

class Utility {

public:
	static bool IsInArray(string*, string, const string);
	static bool IsIn(vector<point>, point);
	static rapidjson::Document parse_state(const string, const string);
	static int valid_placement(const string, const int, ship, int, point, unordered_set<point, pair_hash>);
	static pointprobability probability_score(const string, const int, point, unordered_set<point, pair_hash>, vector<ship>);
};