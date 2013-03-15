#include <stdio.h>
#include <string.h>

/* [in] buf: ´ý·Ö¸îµÄ×Ö·û´®£¬»áÆÆ»µÔ­Ê¼×Ö·û´®£¬·Ö¸ô·û´¦»á±»ÖÃÎª '\0'
 * [in] tok_size: ×î´ó·Ö¸ôÊý
 * [out] toks: ¼ÇÂ¼·Ö¸ôºóµÃµ½µÄtok
 * return : ·µ»ØÊµ¼Ê·Ö¸ôÊý
 * ÀýÈç£º
 * aaa \t bbb
 * ·µ»Ø2£¬Á½¸ötok£ºaaa, bbb
 *
 * aaa \t bbb \t
 * ·µ»Ø3£¬3¸ötok£ºaaa, bbb, \n
 *
 */

int split(char* buf, char** toks, int tok_size)
{
    if(buf == NULL || buf[0] == 0)
    {
         return 0;
    }

    int size = 0;
    // char *strsep(char **stringp, const char *delim);
    // ÔÚstringpÖÐ²éÕÒdelimÖÐµÄ·Ö¸ô·û
    // Èç¹ûÕÒµ½£¬¾Í°ÑstringpµÄ·Ö¸ô·û´¦¸üÐÂÎª'\0'; stringpÖ¸Ïò·Ö¸ô·ûºóµÄÎ»ÖÃ
    // Èç¹ûÃ»ÓÐÕÒµ½£¬ÈÏÎªÕû¸östringpÎªÒ»¸ötok£¬stringpÉèÖÃÎªNULL
    // ·µ»ØµÚÒ»¸ötok£¬¼´Ô­Ê¼µÄstringp
    char* p = strsep(&buf, "\t");
    while(p != NULL && size < tok_size)
    {
         toks[size++] = p;
         p = strsep(&buf, "\t");
    }

    return size;
}
int main()
{
    const int max_length=1024*10;
	char line[max_length+1];
    char* toks[100];
    int tok_num,i;

    // input : "aaa\tbbb"
    // tok[0]: aaa
    // tok[1]: bbb
	while (fgets(line, max_length, stdin))
    {
		line[strlen(line)-1] = 0;
        if (! (tok_num=split(line, toks, 100)) )
        {
            printf("spilt line failed\n");
            continue;
        }
        printf("tok_num = [%d]\n",tok_num);
        for (i=0; i < tok_num; ++i)
            printf("toks[%d] : %s\n",i,toks[i]);
    }
    return 0;
}





















/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
