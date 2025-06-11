#pragma once
#include "stdafx.h"
#include "Core/Tokens/TokenBase.hpp"
#include "Core/Backus/BackusRuleBase.h"
#include "Core/Generator/GeneratorItemBase.h"
#include "SimpleTokens.h"
#include "util.h"
class Assignment : public TokenBase<Assignment>, public BackusRuleBase<Assignment>, public GeneratorItemBase<Assignment>
{
    BASE_ITEM

public:
    Assignment() { setLexeme(":="); };
    virtual ~Assignment() = default;

    void genCode(std::ostream& out, GeneratorDetails& details,
        std::list<std::shared_ptr<IGeneratorItem>>::iterator& it,
        const std::list<std::shared_ptr<IGeneratorItem>>::iterator& end) const final
    {
        std::reverse_iterator ident(it);
        it++;
        const auto inst = GeneratorUtils::Instance();
        inst->GenCodeSync(out, details, it, end);

		if ((*ident)->type() == RSquare::Type()) {
            findOpener(ident, RSquare::Type(), LSquare::Type());
            ++ident;
			const auto args = details.args();
            const auto pref = args.regPrefix;
            auto fwdit = ident.base();
            --fwdit;
            inst->GenCodeSync(out, details, fwdit, end, RSquare::Type());
            out << "\tpop " << args.numberTypeExtended << " ptr _gc;\n";
            out << "\tpop " << args.numberTypeExtended << " ptr [" << (*ident)->customData() << " + " << pref << "bx]\n";

        }
		else {
			out << "\tpop " << (*ident)->customData() << std::endl;
		}
    };
};
