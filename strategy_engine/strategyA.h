#ifndef  __STRATEGYA_H_
#define  __STRATEGYA_H_

#include "strategy.h"

class StrategyA : public Strategy
{
    public:
        StrategyA(){};
        virtual ~StrategyA(){}
        virtual bool Run();
        virtual bool Init(){ return true; }
        virtual void Reset() { return; }
};












#endif  //__STRATEGYA_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
