#pragma once
#include <vector>

class BitMap {
public:
	std::vector<int> inBoard;
	std::vector<int> blackField;
	std::vector<int> whiteField;
	std::vector<int> kingSquare;
	BitMap();
};
