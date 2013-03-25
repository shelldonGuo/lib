#ifndef  __RESOUCE_H_
#define  __RESOUCE_H_

#include "ResourceTracker.h"
#include "stdio.h"

#define interface class
#define MAX_LEN 1024*10
#define MAX_DIR_LEN 1024
#define S_OK 0

typedef struct Dict_t
{
    int size;
    char buf[MAX_LEN];
} Dict;


interface IResource
{
    public:
        virtual int Initialize(char* conf_path) = 0;
        virtual int Reload() = 0;
};

class CResource : public IResource
{
public:
	/**
	 * @brief	构造函数
	 *
	 * @name	CResource::CResource
	 * @return	是否成功执行
	 * @note	
	 */
	CResource();
	/**
	 * @brief	析构函数
	 *
	 * @name	CResource::~CResource
	 * @return	是否成功执行
	 * @note	
	 */
	virtual ~CResource();
	/**
	 * @brief	初始化，读取配置和词典
	 *
	 * @param	pConfig
	 * @name	CResource::Initialize
	 * @return	是否成功执行
	 * @note	
	 */
	virtual int Initialize(char* conf_path);
	/**
	 * @brief	重新读取词典
	 *
	 * @name	CResource::Reload
	 * @return	是否成功执行
	 * @note	
	 */
	virtual int Reload();
	
	virtual int GetValue(char *pVal, int *puSize);
    virtual int SetValue(char *pVal, int puSize);

private:
	/** shaddow copy constructor and copy assignment */
	CResource(const CResource&);
	CResource& operator=(const CResource&);



protected:
    /**
	 * @brief	读取文件
	 *
	 * @name	CResource::LoadDict
	 * @return	是否成功执行
	 * @note	
	 */
	int LoadDict();
    
    int DropCurPowerDict(int nCurDictId);

	/**
	 * @brief	计算当前不服务的词典序号
	 *
	 * @name	CResource::SwitchToNewDict
	 * @return	是否成功执行
	 * @note	
	 */
	int SwitchToNewDict();
    int DropCurDict(int nCurDictId);

	/** 词典buff数，用于无缝切换 */
	static const int MAX_DICT_BUF_NUM;

	/** 词典类型 */
	uint8_t m_nDefaultValue;
	/** 词典路径 */
	char m_szDictPath[MAX_DIR_LEN];
	/** 词典，双buff切换 */
	Dict **m_rgpDict;
	/** 当前服务的词典序号 */
	int m_nCurServingDictId;

    rschelper::ResourceTracker m_rgResourceTracker;
};

#endif

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
