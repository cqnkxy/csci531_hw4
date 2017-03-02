#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include "crypt.h"
#include "tablecheck.h"

using namespace std;

extern vector<int> IP, E, P, V, PC1, PC2, inv_IP;
extern vector<vector<int> > table_s;
static vector<vector<vector<bool> > > keys_vec; //subkeys

// The same logic as crypt. Just do it three times
void encrypt3_decrypt3(
	std::istream &in, std::string key, std::string tablefile,  bool decrypt=false
){
	assert(key.size() == 48);
	vector<vector<bool> > key_bits_vec(3, vector<bool>());
	for (int i = 0; i < 3; i++) {
		checkkey(key.substr(i*16, 16), key_bits_vec[i]);
	}
	ifstream infile(tablefile.c_str());
	if (!infile.is_open()) {
		cerr << "Tablefile not exist" << endl;
		exit(1);
	}
	tablecheck(infile, IP, E, P, V, PC1, PC2, table_s);
	infile.close();
	generate_inv_IP(IP, inv_IP);
	keys_vec.resize(3);
	for (int i = 0; i < 3; i++) {
		subkey(key_bits_vec[i], keys_vec[i], PC1, PC2);
	}
	while(true) {
		unsigned char ch;
		vector<bool> block;
		for (int cnt = 0; cnt < 8 && in >> noskipws >> ch; ++cnt) {
			for (int i = 7; i >= 0; --i) {
				block.push_back((ch >> i) & 0x1);
			}
		}
		if (block.empty()) {
			break;
		}
		while(block.size() < 64){
			block.push_back(0);
		}
		// for encrypt, decrypt flag in [false, true, false], key order 0,1,2
		// for decrypt, decrypt flag in [true, false, true], key order 2,1,0
		block = encrypt_decrypt_block(block, false, false^decrypt, keys_vec[0^(decrypt<<1)]);
		block = encrypt_decrypt_block(block, false, true^decrypt, keys_vec[1]);
		block = encrypt_decrypt_block(block, false, false^decrypt, keys_vec[2^(decrypt<<1)]);
		cout << vec2char(block);
	}
}
