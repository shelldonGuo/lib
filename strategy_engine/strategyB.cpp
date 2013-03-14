#include <strategyB.h>
#include <stdio.h>

DYN_CREATE_STRATEGY(StrategyB)

bool StrategyB::Run()
{
    printf("StrategyB\n");
    return true;
}

















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
