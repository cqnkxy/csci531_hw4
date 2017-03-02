#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "tablecheck.h"
#include "crypt.h"
#include "crypt3.h"

using namespace std;

const bool DEBUG = false;

void malformed_command() 
{
	const string USAGE = 
		"\t\nhw4 tablecheck -t=tablefile\n"
	    "\t\nhw4 encrypt -k=key -t=tablefile [file]\n"
	    "\t\nhw4 decrypt -k=key -t=tablefile [file]\n"
	    "\t\nhw4 encrypt3 -k=key3 -t=tablefile [file]\n"
	    "\t\nhw4 decrypt3 -k=key3 -t=tablefile [file]\n";
	cerr << "Malformed command. Use it as:" << endl << USAGE << endl;
	exit(1);
}

void parse_tablecheck(int argc, char *argv[]) 
{
	if (argc != 3) {
		malformed_command();
	}
	if (strncmp(argv[2], "-t=", 3) != 0) {
		malformed_command();
	}
	ifstream infile(argv[2]+3);
	if (!infile.is_open()) {
		cerr << "File not exists" << endl;
		exit(1);
	}
	tablecheck(infile);
	infile.close();
}

void parse_encrypt(int argc, char *argv[])
{
	if (argc != 4 && argc != 5) {
		malformed_command();
	}
	string key, tablefile;
	for (int i = 2; i <=3; i++) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-t=") {
			tablefile = opt.substr(3, -1);
		} else if (opt.substr(0, 3) == "-k=") {
			key = opt.substr(3, -1);
		} else {
			malformed_command();
		}
	}
	if (argc == 4) {
		encrypt_decrypt(cin, key, tablefile);
	} else {
		ifstream in(argv[4]);
		if (!in.is_open()) {
			cerr << "File not exits" << endl;
			exit(1);
		}
		encrypt_decrypt(in, key, tablefile);
		in.close();
	}
}

void parse_decrypt(int argc, char *argv[])
{
	if (argc != 4 && argc != 5) {
		malformed_command();
	}
	string key, tablefile;
	for (int i = 2; i <=3; i++) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-t=") {
			tablefile = opt.substr(3, -1);
		} else if (opt.substr(0, 3) == "-k=") {
			key = opt.substr(3, -1);
		} else {
			malformed_command();
		}
	}
	if (argc == 4) {
		encrypt_decrypt(cin, key, tablefile, true);
	} else {
		ifstream in(argv[4]);
		if (!in.is_open()) {
			cerr << "File not exits" << endl;
			exit(1);
		}
		encrypt_decrypt(in, key, tablefile, true);
		in.close();
	}
}

void parse_encrypt3(int argc, char *argv[])
{
	if (argc != 4 && argc != 5) {
		malformed_command();
	}
	string key, tablefile;
	for (int i = 2; i <=3; i++) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-t=") {
			tablefile = opt.substr(3, -1);
		} else if (opt.substr(0, 3) == "-k=") {
			key = opt.substr(3, -1);
		} else {
			malformed_command();
		}
	}
	if (argc == 4) {
		encrypt3_decrypt3(cin, key, tablefile, false);
	} else {
		ifstream in(argv[4]);
		if (!in.is_open()) {
			cerr << "File not exits" << endl;
			exit(1);
		}
		encrypt3_decrypt3(in, key, tablefile, false);
		in.close();
	}
}

void parse_decrypt3(int argc, char *argv[])
{	
	if (argc != 4 && argc != 5) {
		malformed_command();
	}
	string key, tablefile;
	for (int i = 2; i <=3; i++) {
		string opt(argv[i]);
		if (opt.substr(0, 3) == "-t=") {
			tablefile = opt.substr(3, -1);
		} else if (opt.substr(0, 3) == "-k=") {
			key = opt.substr(3, -1);
		} else {
			malformed_command();
		}
	}
	if (argc == 4) {
		encrypt3_decrypt3(cin, key, tablefile, true);
	} else {
		ifstream in(argv[4]);
		if (!in.is_open()) {
			cerr << "File not exits" << endl;
			exit(1);
		}
		encrypt3_decrypt3(in, key, tablefile, true);
		in.close();
	}
}

void parse_cmd_run(int argc, char *argv[]) 
{
	if (argc < 2) {
		malformed_command();
	} else if (strcmp(argv[1], "tablecheck") == 0) {
		parse_tablecheck(argc, argv);
	} else if (strcmp(argv[1], "encrypt") == 0) {
		parse_encrypt(argc, argv);
	} else if (strcmp(argv[1], "decrypt") == 0) {
		parse_decrypt(argc, argv);
	} else if (strcmp(argv[1], "encrypt3") == 0) {
		parse_encrypt3(argc, argv);
	} else if (strcmp(argv[1], "decrypt3") == 0) {
		parse_decrypt3(argc, argv);
	} else {	
		malformed_command();
	}
}

int main(int argc, char *argv[]) 
{
	parse_cmd_run(argc, argv);
	return 0;
}
