#include <stdio.h>
#include "strategy_engine.h"

int main()
{
    StrategyEngine* stEngine = new StrategyEngine();
    stEngine->Init("./");
    stEngine->Run();
    return 0;
}


















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
