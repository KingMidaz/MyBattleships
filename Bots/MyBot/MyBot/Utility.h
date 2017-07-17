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

#include "Ships.h"
#include "Structs.h"

class Utility {

public:
	static bool IsInArray(string* ar, string st, const string working_directory);
	static bool IsIn(vector<point> vectorar, point shot);
	static rapidjson::Document parse_state(const string working_directory, const string state_filename);
};