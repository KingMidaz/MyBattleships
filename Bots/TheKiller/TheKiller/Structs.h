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

struct pair_hash {
	template <class T1, class T2>
	std::size_t operator () (const std::pair<T1, T2> &p) const {
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);

		return h1 ^ h2;
	}
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
