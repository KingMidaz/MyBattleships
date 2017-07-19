#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidjson/document.h"

#include "Utility.h"
#include "Ships.h"
#include "Structs.h"

class MyBot {
public:
	static bool HitHandler(const string working_directory, vector<point> valid_points, vector<point> hits, point* out, vector<ship> liveships, int min, int max);
	static string GetMaxSpecialScorePos(int q, int p, int u, int v, vector<point> valid_points);
	static bool SpecialHandler(rapidjson::Document& state, string* outstr, int energy, int energyperround, vector<point> valid_points, const string working_directory);
	static void fire_shot(const string working_directory, rapidjson::Document& state);
	static void MyBot::place_ships(const string working_directory, const int BOARD_SIZE, rapidjson::Document& state);
};
// TODO: reference additional headers your program requires here
