#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "ResourceTracker.h"
#include "Resource.h"


/** 文件是否存在 */
static bool _is_fileexist( const char* filename )
{
    struct stat fst;
    if( stat( filename, &fst ) ==0 )
        return true;
    return false;
}

int main()
{
    int size;
    char buf[MAX_LEN];
    char* load_flag = "load_flag";
    CResource rsc;
    rsc.Initialize("./test.dat");
    while (1)
    {
        rsc.GetValue(buf, &size);
        printf("size:[%d]   buf:[%s]\n", size, buf);
        sleep(5);
        if ( _is_fileexist(load_flag) )
        {
            // touch load_flag
            // reload ./test.dat 
            rsc.Reload();
            remove(load_flag);
        }
    }
    return 0;
}



















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
