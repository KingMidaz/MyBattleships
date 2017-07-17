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
