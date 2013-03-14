#ifndef  __STRATEGYB_H_
#define  __STRATEGYB_H_

#include "strategy.h"

class StrategyB : public Strategy
{
    public:
        StrategyB(){};
        virtual ~StrategyB(){}
        virtual bool Run();
        virtual bool Init(){ return true; }
        virtual void Reset() { return; }
};












#endif  //__STRATEGYB_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
