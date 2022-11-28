#pragma once
#include "includes.h"

std::vector<std::vector<char>> composePackages(std::vector<char> message, int packageSize);
void completePackage(std::vector<char> &package, int packageSize);