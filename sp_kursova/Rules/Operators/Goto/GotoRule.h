#pragma once
#include "stdafx.h"
#include "Controller.h"

BackusRulePtr MakeGoto(std::shared_ptr<Controller> controller);
BackusRulePtr MakeLabel(std::shared_ptr<Controller> controller);