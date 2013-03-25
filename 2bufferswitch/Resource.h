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
	 * @brief	���캯��
	 *
	 * @name	CResource::CResource
	 * @return	�Ƿ�ɹ�ִ��
	 * @note	
	 */
	CResource();
	/**
	 * @brief	��������
	 *
	 * @name	CResource::~CResource
	 * @return	�Ƿ�ɹ�ִ��
	 * @note	
	 */
	virtual ~CResource();
	/**
	 * @brief	��ʼ������ȡ���úʹʵ�
	 *
	 * @param	pConfig
	 * @name	CResource::Initialize
	 * @return	�Ƿ�ɹ�ִ��
	 * @note	
	 */
	virtual int Initialize(char* conf_path);
	/**
	 * @brief	���¶�ȡ�ʵ�
	 *
	 * @name	CResource::Reload
	 * @return	�Ƿ�ɹ�ִ��
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
	 * @brief	��ȡ�ļ�
	 *
	 * @name	CResource::LoadDict
	 * @return	�Ƿ�ɹ�ִ��
	 * @note	
	 */
	int LoadDict();
    
    int DropCurPowerDict(int nCurDictId);

	/**
	 * @brief	���㵱ǰ������Ĵʵ����
	 *
	 * @name	CResource::SwitchToNewDict
	 * @return	�Ƿ�ɹ�ִ��
	 * @note	
	 */
	int SwitchToNewDict();
    int DropCurDict(int nCurDictId);

	/** �ʵ�buff���������޷��л� */
	static const int MAX_DICT_BUF_NUM;

	/** �ʵ����� */
	uint8_t m_nDefaultValue;
	/** �ʵ�·�� */
	char m_szDictPath[MAX_DIR_LEN];
	/** �ʵ䣬˫buff�л� */
	Dict **m_rgpDict;
	/** ��ǰ����Ĵʵ���� */
	int m_nCurServingDictId;

    rschelper::ResourceTracker m_rgResourceTracker;
};

#endif

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
