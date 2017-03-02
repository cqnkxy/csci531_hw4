#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>
#include "tablecheck.h"

using namespace std;

vector<int> IP, E, P, V, PC1, PC2, inv_IP;
vector<vector<int> > table_s;
static vector<vector<bool> > keys; // subkeys

int hex2dec(char ch)
{
	if (isdigit(ch)) {
		return ch - '0';
	} else {
		return tolower(ch) - 'a' + 10;
	}
}

// Check to see if key is valid, and also transform it to bits vec
void checkkey(const string &key, vector<bool> &key_bits)
{
	if (key.size() != 16) {
		cerr << "Key must be 8 bytes hex string" << endl;
		exit(1);
	}
	for (size_t i = 0; i < key.size(); ++i) {
		char ch = tolower(key[i]);
		if (!isalnum(ch) || (isalpha(ch) && !(ch >= 'a' && ch <= 'f'))) {
			cerr << "Malformed key" << endl;
			exit(1);
		}
	}
	for (int i = 0; i < 16; ++i) {
		int ch = hex2dec(key[i]);
		for (int j = 3; j >= 0; --j) {
			key_bits.push_back(ch & (1 << j));
		}
	}
}

vector<bool> xor_2vec(const vector<bool> &vec1, const vector<bool> &vec2)
{
	assert(vec1.size() == vec2.size());
	vector<bool> res;
	for (size_t i = 0; i < vec1.size(); ++i) {
		res.push_back(vec1[i] ^ vec2[i]);
	}
	return res;
}

vector<bool> combine2vec(const vector<bool> &A, const vector<bool> &B)
{
	vector<bool> AB = A;
	AB.insert(AB.end(), B.begin(), B.end());
	return AB;
}

// to hex string
string vec2str(const vector<bool> &vec) 
{
	int sz = vec.size();
	assert(sz % 8 == 0 && sz != 0);
	string res;
	char buf[3];
	for (int i = 0; i < sz; i += 8) {
		unsigned char ch = 0;
		for (int j = 0; j < 8; ++j) {
			ch |= ((unsigned char)vec[i+j] << (7-j));
		}
		sprintf(buf, "%02x", ch);
		res += string(buf);
	}
	return res;
}

// to char string
string vec2char(const vector<bool> &vec)
{
	int sz = vec.size();
	assert(sz % 8 == 0 && sz != 0);
	string res;
	for (int i = 0; i < sz; i += 8) {
		unsigned char ch = 0;
		for (int j = 0; j < 8; ++j) {
			ch |= (unsigned char)vec[i+j] << (7-j);
		}
		res += ch;
	}
	return res;
}

void left_circular_shift(vector<bool> &vec, int vi)
{
	for (int i = 0; i < vi; ++i) {
		vec.push_back(vec[i]);
	}
	vec.erase(vec.begin(), vec.begin()+vi);
}

// define vi (circular rotation values), 1 ≤ i ≤ 16 as follows vi = 1
// for i ∈ {1,2,9,16}, vi = 2 otherwise
void subkey(
	vector<bool> &key_bits, vector<vector<bool> >&keys,
	vector<int> &PC1, vector<int> &PC2
){
	vector<bool> C(28, 0), D(28, 0), k(48, 0);
	// T ← PC1(k): represent T as 28-bit halfs (C0,D0), use PC1 to select
	// bits from k: C0=b57b49...b36, D0=b63b55...b4
	for (int i = 0; i < 28; i++) {
		C[i] = key_bits[PC1[i]-1];
		D[i] = key_bits[PC1[i+28]-1];
	}
	fprintf(stderr, "(C0,D0)=%s\n", vec2str(combine2vec(C, D)).c_str());
	for (int i = 1; i <= 16; ++i) {
		int vi = V[i-1];
		left_circular_shift(C, vi);
		left_circular_shift(D, vi);
		vector<bool> CD = combine2vec(C, D);
		for (int j = 0; j < 48; ++j){
			k[j] = CD[PC2[j]-1];
		}
		keys.push_back(k);
		fprintf(stderr, "(C%d,D%d)=%s\n", i, i, vec2str(CD).c_str());
		fprintf(stderr, "k%d=%s\n", i, vec2str(k).c_str());
	}
}

// f(Ri-1, ki) = P(S(E(Ri-1) ⊕ ki))
vector<bool> func_f(const vector<bool> &R, const vector<bool> &ki)
{
	assert(R.size() == 32 && ki.size() == 48 && E.size() == 48 && P.size() == 32);
	vector<bool> tmp(48, 0);
	// E(Ri-1) ⊕ ki
	for (int i = 0; i < 48; ++i) {
		tmp[i] = R[E[i]-1];
	}
	tmp = xor_2vec(tmp, ki);
	// S
	vector<bool> ret;
	for (int i = 0, r = 0; i < 48; i += 6, r++) {
		vector<bool> six_bits = vector<bool>(tmp.begin()+i, tmp.begin()+i+6);
		unsigned outer = (six_bits[0] << 1) | six_bits[5];
		unsigned inner = (
			(six_bits[1] << 3) | (six_bits[2] << 2) |
			(six_bits[3] << 1) | (six_bits[4])
		);
		unsigned four_bits = table_s[r][outer*16+inner];
		for (int j = 3; j >= 0; --j) {
			ret.push_back((four_bits >> j) & 0x1);
		}
	}
	assert(ret.size() == 32);
	// p
	tmp.assign(32, 0);
	for (int i = 0; i < 32; ++i) {
		tmp[i] = ret[P[i]-1];
	}
	return tmp;
}

// encrypt 64 bits
vector<bool> encrypt_decrypt_block(
	vector<bool> &block, bool output, bool decrypt, vector<vector<bool> > &keys
){
	assert(block.size() == 64);
	vector<bool> L(32, 0), R(32, 0);
	// L0, R0
	for (int i = 0; i < 32; ++i) {
		L[i] = block[IP[i]-1];
		R[i] = block[IP[i+32]-1];
	}
	if (output) {
		fprintf(stderr, "(L0,R0)=%s\n", vec2str(combine2vec(L, R)).c_str());
	}
	for (int i = 0; i <= 15; ++i) {
		vector<bool> &key = decrypt ? keys[15-i] : keys[i];
		vector<bool> newL = R, newR = xor_2vec(L, func_f(R, key));
		if (output) {
			fprintf(stderr, "(L%d,R%d)=%s\n", i+1, i+1, 
				vec2str(combine2vec(newL, newR)).c_str());
		}
		L = newL;
		R = newR;
	}
	// irregular swap
	vector<bool> ret(64, 0), RL = combine2vec(R, L);
	// inverse IP
	for (int i = 0; i < 64; ++i) {
		ret[i] = RL[inv_IP[i]-1];
	}
	return ret;
}

void generate_inv_IP(vector<int> &IP, vector<int> &inv_IP)
{
	inv_IP.assign(64, 0);
	for (int i = 0; i < 64; ++i) {
		inv_IP[IP[i]-1] = i + 1;
	}
}

// encrypt and decrypt are almost the same except that they use subkeys in
// different order
void encrypt_decrypt(
	istream &in, string key, string tablefile, bool decrypt=false
){
	vector<bool> key_bits;
	checkkey(key, key_bits);
	ifstream infile(tablefile.c_str());
	if (!infile.is_open()) {
		cerr << "Tablefile not exist" << endl;
		exit(1);
	}
	tablecheck(infile, IP, E, P, V, PC1, PC2, table_s);
	infile.close();
	generate_inv_IP(IP, inv_IP);
	subkey(key_bits, keys, PC1, PC2);
	for (bool start = true; ; start = false) {
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
		cout << vec2char(encrypt_decrypt_block(block, start, decrypt, keys));
	}
}
