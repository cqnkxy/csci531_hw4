#ifndef __CRYPT_H_
#define __CRYPT_H_

#include <iostream>

void encrypt_decrypt(
	std::istream&,
	std::string key,
	std::string tablefile, 
	bool decrypt=false
);

#endif
