#include "strategyA.h"
#include <stdio.h>

DYN_CREATE_STRATEGY(StrategyA)

bool StrategyA::Run()
{
    printf("StrategyA\n");
    return true;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
