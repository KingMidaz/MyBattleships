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

#include "Structs.h"
#include "Ships.h"
#include "Utility.h"

class Killer {
public:
	static bool HitHandler(const string working_directory, vector<point> valid_points, vector<point> hits, point* out, vector<ship> liveships, int min, int max);
	static bool SpecialHandler(rapidjson::Document& state, string* outstr, int energy, int energyperround, pointprobability, const string working_directory);
	static void fire_shot(const string working_directory, rapidjson::Document& state);
	static void Killer::place_ships(const string working_directory, const int BOARD_SIZE, rapidjson::Document& state);
};

