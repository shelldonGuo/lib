#include "strategy_engine.h"

StrategyEngine::~StrategyEngine()
{
    for(size_t i = 0; i < m_strategies.size(); ++i)
    {   
        delete m_strategies[i];
        m_strategies[i] = NULL;
    }   
    m_strategies.clear();
}

bool StrategyEngine::Init(const char * path)
{
    xmlKeepBlanksDefault(0);
    std::string filepath = "strategy.xml";  // => path
    xmlDocPtr docPtr = xmlReadFile(filepath.c_str(), "UTF-8", XML_PARSE_RECOVER); // ¶ÁÈëxmlÎÄµµ
    if (NULL == docPtr)
    {
        printf("Document not parsed successfully: [%s]", filepath.c_str() );
        return false;
    }
    xmlNodePtr curNode = xmlDocGetRootElement(docPtr); //µÃµ½¸ù½Úµã
    if (NULL == curNode)
    {
        printf("empty document : [%s]", filepath.c_str());
        return false;
    }
    // typedef unsigned char xmlChar;
    // curNode->name µÃµ½<name xxx>
    if (xmlStrcmp(curNode->name,  BAD_CAST "Strategies"))  // BAD_CAST ½«char * ×ª»¯Îª xmlChar *
    {
        printf("document of the wrong type, root node != Strategies");
        return false;
    }

    std::string strategyName = ""; 
    std::string soName = ""; 
    std::string confName = ""; 
    bool isOff = false;
    xmlChar* stAttr = NULL;

    // ±éÀúµ±Ç°½ÚµãµÄ×Ó½Úµã¼¯ºÏ
    // curNode->xmlChildrenNode µ±Ç°½ÚµãµÄ×Ó½Úµã
    // curNode->next µ±Ç°½ÚµãµÄÐÖµÜ½Úµã
    for(curNode = curNode->xmlChildrenNode; curNode != NULL; curNode = curNode->next) 
    {   
        strategyName = ""; 
        soName = ""; 
        confName = ""; 
        isOff = false; 
        stAttr = NULL;
        if (xmlStrcmp(curNode->name,  BAD_CAST "strategy"))
        {   
            printf("This phase has no strategy! [%s]", curNode->name);
            continue;
        }   
        stAttr = xmlGetProp(curNode, BAD_CAST "switch");   // µÃµ½ÊôÐÔÖµ
        if(stAttr != NULL && !xmlStrcmp(stAttr,  BAD_CAST "off"))
        {   
            isOff = true;
            xmlFree(stAttr);      //ÊÍ·ÅÄÚ´æ
        }   
        if(isOff)
            continue;
        stAttr = xmlGetProp(curNode, BAD_CAST "class");
        if(NULL == stAttr)
        {
            printf("document of the wrong type, curNode name is null!");
            continue;
        }
        strategyName = reinterpret_cast<char *>(stAttr);     //Ç¿ÖÆ×ª»»
        xmlFree(stAttr);

        stAttr = xmlGetProp(curNode, BAD_CAST "so");
        if(NULL == stAttr)
        {
            printf("document of the wrong type, curNode soNname is null!");
            continue;
        }
        soName = reinterpret_cast<char *>(stAttr);
        xmlFree(stAttr);

        void* handle = PluginHelper::Load(soName);
        if (handle == NULL)
        {
            printf("load so library %s fail.", soName.c_str());
            return false;
        }
        CreateStrategy_T createFactory = (CreateStrategy_T)(PluginHelper::GetCreatePoint(strategyName, \
                                          CREATE_STRATEGY_PREFIX, handle));
        Strategy* strategy = createFactory();
        if(NULL == strategy )
        {
            printf("Creat strategy [%s] is fail!", strategyName.c_str());
            return false;
        }
        if(!strategy ->Init())
        {
            printf("Init strategy [%s] is fail!", strategyName.c_str());
            return false;
        }
        m_strategies.push_back(strategy);
    }
    return true;
}

bool StrategyEngine::Run()
{
    for(size_t i = 0; i < m_strategies.size(); ++i)
    {
        if (!m_strategies[i]->Run())
        {
            return false;
        }
    }
    return true;
}


/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
