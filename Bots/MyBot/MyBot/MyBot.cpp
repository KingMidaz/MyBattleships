#include "MyBot.h"

using namespace std;

constexpr char state_filename[] = "state.json";
constexpr char place_filename[] = "place.txt";
constexpr char command_filename[] = "command.txt";

bool MyBot::HitHandler(const string working_directory, vector<point> valid_points, vector<point> hits, point* out, vector<ship> liveships, int min, int max)
{
	//ofstream hitdebug(working_directory + "/hitdebug.txt");
	//hitdebug << "Hitdebug init" << endl;

	for (int i = 0; i < hits.size(); i++)
	{
		bool adj = false;
		bool miss1 = false;
		bool miss2 = false;

		//hitdebug << "1" << endl;
		for (int g = 1; g > -2; g -= 2)
		{
			if (Utility::IsIn(hits, { hits[i].x, hits[i].y + g }))
			{
				miss1 = false;
				miss2 = false;

				adj = true;
				//hitdebug << "2" << endl;
				for (int k = 1; k <= max - 2; k++)
				{
					//hitdebug << "3" << endl;
					if (Utility::IsIn(valid_points, { hits[i].x, hits[i].y - k }) && !miss1)
					{
						//hitdebug << "4" << endl;
						*out = { hits[i].x, hits[i].y - k };
						//hitdebug << out->x << " " << out->y << endl;
						return true;
					}
					else if (!Utility::IsIn(hits, { hits[i].x, hits[i].y - k }))
					{
						miss1 = true;
					}
					if (Utility::IsIn(valid_points, { hits[i].x, hits[i].y + 1 + k }) && !miss2)
					{
						//hitdebug << "5" << endl;
						*out = { hits[i].x, hits[i].y + 1 + k };
						//hitdebug << out->x << " " << out->y << endl;
						return true;
					}
					else if (!Utility::IsIn(hits, { hits[i].x, hits[i].y + 1 + k }))
					{
						miss2 = true;
					}
				}
			}
		}
		
		for (int g = 1; g > -2; g -= 2)
		{
			if (Utility::IsIn(hits, { hits[i].x + g, hits[i].y }))
			{
				miss1 = false;
				miss2 = false;

				adj = true;
				//hitdebug << "10" << endl;
				for (int k = 1; k <= max - 2; k++)
				{
					//hitdebug << "11" << endl;
					if (Utility::IsIn(valid_points, { hits[i].x - k, hits[i].y }) && !miss1)
					{
						//hitdebug << "12" << endl;
						*out = { hits[i].x - k, hits[i].y };
						//hitdebug << out->x << " " << out->y << endl;
						return true;
					}
					else if (!Utility::IsIn(hits, { hits[i].x - k, hits[i].y }))
					{
						miss1 = true;
					}
					if (Utility::IsIn(valid_points, { hits[i].x + 1 + k, hits[i].y }) && !miss2)
					{
						//hitdebug << "13" << endl;
						*out = { hits[i].x + 1 + k, hits[i].y };
						//hitdebug << out->x << " " << out->y << endl;
						return true;
					}
					else if (!Utility::IsIn(hits, { hits[i].x + 1 + k, hits[i].y }))
					{
						miss2 = true;
					}
				}
			}
		}

		for (int f = 0; f < 3; f += 2)
		{
			if (Utility::IsIn(valid_points, { hits[i].x - 1 + f, hits[i].y - 1}) || Utility::IsIn(valid_points, { hits[i].x - 1 + f, hits[i].y + 1}))
			{
				adj = false;
			}
		}

		for (int g = 1; g > -2; g -= 2)
		{
			if (Utility::IsIn(valid_points, { hits[i].x + g, hits[i].y }) && !adj)
			{
				//hitdebug << "18" << endl;
				*out = { hits[i].x + g, hits[i].y };
				//hitdebug << out->x << " " << out->y << endl;
				return true;
			}
		}

		for (int g = 1; g > -2; g -= 2)
		{
			if (Utility::IsIn(valid_points, { hits[i].x, hits[i].y + g }) && !adj)
			{
				//hitdebug << "20" << endl;
				*out = { hits[i].x, hits[i].y + g };
				//hitdebug << out->x << " " << out->y << endl;
				return true;
			}
		}
	}

	//hitdebug << "Hitdebug exiting" << endl;
	return false;
}

string MyBot::GetMaxSpecialScorePos(int q, int p, int u, int v, vector<point> valid_points)
{
	vector<pointspecialscore> score;

	for (int i = 0; i < valid_points.size(); i++)
	{
		score.push_back(pointspecialscore({ { valid_points[i].x, valid_points[i].y }, 0 }));
		for (int x = u; x < q; x+=p)
		{
			for (int y = v; y < q; y+=p)
			{
				if (Utility::IsIn(valid_points, { valid_points[i].x - 2 + x, valid_points[i].y - 2 + y }))
				{
					score[i].score++;
				}
			}
		}
	}

	pointspecialscore max = pointspecialscore{ { 0,0 }, 0 };
	for (int k = 0; k < valid_points.size(); k++)
	{
		if (max.score < score[k].score)
		{
			max = score[k];
		}
	}
	if (max.score > 0)
	{
		stringstream tmp;
		tmp << max.pt.x << "," << max.pt.y;
		return tmp.str();
	}
	
	return "";
}

bool MyBot::SpecialHandler(rapidjson::Document& state, string* outstr, int energy, int energyperround, vector<point> valid_points, const string working_directory)
{
	vector<ship> live = Ships::GetPlaceShipState(state);

	//ofstream specialdebug(working_directory + "/specialdebug.txt");
	//specialdebug << "Special debug init" << endl;

	for (int l = 0; l < 5; l++)
	{
		if (live[l].type.compare("Battleship") == 0 && !live[l].destroyed)
		{
			if (energy >= 12 * energyperround)
			{
				*outstr = "5," + GetMaxSpecialScorePos(3, 2, 0, 0, valid_points);
				return true;
			}
			return false;
		}
	}
	for (int l = 0; l < 5; l++)
	{
		if (live[l].type.compare("Submarine") == 0 && !live[l].destroyed)
		{
			if (energy >= 10 * energyperround)
			{
				*outstr = "7," + GetMaxSpecialScorePos(5, 1, 0, 0, valid_points);
				return true;
			}
			return false;
		}
	}
	for (int l = 0; l < 5; l++)
	{
		if (live[l].type.compare("Cruiser") == 0 && !live[l].destroyed)
		{
			if (energy >= 14 * energyperround)
			{
				*outstr = "6," + GetMaxSpecialScorePos(3, 1, 0, 0, valid_points);
				return true;
			}
			return false;
		}
	}
	for (int l = 0; l < 5; l++)
	{
		if (live[l].type.compare("Carrier") == 0 && !live[l].destroyed)
		{
			if (energy >= 10 * energyperround)
			{
				*outstr = "4," + GetMaxSpecialScorePos(3, 2, 0, 0, valid_points);
				return true;
			}
			return false;
		}
	}
	for (int l = 0; l < 5; l++)
	{
		if (live[l].type.compare("Destroyer") == 0 && !live[l].destroyed)
		{
			if (energy >= 8 * energyperround)
			{
				*outstr = "3," + GetMaxSpecialScorePos(4, 2, 2, 1, valid_points);
				return true;
			}
			return false;
		}
	}
	
	return false;
}

void MyBot::fire_shot(const string working_directory, rapidjson::Document& state) 
{
	
	ofstream ofs(working_directory + "/" + command_filename);
	//ofstream debug(working_directory + "/debug.txt");
	
	//debug << "Debug init" << endl;
	
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
			//debug << h.x << " " << h.y << endl;
			hits.push_back(h);
		}
	}
	
	vector<ship> LiveEnemyShips = Ships::GetShipState(state);

	const int BOARD_SIZE = state["MapDimension"].GetInt();
	int MaxLength = 0;
	int MinLength = 5;

	for (int i = 0; i < 5; i++)
	{
		if (MaxLength < LiveEnemyShips[i].length && !LiveEnemyShips[i].destroyed)
		{
			MaxLength = LiveEnemyShips[i].length;
		}
		if (MinLength > LiveEnemyShips[i].length && !LiveEnemyShips[i].destroyed)
		{
			MinLength = LiveEnemyShips[i].length;
		}
		//if (!LiveEnemyShips[i].destroyed) { debug << LiveEnemyShips[i].type << endl; }
	}
	
	//debug << "Max " << MaxLength << endl;
	//debug << "Min " << MinLength << endl;
	
	point out;
	if (HitHandler(working_directory, valid_points, hits, &out, LiveEnemyShips, MinLength, MaxLength))
	{
		//debug << "Hithandler true" << endl;
		//debug << "Out true" << out.x << " " << out.y << endl;
		ofs << "1," << out.x << "," << out.y << "\n";
		return;
	
	}
	int energyperround = BOARD_SIZE / 3;
	int energy = state["PlayerMap"]["Owner"]["Energy"].GetInt();
	//debug << "Energy " << energy << " " << energyperround;
	string outstr;
	if (energy >= 8*energyperround && SpecialHandler(state, &outstr, energy, energyperround, valid_points, working_directory))
	{
		//debug << "SpecialHandler true" << endl;
		ofs << outstr << endl;
		return;
	}

	for (auto pos = 0; pos < BOARD_SIZE; pos++)
	{
		point shot1 = { pos, pos };
		point shot2 = { BOARD_SIZE - pos, pos };

		if (Utility::IsIn(valid_points, shot1))
		{
			ofs << "1," << shot1.x << "," << shot1.y << "\n";
			//debug << "Route1\n";
			return;
		}
		else if (Utility::IsIn(valid_points, shot2))
		{
			ofs << "1," << shot2.x << "," << shot2.y << "\n";
			//debug << "Route2\n";
			return;
		}
	}

	int pos = 0;
	int it = 0;

	while (++it*MinLength <= BOARD_SIZE*0.5) {
		for (pos = 0; pos < BOARD_SIZE; pos++)
		{
			point shot1 = { pos + it*MinLength, pos };
			point shot2 = { pos, pos + it*MinLength };
			point shot3 = { BOARD_SIZE - pos - it*MinLength, pos };
			point shot4 = { BOARD_SIZE - pos, pos + it*MinLength };

			if (Utility::IsIn(valid_points, shot1))
			{
				ofs << "1," << shot1.x << "," << shot1.y << "\n";
				//debug << "Route3\n";
				return;
			}
			else if (Utility::IsIn(valid_points, shot2))
			{
				ofs << "1," << shot2.x << "," << shot2.y << "\n";
				//debug << "Route4\n";
				return;
			}
			else if (Utility::IsIn(valid_points, shot3))
			{
				ofs << "1," << shot3.x << "," << shot3.y << "\n";
				//debug << "Route5\n";
				return;
			}
			else if (Utility::IsIn(valid_points, shot4))
			{
				ofs << "1," << shot4.x << "," << shot4.y << "\n";
				//debug << "Route6\n";
				return;
			}
		}
	}

	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> cell_dist(0, valid_points.size() - 1);
	auto shot = valid_points[cell_dist(rng)];
	//debug << "Random shot taken" << endl;
	ofs << "1," << shot.x << "," << shot.y << "\n";
}

void MyBot::place_ships(const string working_directory, const int BOARD_SIZE, rapidjson::Document& state) 
{
	ofstream ofs(working_directory + "/" + place_filename);
	ofstream debugplace(working_directory + "/debugplace.txt");

	//debugplace << "Place init" << endl;
	
	auto ships = Ships::GetPlaceShipState(state);

	vector<point> occupied;

	for (int i = 0; i < 5; i++)
	{
		//debugplace << "Loop init " << ships[i].type << endl;

		bool searchforpos = true;
		
		int x, y;
		string dir;

		while (searchforpos)
		{
			bool valid = true;
			
			random_device rd;
			default_random_engine rng(rd());
			uniform_int_distribution<int> randcoord(0, BOARD_SIZE - 1);

			x = randcoord(rng);
			y = randcoord(rng);
			int k = ships[i].length - 1;

			//debugplace << x << " " << y << " " << k << endl;

			uniform_int_distribution<int> randorent(0, 3);

			switch (randorent(rng))
			{
			case 0:
				dir = "North";
				//debugplace << "N" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x, y + v })) { valid = false; /*debugplace << "Occupied" << endl;*/} }
				if (!(y + k >= 0 && y + k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x, y + v }); } } }
				break;
			case 1:
				dir = "South";
				//debugplace << "S" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x, y - v })) { valid = false; /*debugplace << "Occupied" << endl;*/} }
				if (!(y - k >= 0 && y - k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x, y - v }); } } }
				break;
			case 2:
				dir = "East";
				//debugplace << "E" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x + v, y })) { valid = false; /*debugplace << "Occupied" << endl;*/} }
				if (!(x + k >= 0 && x + k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x + v, y }); } } }
				break;
			case 3:
				dir = "West";
				//debugplace << "W" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x - v, y })) { valid = false; /*debugplace << "Occupied" << endl;*/} }
				if (!(x - k >= 0 && x - k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x - v, y }); } } }
				break;
			}
			if (valid) { searchforpos = false; }
		}
		ofs << ships[i].type << " " << x << " " << y << " " << dir << endl;
	}

	/*ofs << "Carrier 1 1 East\n";
	ofs << "Battleship 7 7 South\n";
	ofs << "Cruiser 9 0 North\n";
	ofs << "Submarine 4 4 West\n";
	ofs << "Destroyer 0 9 East\n";*/
}


int main(int argc, char** argv)
{
	if (argc != 3) {
		cout << "Usage: MyBot.exe PlayerKey WorkingDirectory" << endl;
		return 1;
	}
	string working_directory(argv[2]);
	auto state = Utility::parse_state(working_directory, state_filename);

	if (state["Phase"].GetInt() == 1) {
		MyBot::place_ships(working_directory, state["MapDimension"].GetInt(), state);
	}
	else {
		MyBot::fire_shot(working_directory, state);
	}

	return 0;
}
