#pragma once
#include "stdafx.h"
#include "Core/Backus/BackusRuleBase.h"

unsigned int nextPowerOfTwo(unsigned int n);

bool findOpener(std::list<std::shared_ptr<IGeneratorItem>>::reverse_iterator& it,
	const std::string& incrementer, const std::string& decrementer);