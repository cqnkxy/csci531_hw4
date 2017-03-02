#ifndef __CRYPT_H_
#define __CRYPT_H_

#include <iostream>

void encrypt_decrypt(
	std::istream&,
	std::string key,
	std::string tablefile, 
	bool decrypt=false
);
std::vector<bool> encrypt_decrypt_block(
	std::vector<bool> &block, bool output, bool decrypt, std::vector<std::vector<bool> > &keys
);
void checkkey(const std::string &key, std::vector<bool> &key_bits);
void generate_inv_IP(std::vector<int> &IP, std::vector<int> &inv_IP);
void subkey(
	std::vector<bool> &key_bits, std::vector<std::vector<bool> >&keys,
	std::vector<int> &PC1, std::vector<int> &PC2
);
std::string vec2char(const std::vector<bool> &vec);

#endif
