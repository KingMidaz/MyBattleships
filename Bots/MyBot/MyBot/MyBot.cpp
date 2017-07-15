#include "MyBot.h"
#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "rapidjson/document.h"

using namespace std;

constexpr char state_filename[] = "state.json";
constexpr char place_filename[] = "place.txt";
constexpr char command_filename[] = "command.txt";

struct point {
	int x;
	int y;
};

struct ship {
	string type;
	int length;
	bool destroyed;
};


rapidjson::Document parse_state(const string working_directory) {
	// Long-winded parsing because Nuget version of
	// rapidjson (1.0.2) requires a C-style string

	ifstream ifs(working_directory + "/" + state_filename);
	stringstream buffer;
	buffer << ifs.rdbuf();
	auto buffer_string = buffer.str();
	rapidjson::Document json_doc;
	json_doc.Parse(buffer_string.c_str());
	return json_doc;
}

ship* GetShipState(rapidjson::Document& state)
{
	int i = 0;

	const auto& ships = state["OpponentMap"]["Ships"];
	ship* live = new ship[5];
	for (auto it = ships.Begin(); it != ships.End(); it++) {
		const auto& ship = (*it);
		live[i].type = string(ship["ShipType"].GetString());
		live[i].destroyed = ship["Destroyed"].GetBool();
		
		if (live[i].type == "Carrier")
		{
			live[i].length = 5;
		}
		else if (live[i].type == "Battleship")
		{
			live[i].length = 4;
		}
		else if (live[i].type == "Submarine" || live[i].type == "Cruiser")
		{
			live[i].length = 3;
		}
		else if (live[i].type == "Destroyer")
		{
			live[i].length = 2;
		}

		i++;
	}

	return live;
}

bool IsInArray(string* ar, string st, const string working_directory)
{
	ofstream isinar(working_directory + "/isinar.txt", std::ofstream::out | std::ofstream::app);

	for (int i = 0; i < ar->length() - 1; i++)
	{
		isinar << st << " " << i << " " << ar[i].compare(st) << endl;

		if (ar[i].compare(st) == 0)
		{
			return true;
		}
	}
	isinar << st << " false" << endl;
	return false;
}

bool IsIn(vector<point> vectorar, point shot)
{
	for (int i = 0; i < vectorar.size(); i++) {
		if (vectorar[i].x == shot.x && vectorar[i].y == shot.y)
		{
			return true;
		}
	}

	return false;
}

bool HitHandler(const string working_directory, vector<point> valid_points, vector<point> hits, point* out, ship* liveships, int min, int max)
{
	ofstream hitdebug(working_directory + "/hitdebug.txt");
	hitdebug << "Hitdebug init" << endl;

	for (int i = 0; i < hits.size(); i++)
	{
		hitdebug << "1" << endl;
		if (IsIn(hits, { hits[i].x, hits[i].y + 1 }))
		{
			hitdebug << "2" << endl;
			for (int k = 1; k <= max-2; k++)
			{
				hitdebug << "3" << endl;
				if (IsIn(valid_points, { hits[i].x, hits[i].y - k }))
				{
					hitdebug << "4" << endl;
					*out = { hits[i].x, hits[i].y - k };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
				else if (IsIn(valid_points, { hits[i].x, hits[i].y + 1 + k }))
				{
					hitdebug << "5" << endl;
					*out = { hits[i].x, hits[i].y + 1 + k };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
			}
		}
		if (IsIn(hits, { hits[i].x, hits[i].y - 1 }))
		{
			hitdebug << "6" << endl;
			for (int k = 1; k <= max - 2; k++)
			{
				hitdebug << "7" << endl;
				if (IsIn(valid_points, { hits[i].x, hits[i].y + k }))
				{
					hitdebug << "8" << endl;
					*out = { hits[i].x, hits[i].y + k };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
				else if (IsIn(valid_points, { hits[i].x, hits[i].y - 1 - k }))
				{
					hitdebug << "9" << endl;
					*out = { hits[i].x, hits[i].y - 1 - k };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
			}
		}
		if (IsIn(hits, { hits[i].x + 1, hits[i].y }))
		{
			hitdebug << "10" << endl;
			for (int k = 1; k <= max - 2; k++)
			{
				hitdebug << "11" << endl;
				if (IsIn(valid_points, { hits[i].x - k, hits[i].y }))
				{
					hitdebug << "12" << endl;
					*out = { hits[i].x - k, hits[i].y };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
				else if (IsIn(valid_points, { hits[i].x + 1 + k, hits[i].y }))
				{
					hitdebug << "13" << endl;
					*out = { hits[i].x + 1 + k, hits[i].y };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
			}
		}
		if (IsIn(hits, { hits[i].x - 1, hits[i].y }))
		{
			hitdebug << "14" << endl;
			for (int k = 1; k <= max - 2; k++)
			{
				hitdebug << "15" << endl;
				if (IsIn(valid_points, { hits[i].x + k, hits[i].y }))
				{
					hitdebug << "16" << endl;
					*out = { hits[i].x + k, hits[i].y };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
				else if (IsIn(valid_points, { hits[i].x - 1 - k, hits[i].y }))
				{
					hitdebug << "17" << endl;
					*out = { hits[i].x - 1 - k, hits[i].y };
					hitdebug << out->x << " " << out->y << endl;
					return true;
				}
			}
		}
	}

	for (int i = 0; i < hits.size(); i++)
	{
		if (IsIn(valid_points, { hits[i].x + 1, hits[i].y }))
		{
			hitdebug << "18" << endl;
			*out = { hits[i].x + 1, hits[i].y };
			hitdebug << out->x << " " << out->y << endl;
			return true;
		}
		if (IsIn(valid_points, { hits[i].x - 1, hits[i].y }))
		{
			hitdebug << "19" << endl;
			*out = { hits[i].x - 1, hits[i].y };
			hitdebug << out->x << " " << out->y << endl;
			return true;
		}
		if (IsIn(valid_points, { hits[i].x, hits[i].y + 1}))
		{
			hitdebug << "20" << endl;
			*out = { hits[i].x, hits[i].y + 1};
			hitdebug << out->x << " " << out->y << endl;
			return true;
		}
		if (IsIn(valid_points, { hits[i].x, hits[i].y - 1}))
		{
			hitdebug << "21" << endl;
			*out = { hits[i].x, hits[i].y - 1};
			hitdebug << out->x << " " << out->y << endl;
			return true;
		}
	}

	return false;
}

void fire_shot(const string working_directory, rapidjson::Document& state) {
	
	ofstream ofs(working_directory + "/" + command_filename);
	ofstream debug(working_directory + "/debug.txt");
	
	debug << "Debug init" << endl;
	
	// Get cells that haven't already been shot at
	const auto& cells = state["OpponentMap"]["Cells"];
	vector<point> valid_points;
	for (auto it = cells.Begin(); it != cells.End(); it++) {
		const auto& cell = (*it);
		if (!cell["Damaged"].GetBool() && !cell["Missed"].GetBool()) {
			point p{ cell["X"].GetInt(), cell["Y"].GetInt() };
			valid_points.push_back(p);
		}
	}
	vector<point> hits;
	for (auto it = cells.Begin(); it != cells.End(); it++) {
		const auto& cell = (*it);
		if (cell["Damaged"].GetBool()) {
			point h{ cell["X"].GetInt(), cell["Y"].GetInt() };
			debug << h.x << " " << h.y << endl;
			hits.push_back(h);
		}
	}
	
	ship* LiveShips = GetShipState(state);

	int MaxLength = 0;
	int MinLength = 5;

	for (int i = 0; i < 5; i++)
	{
		if (MaxLength < LiveShips[i].length && !LiveShips[i].destroyed)
		{
			MaxLength = LiveShips[i].length;
		}
		if (MinLength > LiveShips[i].length && !LiveShips[i].destroyed)
		{
			MinLength = LiveShips[i].length;
		}
		if (LiveShips[i].destroyed) { debug << LiveShips[i].type << endl; }
	}
	
	debug << "Max " << MaxLength << endl;
	debug << "Min " << MinLength << endl;
	
	point out;
	if (HitHandler(working_directory, valid_points, hits, &out, LiveShips, MinLength, MaxLength))
	{
		debug << "Hithandler true" << endl;
		debug << "Out true" << out.x << " " << out.y << endl;
		ofs << "1," << out.x << "," << out.y << "\n";
		return;
	}

	for (auto pos = 0; pos < state["MapDimension"].GetInt(); pos++)
	{
		point shot1 = { pos, pos };
		point shot2 = { state["MapDimension"].GetInt() - pos, pos };

		if (IsIn(valid_points, shot1))
		{
			ofs << "1," << shot1.x << "," << shot1.y << "\n";
			debug << "Route1\n";
			return;
		}
		else if (IsIn(valid_points, shot2))
		{
			ofs << "1," << shot2.x << "," << shot2.y << "\n";
			debug << "Route2\n";
			return;
		}
	}

	int pos = 0;
	int it = 0;

	while (++it*MinLength < state["MapDimension"].GetInt()*0.5) {
		for (pos = 0; pos < state["MapDimension"].GetInt(); pos++)
		{
			point shot1 = { pos + it*MinLength, pos };
			point shot2 = { pos, pos + it*MinLength };
			point shot3 = { state["MapDimension"].GetInt() - pos - it*MinLength, pos };
			point shot4 = { state["MapDimension"].GetInt() - pos, pos + it*MinLength };

			if (IsIn(valid_points, shot1))
			{
				ofs << "1," << shot1.x << "," << shot1.y << "\n";
				debug << "Route3\n";
				return;
			}
			else if (IsIn(valid_points, shot2))
			{
				ofs << "1," << shot2.x << "," << shot2.y << "\n";
				debug << "Route4\n";
				return;
			}
			else if (IsIn(valid_points, shot3))
			{
				ofs << "1," << shot3.x << "," << shot3.y << "\n";
				debug << "Route5\n";
				return;
			}
			else if (IsIn(valid_points, shot4))
			{
				ofs << "1," << shot4.x << "," << shot4.y << "\n";
				debug << "Route6\n";
				return;
			}
		}
	}

	//Select among them randomly
	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> cell_dist(0, valid_points.size() - 1);
	auto shot = valid_points[cell_dist(rng)];
	debug << "Random shot taken" << endl;
	ofs << "1," << shot.x << "," << shot.y << "\n";
}


void place_ships(const string working_directory) {
	ofstream ofs(working_directory + "/" + place_filename);
	ofs << "Carrier 1 1 East\n";
	ofs << "Battleship 7 7 South\n";
	ofs << "Cruiser 9 0 North\n";
	ofs << "Submarine 4 4 West\n";
	ofs << "Destroyer 0 9 East\n";
}


int main(int argc, char** argv)
{
	if (argc != 3) {
		cout << "Usage: SampleBot.exe PlayerKey WorkingDirectory" << endl;
		return 1;
	}
	string working_directory(argv[2]);
	auto state = parse_state(working_directory);

	if (state["Phase"].GetInt() == 1) {
		place_ships(working_directory);
	}
	else {
		fire_shot(working_directory, state);
	}

	return 0;
}
