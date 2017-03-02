#ifndef __TABLECHECK_H_
#define __TABLECHECK_H_

#include <iostream>

void tablecheck(std::istream&);
void tablecheck(
	std::istream &in,
	std::vector<int> &IP,
	std::vector<int> &E,
	std::vector<int> &P,
	std::vector<int> &V,
	std::vector<int> &PC1,
	std::vector<int> &PC2,
	std::vector<std::vector<int> > &table_s
);

#endif
