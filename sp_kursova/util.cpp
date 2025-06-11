#include "util.h"
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"


unsigned int nextPowerOfTwo(unsigned int n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
}

bool findOpener(std::list<std::shared_ptr<IGeneratorItem>>::reverse_iterator& it,
	const std::string& incrementer, const std::string& decrementer) {
	int acc = 0;
	do {
		const auto item = *it;
		const auto itemType = item->type();
		if (item->type() == incrementer) {
			++acc;
		}
		if (item->type() == decrementer) {
			--acc;
		}
		if (acc == 0) {
			return true;
		}
		++it;
	} while (acc);
	return 0;
}