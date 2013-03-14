#ifndef  __STRATEGY_ENGINE_H_
#define  __STRATEGY_ENGINE_H_

#include "strategy.h"
#include "plugin_helper.h"

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <stdio.h>
#include <string>
#include <vector>

class StrategyEngine
{
public:
    StrategyEngine(){}
    ~StrategyEngine();

    bool Init(const char* path);
    bool Run();
private:
    std::vector<Strategy*> m_strategies;
};















#endif  //__STRATEGY_ENGINE_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
