#include "Ships.h"

using namespace std;

vector<ship> Ships::GetShipState(rapidjson::Document& state)
{
	int i = 0;

	const auto& ships = state["OpponentMap"]["Ships"];
	vector<ship> live;
	for (auto it = ships.Begin(); it != ships.End(); it++) {
		const auto& ship = (*it);
		live.push_back({ string(ship["ShipType"].GetString()), 0, ship["Destroyed"].GetBool() });

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

vector<ship> Ships::GetPlaceShipState(rapidjson::Document& state)
{
	int i = 0;

	const auto& ships = state["PlayerMap"]["Owner"]["Ships"];
	vector<ship> live;
	for (auto it = ships.Begin(); it != ships.End(); it++) {
		const auto& ship = (*it);
		live.push_back({ string(ship["ShipType"].GetString()), 0, ship["Destroyed"].GetBool() });

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
