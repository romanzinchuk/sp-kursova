#include "stdafx.h"
#include "Core/Parser/TokenRegister.h"
#include "Controller.h"
#include "Tokens/Common.h"

#include "Rules/Operators/If/IfRule.h"
#include "Rules/Operators/Goto/GotoRule.h"
#include "Rules/Operators/For/ForRule.h"
#include "Rules/Operators/WhileC/WhileRule.h"
#include "Rules/Operators/RepeatUntil/RepeatUntilRule.h"

void Init()
{
    Controller::Instance()->regOperatorRule(MakeIf);
    Controller::Instance()->regOperatorRule(MakeGoto, true);
    Controller::Instance()->regOperatorRule(MakeLabel);
    Controller::Instance()->regOperatorRule(MakeFor);
    Controller::Instance()->regOperatorRule(MakeWhile);
    Controller::Instance()->regOperatorRule(MakeRepeatUntil);

    Controller::Instance()->regItem<token::Unknown>(ItemType::TokenAndRule, -2);

    Controller::Instance()->regUnchangedTextToken(std::make_shared<Comment>(), std::make_shared<LComment>(), std::make_shared<RComment>());

    Controller::Instance()->init();
}