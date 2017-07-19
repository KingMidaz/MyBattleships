#include "Killer.h"

using namespace std;

constexpr char state_filename[] = "state.json";
constexpr char place_filename[] = "place.txt";
constexpr char command_filename[] = "command.txt";

bool Killer::HitHandler(const string working_directory, vector<point> valid_points, vector<point> hits, point* out, vector<ship> liveships, int min, int max)
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
			if (Utility::IsIn(valid_points, { hits[i].x - 1 + f, hits[i].y - 1 }) || Utility::IsIn(valid_points, { hits[i].x - 1 + f, hits[i].y + 1 }))
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

bool Killer::SpecialHandler(rapidjson::Document& state, string* outstr, int energy, int energyperround, pointprobability maxprob, const string working_directory)
{
	vector<ship> live = Ships::GetPlaceShipState(state);

	ofstream specialdebug(working_directory + "/specialdebug.txt");
	specialdebug << "Special debug init" << endl;

	stringstream ss;

	for (int l = 0; l < 5; l++)
	{
		if (live[l].type.compare("Battleship") == 0 && !live[l].destroyed)
		{
			if (energy >= 12 * energyperround)
			{
				ss << "5," << maxprob.pt.x << "," << maxprob.pt.y << endl;
				*outstr = ss.str();
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
				ss << "7," << maxprob.pt.x << "," << maxprob.pt.y << endl;
				*outstr = ss.str();
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
				ss << "6," << maxprob.pt.x << "," << maxprob.pt.y << endl;
				*outstr = ss.str();
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
				ss << "4," << maxprob.pt.x << "," << maxprob.pt.y << endl;
				*outstr = ss.str();
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
				ss << "3," << maxprob.pt.x << "," << maxprob.pt.y << endl;
				*outstr = ss.str();
				return true;
			}
			return false;
		}
	}

	return false;
}

void Killer::fire_shot(const string working_directory, rapidjson::Document& state)
{

	ofstream ofs(working_directory + "/" + command_filename);
	ofstream debug(working_directory + "/debug.txt");

	debug << "Debug init" << endl;

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

	vector<ship> EnemyShips = Ships::GetShipState(state);

	const int BOARD_SIZE = state["MapDimension"].GetInt();
	int MaxLength = 0;
	int MinLength = 5;

	for (int i = 0; i < 5; i++)
	{
		if (MaxLength < EnemyShips[i].length && !EnemyShips[i].destroyed)
		{
			MaxLength = EnemyShips[i].length;
		}
		if (MinLength > EnemyShips[i].length && !EnemyShips[i].destroyed)
		{
			MinLength = EnemyShips[i].length;
		}
		if (!EnemyShips[i].destroyed) { debug << EnemyShips[i].type << endl; }
	}

	debug << "Max " << MaxLength << endl;
	debug << "Min " << MinLength << endl;

	point out;
	if (HitHandler(working_directory, valid_points, hits, &out, EnemyShips, MinLength, MaxLength))
	{
		debug << "Hithandler true" << endl;
		debug << "Out true" << out.x << " " << out.y << endl;
		ofs << "1," << out.x << "," << out.y << "\n";
		return;

	}
	
	vector<pointprobability> pdf;
	//set<ship> sh(EnemyShips.begin(), EnemyShips.end());
	unordered_set<point, pair_hash> vp(valid_points.begin(), valid_points.end());
	for (auto v : vp)
	{
		pdf.push_back(Utility::probability_score(working_directory, BOARD_SIZE, v, vp, EnemyShips));
		debug << v.x << " " << v.y << endl;
	}

	int maxprob = 0;
	for (auto q : pdf)
	{
		debug << maxprob << " " << q.score << endl;
		if (q.score > maxprob) { maxprob = q.score; }
	}

	vector<pointprobability> maxar;
	for (auto q : pdf)
	{
		if (q.score == maxprob) { maxar.push_back(q); debug << q.score << " " << q.pt.x << " " << q.pt.y << endl; }
	}

	pointprobability pick;

	for (auto j : maxar)
	{
		for (auto pos = 0; pos < BOARD_SIZE; pos++)
		{
			point shot1 = { pos, pos };
			point shot2 = { BOARD_SIZE - pos, pos };

			if (j.pt.x == shot1.x && j.pt.y == shot1.y)
			{
				pick = j;
				goto label;
			}
			else if (j.pt.x == shot2.x && j.pt.y == shot2.y)
			{
				pick = j;
				goto label;
			}
		}

		int pos = 0;
		int it = 0;

		while (++it*MinLength <= BOARD_SIZE*0.5)
		{
			for (pos = 0; pos < BOARD_SIZE; pos++)
			{
				point shot1 = { pos + it*MinLength, pos };
				point shot2 = { pos, pos + it*MinLength };
				point shot3 = { BOARD_SIZE - pos - it*MinLength, pos };
				point shot4 = { BOARD_SIZE - pos, pos + it*MinLength };

				if (j.pt.x == shot1.x && j.pt.y == shot1.y)
				{
					pick = j;
					goto label;
				}
				else if (j.pt.x == shot2.x && j.pt.y == shot2.y)
				{
					pick = j;
					goto label;
				}
				else if (j.pt.x == shot3.x && j.pt.y == shot3.y)
				{
					pick = j;
					goto label;
				}
				else if (j.pt.x == shot4.x && j.pt.y == shot4.y)
				{
					pick = j;
					goto label;
				}
			}
		}
	}

	random_device rd;
	default_random_engine rng(rd());
	uniform_int_distribution<int> cell_dist(0, maxar.size() - 1);
	pick = maxar[cell_dist(rng)];
	debug << "Random shot taken" << endl;
label:

	int energyperround = BOARD_SIZE / 3;
	int energy = state["PlayerMap"]["Owner"]["Energy"].GetInt();
	debug << "Energy " << energy << " " << energyperround;
	string outstr;
	if (energy >= 8 * energyperround && SpecialHandler(state, &outstr, energy, energyperround, pick, working_directory))
	{
		debug << "SpecialHandler true" << endl;
		ofs << outstr << endl;
		return;
	}

	ofs << "1," << pick.pt.x << "," << pick.pt.y << endl;
	return;

}

void Killer::place_ships(const string working_directory, const int BOARD_SIZE, rapidjson::Document& state)
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
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x, y + v })) { valid = false; /*debugplace << "Occupied" << endl;*/ } }
				if (!(y + k >= 0 && y + k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x, y + v }); } } }
				break;
			case 1:
				dir = "South";
				//debugplace << "S" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x, y - v })) { valid = false; /*debugplace << "Occupied" << endl;*/ } }
				if (!(y - k >= 0 && y - k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x, y - v }); } } }
				break;
			case 2:
				dir = "East";
				//debugplace << "E" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x + v, y })) { valid = false; /*debugplace << "Occupied" << endl;*/ } }
				if (!(x + k >= 0 && x + k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x + v, y }); } } }
				break;
			case 3:
				dir = "West";
				//debugplace << "W" << endl;
				for (int v = 0; v < ships[i].length; v++) { if (Utility::IsIn(occupied, { x - v, y })) { valid = false; /*debugplace << "Occupied" << endl;*/ } }
				if (!(x - k >= 0 && x - k < BOARD_SIZE)) { valid = false; }
				if (valid) { { for (int v = 0; v < ships[i].length; v++) { occupied.push_back({ x - v, y }); } } }
				break;
			}
			if (valid) { searchforpos = false; }
		}
		ofs << ships[i].type << " " << x << " " << y << " " << dir << endl;
	}
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
		Killer::place_ships(working_directory, state["MapDimension"].GetInt(), state);
	}
	else {
		Killer::fire_shot(working_directory, state);
	}

	return 0;
}
