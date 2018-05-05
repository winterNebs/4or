#pragma once
#include <vector>
#include "xmlTag.h"
#include <iostream>

class svg {
public:
	svg();
	~svg();
	void parse(std::string input);
};