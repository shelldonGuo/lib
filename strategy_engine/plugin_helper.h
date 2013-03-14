#ifndef  __PLUGIN_HELPER_H_
#define  __PLUGIN_HELPER_H_

#include <dlfcn.h>
#include <string>
#include <map>

class PluginHelper
{
public:

    /**
     ** @brief ���Ĭ�ϼ���·��.
     **  
     **/
    static const std::string PLUGINS_PATH;

    /**
     ** @brief ���ض�̬��.
     **
     ** @param [in] soName   : const std::string& ��̬������.
     ** @return  void* ��̬����.
     **/
    static void* Load(const std::string& soName);

    /**
     ** @brief ж�ض�̬��.
     **
     ** @param [in] soName   : const std::string& ��̬������.
     ** @return  void 
     **/
    static void Unload(void* handle);

    static void* GetCreatePoint(const std::string& className, \
                                const std::string& createPrefix, \
                                void* handle);
};















#endif  //__PLUGIN_HELPER_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
