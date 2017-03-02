#ifndef __CRYPT3_H_
#define __CRYPT3_H_

#include <iostream>

void encrypt3_decrypt3(
	std::istream&,
	std::string key,
	std::string tablefile,
	bool decrypt=false
);

#endif
