#pragma once
#include "stdafx.h"
#include "Core/IItem.h"
#include "Core/Generator/GeneratorDetails.h"

__interface IGeneratorItem : public IItem
{
public:
    virtual void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const = 0;
};