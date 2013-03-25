#ifndef  __RESOURCETRACKER_H_
#define  __RESOURCETRACKER_H_

typedef unsigned char uint8_t;

namespace rschelper
{
    class ResourceTracker
    {
        public:
            ResourceTracker();
            ~ResourceTracker();
            bool CheckRscUsing(int rsc_id);
            friend class ResourceTrackHelper;
        private:
            void AddTracker(int rsc_id);
            void ClearTracker();
            uint8_t *m_nThreadTrackers;
    };


    class ResourceTrackHelper
    {
        public:
            ResourceTrackHelper(ResourceTracker *tracker, int rsc_id);
            ~ResourceTrackHelper();
        private:
            ResourceTracker *m_pTracker;
    };
}














#endif 

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
