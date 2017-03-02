#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// table: table name
// lower and upper: the lower bound and upper bound occur times for each number
void check_permutation(const string &table, const string &value, size_t size, int range, int lower, int upper, vector<int> &res)
{
	vector<int> cntr(size, 0);
	stringstream ss(value);
	string item;
	try {
		while(getline(ss, item, ',')) {
			int val = atoi(item.c_str());
			if (val < 1 || val > (int)size) {
				fprintf(stderr, "(%-3s) %d not in range [%d, %lu]\n", table.c_str(), val, 1, size);
				exit(1);
			}
			if (++cntr[val-1] > upper) {
				fprintf(stderr, "(%-3s) %d occur too many times\n", table.c_str(), val);
				exit(1);
			}
			res.push_back(val);
		}
	} catch(...) {
		fprintf(stderr, "(%-3s) %s not integer value\n", table.c_str(), item.c_str());
		exit(1);
	}
	for (int val = 1; val <= range; ++val) {
		if (cntr[val-1] < lower) {
			fprintf(stderr, "(%-3s) %d occur too few times\n", table.c_str(), val);
			exit(1);
		}
	}
}

// IP: This table contains a random permutation of the values from
// 1 through 64, inclusive. (You need to compute the inverse transform
// in your code.)
void checkIP(const string &value, vector<int> &res)
{
	check_permutation("IP", value, 64, 64, 1, 1, res);
}

// E: This table contains 48 values. The allowed values are between
// 1 through 32, inclusive. All 32 numbers must appear in this table
// at least once and at most twice.
void checkE(const string &value, vector<int> &res)
{
	check_permutation("E", value, 48, 32, 1, 2, res);
}

// P: This table contains a random permutation of the values from
// 1 through 32, inclusive.
void checkP(const string &value, vector<int> &res)
{
	check_permutation("P", value, 32, 32, 1, 1, res);
}

// V: This table contains 16 values. Each value can be either 1 or 2. 
// There should be 4 ones and 12 twos.
void checkV(const string &value, vector<int> &res)
{
	int ones = 0, twos = 0;
	stringstream ss(value);
	string item;
	try {
		while(getline(ss, item, ',')) {
			int val = atoi(item.c_str());
			if (val != 1 && val != 2) {
				fprintf(stderr, "(%-3s) %d must be either 1 or 2\n", "V", val);
				exit(1);
			}
			ones += val == 1;
			twos += val == 2;
			res.push_back(val);
		}
	} catch(...) {
		fprintf(stderr, "(%-3s) %s not integer value\n", "V", item.c_str());
		exit(1);
	}
	if (ones != 4) {
		fprintf(stderr, "(%-3s) 1 not occur 4 times\n", "V");
	}
	if (twos != 12) {
		fprintf(stderr, "(%-3s) 2 not occur 12 times\n", "V");	
	}
}

// S#: This table contains 64 values, in a group of 16. Each group is
// a random permutation of the values from 0 through 15, inclusive.
void checkS(const string &value, vector<vector<int> > &table_s, int row)
{
	if (!table_s[row-1].empty()) {
		fprintf(stderr, "(%-2s%d) occur too many times\n", "S", row);
		exit(1);
	}
	stringstream ss(value);
	string item;
	try {
		for (int i = 0; i < 4; ++i) {
			vector<bool> exist(16, false);
			for (int j = 0; j < 16; ++j) {
				if (!getline(ss, item, ',')) {
					fprintf(stderr, "(%-2s%d) too few elements\n", "S", row);
				}
				int val = atoi(item.c_str());
				if (val < 0 || val > 15) {
					fprintf(stderr, "(%-2s%d) %d must be in range [0, 15]\n", "S", row, val);
					exit(1);
				}
				if (exist[val]) {
					fprintf(stderr, "(%-2s%d) %d occur too many times\n", "S", row, val);
					exit(1);
				}
				table_s[row-1].push_back(val);
				exist[val] = true;
			}
		}
	} catch(...) {
		fprintf(stderr, "(%-2s%d) %s not integer value\n", "S", row, item.c_str());
		exit(1);
	}
}

// PC1: This table contains 56 values. These values are a random 
// permutation of the numbers from 1 through 64, inclusive, with 
// the values which are multiples of 8 removed.
void checkPC1(const string &value, vector<int> &res)
{
	stringstream ss(value);
	string item;
	vector<bool> exist(64, false);
	try {
		while(getline(ss, item, ',')) {
			int val = atoi(item.c_str());
			if (val < 1 || val > 64 || val % 8 == 0) {
				fprintf(stderr, "(%-3s) %d must be in range [1, 64] and not multiple of 8\n", "PC1", val);
				exit(1);
			}
			if (exist[val-1]) {
				fprintf(stderr, "(%-3s) %d occur too many times\n", "PC1", val);
				exit(1);
			}
			res.push_back(val);
			exist[val-1] = true;
		}
	} catch(...) {
		fprintf(stderr, "(%-3s) %s not integer value\n", "PC1", item.c_str());
		exit(1);
	}
}

// PC2: This table contains 48 values. These values are a random
// permutation of the numbers from 1 through 56, inclusive, 
// with 8 randomly chosen numbers removed.
void checkPC2(const string &value, vector<int> &res)
{
	stringstream ss(value);
	string item;
	vector<bool> exist(56, false);
	try {
		while(getline(ss, item, ',')) {
			int val = atoi(item.c_str());
			if (val < 1 || val > 56) {
				fprintf(stderr, "(%-3s) %d must be in range [1, 56]\n", "PC2", val);
				exit(1);
			}
			if (exist[val-1]) {
				fprintf(stderr, "(%-3s) %d occur too many times\n", "PC2", val);
				exit(1);
			}
			res.push_back(val);
			exist[val-1] = true;
		}
	} catch(...) {
		fprintf(stderr, "(%-3s) %s not integer value\n", "PC2", item.c_str());
		exit(1);
	}
}

// Check & load tables.
void tablecheck(
	std::istream &in,
	vector<int> &IP,
	vector<int> &E,
	vector<int> &P,
	vector<int> &V,
	vector<int> &PC1,
	vector<int> &PC2,
	vector<vector<int> > &table_s
	)
{
	string line;
	table_s.resize(8);
	while (getline(in, line)) {
		size_t equal_pos = line.find('=');
		vector<int> res;
		if (equal_pos == string::npos) {
			cerr << "Malformed line!" << endl;
			exit(1);
		}
		string opt = line.substr(0, equal_pos);
		if (opt == "IP") {
			checkIP(line.substr(equal_pos+1, -1), IP);
		} else if (opt == "E") {
			checkE(line.substr(equal_pos+1, -1), E);
		} else if (opt == "P") {
			checkP(line.substr(equal_pos+1, -1), P);
		} else if (opt == "V") {
			checkV(line.substr(equal_pos+1, -1), V);
		} else if (opt == "PC1") {
			checkPC1(line.substr(equal_pos+1, -1), PC1);
		} else if (opt == "PC2") {
			checkPC2(line.substr(equal_pos+1, -1), PC2);
		} else if (opt[0] == 'S') {
			checkS(line.substr(equal_pos+1, -1), table_s, atoi(opt.c_str()+1));
		} else {
			cerr << "Unrecognized key" << endl;
			exit(1);
		}
	}
}

void tablecheck(std::istream &in)
{
	vector<int> IP, E, P, V, PC1, PC2;
	vector<vector<int> > table_s;
	tablecheck(in, IP, E, P, V, PC1, PC2, table_s);
}
