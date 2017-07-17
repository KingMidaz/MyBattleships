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

using namespace std;

class ship {

public:
	string type;
	int length;
	bool destroyed;
};

class point {

public:
	int x;
	int y;
};

class pointspecialscore {

public:
	point pt;
	int score;
};
