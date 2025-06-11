#include "stdafx.h"
#include "IdentRule.h"

#include "Rules/IdentRule/Identifier.h"
#include "Rules/IdentRule/Undefined.h"


#include "SimpleTokens.h"
SimpleToken(ProgramName, "");

BackusRulePtr MakeIdentRule(std::shared_ptr<Controller> controller)
{
    using enum ItemType;

    controller->regItem<Identifier>(TokenAndRule, -1);

    GeneratorUtils::Instance()->RegisterOperand(Identifier::Type());

    auto context = controller->context();

    auto singleIdentRule = controller->addRule(context->SingleIdentRuleName(), {
        BackusRuleItem({ Identifier::Type()}, OnlyOne)
        });

    singleIdentRule->setPostHandler([context](BackusRuleList::iterator&,
        BackusRuleList::iterator& it,
        BackusRuleList::iterator& end)
    {
        static bool isFirstIdentChecked = !context->IsFirstProgName();
        auto isVarBlockChecked = context->IsVarBlockChecked();
        auto& identTable = context->IdentTable();

        auto identIt = std::prev(it, 1);
        if (isVarBlockChecked)
        {
            if (!identTable.contains((*identIt)->value()))
            {
                auto undef = std::make_shared<Undefined>();
                undef->setValue((*identIt)->value());
                undef->setLine((*identIt)->line());
                undef->setCustomData((*identIt)->customData());
                *identIt = undef;
            }
        }
        else
        {
            if (isFirstIdentChecked)
            {
                identTable.insert((*identIt)->value());
            }
            else
            {
                auto progName = std::make_shared<ProgramName>();
                progName->setValue((*identIt)->value());
                progName->setLine((*identIt)->line());
                progName->setCustomData((*identIt)->customData());
                *identIt = progName;
                isFirstIdentChecked = true;
            }
        }

        (*identIt)->setCustomData((*identIt)->value() + "_");
    });

	auto arrayRule = controller->addRule("ArrayRule", {
	BackusRuleItem({  singleIdentRule->type()}, OnlyOne),
	BackusRuleItem({     LSquare::Type()}, OnlyOne),
	BackusRuleItem({ context->EquationRuleName() }, OnlyOne),
	BackusRuleItem({       RSquare::Type()}, OnlyOne)
	});

	auto identRule = controller->addRule(context->IdentRuleName(), {
    BackusRuleItem({  arrayRule->type(), singleIdentRule->type()}, OnlyOne),
	});

	return identRule;
}