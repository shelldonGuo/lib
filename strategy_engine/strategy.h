#ifndef  __STRATEGY_H_
#define  __STRATEGY_H_

#include <string>

/**
 ** @brief ¸¨Öúºê£ºÉùÃ÷Ò»¸ö¶¯Ì¬¼ÓÔØ²ßÂÔ.
 ** @note ´Ë¼ÓÔØµãÓÉ¿ò¼ÜÊ¶±ð²¢¼ÓÔØ, ¶ÔÍ¬Ò»¸ökey¼üµÄ²ßÂÔ½öÄÜÉùÃ÷Ò»´Î.
 **/
#define DYN_CREATE_STRATEGY(strategyName) \
        extern "C" \
    Strategy* CreateStrategy##strategyName() \
{ return new strategyName(); } 

/**
 ** @brief ¶¯Ì¬¼ÓÔØÍ³Ò»Ç°×º.
 **/
static const std::string CREATE_STRATEGY_PREFIX = "CreateStrategy";

class Strategy
{
public:
    Strategy(){}
    virtual ~Strategy(){}
    virtual bool Run() = 0;
    virtual bool Init() = 0;
    virtual void Reset() = 0;
};


typedef Strategy* (*CreateStrategy_T)();








#endif  //__STRATEGY_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
