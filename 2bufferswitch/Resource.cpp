#include "Resource.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

const int CResource::MAX_DICT_BUF_NUM = 2;

CResource::CResource() :
	m_nDefaultValue(0),
	m_rgpDict(NULL),
	m_nCurServingDictId(0)
{
	m_rgpDict = new Dict*[MAX_DICT_BUF_NUM];
	for (int i = 0; i < MAX_DICT_BUF_NUM; ++i)
	{
		m_rgpDict[i] = NULL;
	}
}

CResource::~CResource()
{
	if (m_rgpDict != NULL)
	{
		for (int i = 0; i < MAX_DICT_BUF_NUM; ++i)
		{
			if (m_rgpDict[i] != NULL)
			{
				delete m_rgpDict[i];
				m_rgpDict[i] = NULL;
			}
		}

		delete[] m_rgpDict;
		m_rgpDict = NULL;
	}
}

int CResource::Initialize(char* dict_path)
{
	int ret;
	if (dict_path == NULL)
	{
		printf("dict_path is NULL\n");
		return -1;
	}
	memcpy(m_szDictPath, dict_path, MAX_DIR_LEN);
	ret = LoadDict(); 

	return ret;
}

int CResource::Reload()
{
	int ret = LoadDict();
	if (ret < 0)
	{
		printf("reload dict [%s] failed",m_szDictPath);
	}

	return ret;
}

int CResource::LoadDict()
{
	int ret = S_OK;
	int nNewDictId = SwitchToNewDict();
	m_rgpDict[nNewDictId] = new Dict;

	FILE *fp = fopen(m_szDictPath,"r");
	if (fp == NULL)
	{
		printf("open file[%s] faild\n",m_szDictPath);
		return -1;
	}

	ret = fread(m_rgpDict[nNewDictId]->buf, 1, MAX_LEN, fp);
	if (ret > 0)
	{
		m_rgpDict[nNewDictId]->size = ret;
        int nCurDictId = m_nCurServingDictId;
		m_nCurServingDictId = nNewDictId;
        DropCurDict(nCurDictId);
	}
	else
	{
		delete m_rgpDict[nNewDictId];
		m_rgpDict[nNewDictId] = NULL;

		printf("load file[%s] faild\n",m_szDictPath);
		return -1;
	}

	return ret;
}

int CResource::DropCurDict(int nCurDictId)
{
    while (m_rgResourceTracker.CheckRscUsing(nCurDictId))
    {
        usleep(1000);
    }
    printf("Drop dict %d\n", nCurDictId);
	int ret = S_OK;
    if ( m_rgpDict[nCurDictId]!= NULL)
    {
		delete m_rgpDict[nCurDictId];
		m_rgpDict[nCurDictId] = NULL;
    }

    return ret;
}

int CResource::SwitchToNewDict()
{
	return (m_nCurServingDictId + 1) % MAX_DICT_BUF_NUM;
}

int CResource::GetValue(char *pVal, int *puSize)
{
    rschelper::ResourceTrackHelper helper(
            &m_rgResourceTracker, m_nCurServingDictId);

	if (m_rgpDict[m_nCurServingDictId] != NULL )
	{
		*puSize = m_rgpDict[m_nCurServingDictId]->size;
		memcpy(pVal,m_rgpDict[m_nCurServingDictId]->buf,MAX_LEN);
	}
	else
	{
		*puSize = 0;
		memcpy(pVal,0,MAX_LEN);
	}
	return 0;
}

int CResource::SetValue(char *pVal, int uSize)
{
    rschelper::ResourceTrackHelper helper(
            &m_rgResourceTracker, m_nCurServingDictId);

	if (pVal == NULL || uSize == 0)
	{
		printf("input value empty");
		return -1;
	}
	if (uSize > MAX_LEN)
	{
		printf("input value too long[%d]\n",uSize);
		return -1;
	}

	if (m_rgpDict[m_nCurServingDictId] != NULL )
	{
		m_rgpDict[m_nCurServingDictId]->size = uSize;
		memcpy(m_rgpDict[m_nCurServingDictId]->buf, pVal, uSize);
	}
	else
	{
		printf("dict is NULL");
		return -1;
	}

	return 0;
}
