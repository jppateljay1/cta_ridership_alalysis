/*
 * CTA "L" Ridership Analysis: main program file
 * 
 * Jay Patel
 * U. of Illinois, Chicago
 * CS 341, Spring 2016
 * HW #2
 */

#include <sstream>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>


// classes don not really have much, so all in one file
using namespace std;

/*
 * Once data gets read from Station vector
 * it pretty much becomes useless
 */
class Station
{
public:
	int station_id;
	string station_name;

	Station(int s, string sn)
		: station_id(s), station_name(sn)
	{}
};

/*
 * pretty much the back bone of all the structures
 * stores all the variables
 */
class Stop
{
public:
	int station_id;
	string service_day;
	int total_rides;
	string station_name;

	Stop(int st_id, string sr_day, int ttl_rides, string s_name)
		: station_id(st_id), service_day(sr_day), total_rides(ttl_rides), station_name(s_name)
	{}
};

/*
 * Checks if the station number already exists in the vector
 * return true if it does
 * else false otherwise
 */
bool check_station(vector<Station>& V, int station)
{
	auto ret = find_if(V.begin(), V.end(),
		[=](Station& elem)
		{
			if (elem.station_id == station)
				return true;
			else
				return false;
	});

		if (ret != V.end())
			return true;
		/*
		 * if the code makes it here, that means the value was not found
		 * we are probably trying to insert a new station
		 */
		return false;
}

/*
 * iterate through until id and service day is matched
 * if matched, then updates the total_rides
 * else returns false, which means that entity does not exist
 */
bool apply_service_day(vector<Stop>& V, int id, string s_day, int rides)
{
	auto ret = find_if(V.begin(), V.end(),
		[=](Stop& elem)
		{
			if (elem.station_id == id && elem.service_day == s_day)
			{
				elem.total_rides = elem.total_rides + rides;
				return true;
			}
			else
				return false;
	});

	if (ret != V.end())
		return true;
	else
		return false;
}

/*
 * returns a name based on the id associated with it
 */
string get_name(vector<Station>& V, int id)
{
	string to_ret = "no_name";
	auto ret = find_if(V.begin(), V.end(),
		[&](Station& elem)
		{
			if (elem.station_id == id)
			{
				to_ret = elem.station_name;
				return true;
			}
			else
				return false;
	});

	return to_ret;
}

/*
 * lambda function
 */
bool print_all_sort(Stop& s1, Stop& s2)
{
	if (s1.total_rides > s2.total_rides)
		return true;
	else
		return false;
}

/*
 * once the function receives a sorted vector
 * it then begins printing
 */
void print_all(vector<Stop>& V, string ch)
{
	// first, prints the Top-5 Ridership statement based on the input string
	if (ch == "W")
		cout << "\nTop-5 Ridership on Weekdays:" << endl;
	else if (ch == "U")
		cout << "\nTop-5 Ridership on Sunday/Holiday:" << endl;
	else
		cout << "\nTop-5 Ridership on Saturday: " << endl;

	int count = 1;

	/*
	 * This for loop basically goes through each vector and prints the top
	 * has to match the string input value
	 */
	for (int i = 0; i < V.size(); i++)
	{
		if (V[i].service_day == ch)
		{
			cout << count << ". " << V[i].station_name << ": " << V[i].total_rides << endl;
			count++;
		}
		// performs a check to see 5 objects have been printed out or not
		if (count > 5)
			break;
	}
}

int main()
{
	cout << "** CTA L Ridership Analysis Program **";

	vector<Stop> all_stops;
	vector<Station> all_stations;
	int to_print_total_rides = 0, to_print_num_stops = 0, to_print_num_stations = 0;

	cout << "Inputting stations and stops from 'CTA-L-Stops.csv'...." << endl;

	string line;

	// reads data and puts it all in all_stations vector
	fstream file_stops("CTA-L-Stops.csv");
	if (!file_stops.good())
		throw invalid_argument("file 'CTA-L-Stops.csv' not found");

	getline(file_stops, line); // discard first line: column headers

	while (getline(file_stops, line))
	{
		stringstream ss(line);

		string station_id, junk, station_name;

		getline(ss, station_id, ',');
		getline(ss, junk, ',');
		getline(ss, junk, ',');
		getline(ss, station_name, ',');
		getline(ss, junk); // rest of the line

		int id = stoi(station_id);

		// if the following line returns true then the station alrady exists
		if (check_station(all_stations, id))
		{
			to_print_num_stops++;
			continue;
		}

		// at this point, we are trying to add a new station
		to_print_num_stops++;
		to_print_num_stations++;

		Station s(id, station_name);
		all_stations.push_back(s);
	}

	cout << "Num stations: " << to_print_num_stations << endl;
	cout << "Num stops: " << to_print_num_stops << endl;

	cout << "\nInputting stations and stops from 'CTA-L-Ridership-Daily-2weeks.csv'...." << endl;

	// reads data and puts in all_stops vector
	fstream file_station("CTA-L-Ridership-Daily-2weeks.csv");
	if (!file_station.good())
		throw invalid_argument("file 'CTA-L-Ridership-Daily-2weeks.csv' not found");

	getline(file_station, line); // discard first line: column headers

	while (getline(file_station, line))
	{
		stringstream ss(line);

		string station_id, service_day, total_rides, junk;

		getline(ss, station_id, ',');
		getline(ss, junk, ',');
		getline(ss, service_day, ',');
		getline(ss, total_rides);

		int id = stoi(station_id);
		int t_rides = stoi(total_rides);

		if (check_station(all_stations, id))
		{
			to_print_total_rides = to_print_total_rides + t_rides;

			if (apply_service_day(all_stops, id, service_day, t_rides))
				continue;
			string s_name = get_name(all_stations, id);
			Stop s(id, service_day, t_rides, s_name);
			all_stops.push_back(s);
		}
	}

	cout << "Total ridership: " << to_print_total_rides << endl;

	// sorts the entire vector sending it to print_all function
	sort(all_stops.begin(), all_stops.end(), print_all_sort);
	
	// prints all the
	vector<string> tmp = { "W", "A", "U" };
	for (int i = 0; i < tmp.size(); i++)
		print_all(all_stops, tmp[i]);

	// the next part of code has been added to make sure the program doesn't end after reading data
	int garbage;
	cin >> garbage;
	return 0;
}
