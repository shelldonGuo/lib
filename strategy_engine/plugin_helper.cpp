#include "plugin_helper.h"

const std::string PluginHelper::PLUGINS_PATH = "plugins/";

void* PluginHelper::Load(const std::string& soName)
{
    void* handle = NULL;
    handle = dlopen(soName.c_str(), RTLD_GLOBAL|RTLD_NOW);
    if(NULL == handle)
    {
        printf("dlopen [%s] error:  [%s]", soName.c_str(), dlerror());
    }
    return handle;
}

void PluginHelper::Unload(void* handle)
{
    if(NULL == handle)
    {
        printf("handle is NULL!");
        return;
    }
    dlclose(handle);
    return;
}

void* PluginHelper::GetCreatePoint(const std::string& className, \
                                   const std::string& createPrefix, \
                                   void* handle)
{
    void *pCreatePoint = NULL;
    std::string classCreateName = createPrefix + className;
    pCreatePoint = dlsym(handle, classCreateName.c_str());
    if(NULL == pCreatePoint)
    {
        printf("dlsym [%s] error: [%s]", classCreateName.c_str(), dlerror());
    }
    return pCreatePoint;
}
        
















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
