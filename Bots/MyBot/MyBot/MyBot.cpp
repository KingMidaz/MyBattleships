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

string* GetLiveShips(rapidjson::Document& state)
{
	const auto& ships = state["OpponentMap"]["Ships"];
	string* live = new string[5];
	for (auto it = ships.Begin(); it != ships.End(); it++) {
		const auto& ship = (*it);
		if (!ship["Destroyed"].GetBool()) {
			live->push_back(*ship["ShipType"].GetString());
		}
	}

	return live;
}

bool IsInArray(string* ar, string st)
{
	for (int i = 0; i < ar->length(); i++)
	{
		if (ar[i].compare(st) == 0)
		{
			return true;
		}
	}
	return false;
}

int MaxShipLength(string* live)
{
	if (IsInArray(live, "Carrier"))
	{
		return 5;
	}
	else if (IsInArray(live, "Battleship"))
	{
		return 4;
	}
	else if (IsInArray(live, "Submarine") || IsInArray(live, "Cruiser"))
	{
		return 3;
	}
	else
	{
		return 2;
	}
}

int MinShipLength(string* live)
{
	if (IsInArray(live, "Destroyer"))
	{
		return 2;
	}
	else if (IsInArray(live, "Submarine") || IsInArray(live, "Cruiser"))
	{
		return 3;
	}
	else if (IsInArray(live, "Battelship"))
	{
		return 4;
	}
	else
	{
		return 5;
	}
}

bool IsIn(vector<point> valid_points, point shot)
{
	for (int i = 0; i < valid_points.size(); i++) {
		if (valid_points[i].x == shot.x && valid_points[i].y == shot.y)
		{
			return true;
		}
	}

	return false;
}

void HitHandler(rapidjson::Document& state, vector<point> hits)
{

}

void fire_shot(const string working_directory, rapidjson::Document& state) {
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
			hits.push_back(h);
		}
	}

	//HitHandler(state, hits);

	ofstream ofs(working_directory + "/" + command_filename);
	ofstream debug(working_directory + "/debug.txt");

	debug << "Debug init" << endl;

	string* LiveShips = GetLiveShips(state);

	for (int i = 0; i < LiveShips->length(); i++)
	{
		debug << LiveShips[i];
	}
	
	int MaxLength = MaxShipLength(LiveShips);
	int MinLength = MinShipLength(LiveShips);

	debug << "Max" << MaxLength << endl;
	debug << "Min" << MinLength << endl;

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
