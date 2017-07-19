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

#include "Utility.h"
#include "Structs.h"

class Ships {

public:
	static vector<ship> Ships::GetShipState(rapidjson::Document& state);
	static vector<ship> GetPlaceShipState(rapidjson::Document& state);
};