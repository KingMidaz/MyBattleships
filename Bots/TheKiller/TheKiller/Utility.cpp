#include "Utility.h"

using namespace std;

bool Utility::IsInArray(string* ar, string st, const string working_directory)
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

bool Utility::IsIn(vector<point> vectorar, point shot)
{
	for (int i = 0; i < vectorar.size(); i++) {
		if (vectorar[i].x == shot.x && vectorar[i].y == shot.y)
		{
			return true;
		}
	}

	return false;
}

rapidjson::Document Utility::parse_state(const string working_directory, const string state_filename)
{
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

int Utility::valid_placement(const string working_directory, const int BOARD_SIZE, ship ship, int i, point place, unordered_set<point, Hash> validpos)
{
	//ofstream prob(working_directory + "/prob.txt", std::ofstream::out | std::ofstream::app);

	int score = 2;
	int k = ship.length - 1;

	int x = place.x;
	int y = place.y;

	//debugplace << x << " " << y << " " << k << endl;
	
	for (int v = 0; v < ship.length; v++) 
	{
		if ((validpos.find({ x, y + i + v }) == validpos.end())) 
		{ 
			//prob << x << " " << y+i+v << " Invalid "  << validpos.find({ x, y + i + v })->y << endl;
			score--; 
			break; 
		}
		//else { prob << x << " " << y+v+i << "Valid" << validpos.find({ x, y + i + v })->y << endl; }
	}
	for (int v = 0; v < ship.length; v++) 
	{
		if ((validpos.find({ x + i + v, y }) == validpos.end())) 
		{ 
			//prob << x+v+i << " " << y << "Invalid" << validpos.find({ x + i + v, y })->y << endl;
			score--; 
			break; 
		}
		//else { prob << x+i+v << " " << y << "Valid" << validpos.find({ x + i + v, y })->y << endl; }
	}

	return score;
}

pointprobability Utility::probability_score(const string working_directory, const int BOARD_SIZE, point place, unordered_set<point, Hash> validpos, vector<ship> ship)
{
	//ofstream prob(working_directory + "/prob.txt", std::ofstream::out | std::ofstream::app);

	int score = 0;

	for (auto k : ship)
	{
		for (int i = 1 - k.length; i < 1; i++)
		{
			//prob << k.length << "Ship" << endl;
			if (!ship[i].destroyed)
			{
				score += valid_placement(working_directory, BOARD_SIZE, k, i, { place.x, place.y }, validpos);
			}
		}
	}

	//prob << place.x << " " << place.y << " " << score << endl;

	return {place, score};
}