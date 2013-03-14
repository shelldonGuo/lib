#ifndef  __PLUGIN_HELPER_H_
#define  __PLUGIN_HELPER_H_

#include <dlfcn.h>
#include <string>
#include <map>

class PluginHelper
{
public:

    /**
     ** @brief 插件默认加载路径.
     **  
     **/
    static const std::string PLUGINS_PATH;

    /**
     ** @brief 加载动态库.
     **
     ** @param [in] soName   : const std::string& 动态库名称.
     ** @return  void* 动态库句柄.
     **/
    static void* Load(const std::string& soName);

    /**
     ** @brief 卸载动态库.
     **
     ** @param [in] soName   : const std::string& 动态库名称.
     ** @return  void 
     **/
    static void Unload(void* handle);

    static void* GetCreatePoint(const std::string& className, \
                                const std::string& createPrefix, \
                                void* handle);
};















#endif  //__PLUGIN_HELPER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
