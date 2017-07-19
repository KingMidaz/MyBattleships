#pragma once

#include <stdio.h>
#include <tchar.h>
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "rapidjson/document.h"

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

struct Hash {
	std::size_t operator () (const point &p) const {
		string temp = to_string(p.x) + to_string(p.y);
		return hash<string>()(temp);
	}
};

inline bool operator == (point const& lhs, point const& rhs){
		return (lhs.x == rhs.x) &&
			(lhs.y == rhs.y);
};

class pointspecialscore {

public:
	point pt;
	int score;
};

class pointprobability {

public:
	point pt;
	int score;

};
