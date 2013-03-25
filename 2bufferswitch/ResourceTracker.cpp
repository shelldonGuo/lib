#include "ResourceTracker.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <string.h>
#define gettid() syscall(__NR_gettid)

using namespace rschelper;

const int MAX_THREADID = 65536;

ResourceTracker::ResourceTracker()
{
    m_nThreadTrackers = new uint8_t[MAX_THREADID];
    memset(m_nThreadTrackers, 0, sizeof(uint8_t) * MAX_THREADID);
}
ResourceTracker::~ResourceTracker()
{
    delete [] m_nThreadTrackers;
    m_nThreadTrackers = NULL;
}
void ResourceTracker::AddTracker(int rsc_id)
{
    int tid = gettid(); //¿ÉÓÃÓÚ¶àÏß³Ì
    if (tid < 0 || tid >= MAX_THREADID)
    {
        printf("thread id %d exceed!\n", tid);
        return;
    }
    if (rsc_id >= 255 || rsc_id < 0)
    {
        printf("rsc_id only support 0-254");
        return;
    }
    if (m_nThreadTrackers[tid] > 0)
    {
        printf("thread info not clear tid:%d", tid);
        return;
    }
    m_nThreadTrackers[tid] = rsc_id + 1;
}
void ResourceTracker::ClearTracker()
{
    int tid = gettid();
    if (tid < 0 || tid >= MAX_THREADID)
    {
        printf("thread id %d exceed!\n", tid);
        return;
    }
    m_nThreadTrackers[tid] = 0;
}
bool ResourceTracker::CheckRscUsing(int rsc_id)
{
    for (int i = 0; i < MAX_THREADID; ++i)
    {
        if (m_nThreadTrackers[i] == rsc_id + 1)
        {
            printf("rsc is in using!");
            return true;
        }
    }
    return false;
}





ResourceTrackHelper::ResourceTrackHelper(
        ResourceTracker *tracker, int rsc_id)
{
    m_pTracker = tracker ;
    m_pTracker->AddTracker(rsc_id);
}
ResourceTrackHelper::~ResourceTrackHelper()
{
    m_pTracker->ClearTracker();
}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
