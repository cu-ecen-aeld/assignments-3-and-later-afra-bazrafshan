#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <syslog.h>

int main(int argc, char* argv[])
{
    int mkdir_stat;

    openlog(NULL, 0, LOG_USER);

    if (argc != 3)
    {
        syslog(LOG_ERR, "Invalid number of arguments");
        //printf("Invalid number of arguments\n");
        return 1;
    }

    #if 1
    char *fileDir = argv[1];
    char *writeStr = argv[2];
    char buffer[50];
    int pos = 0;

    char* dir[10] = {0};
    int cnt =0;
    dir[cnt] = strtok(fileDir, "/");

    while(NULL != dir[cnt])
    {
        cnt+=1;
        dir[cnt] = strtok(NULL, "/");
    }

    for(int i=0; i<cnt; i++)
    {

        if( i != cnt-1)
        {

            pos += sprintf(buffer+pos,"%c",'/');
            pos += sprintf(buffer+pos,"%s",dir[i]);
            //printf("%s\n",buffer);
        }
        else
        {
            mkdir_stat =  mkdir(buffer, S_IRWXO);
            if( 0 != mkdir_stat)
            {
                syslog(LOG_ERR, "Can not make directory at %s",fileDir);
            }
            pos += sprintf(buffer+pos,"%c",'/');
            pos += sprintf(buffer+pos,"%s",dir[i]);
            //printf("%s\n",buffer);
            FILE *f;
            f = fopen(buffer,"wt");
            if(f != NULL)
            {
                if(0 == fputs(writeStr,f))
                {
                    syslog(LOG_ERR, "Can not write in file");
                }
                else
                {
                    syslog(LOG_DEBUG, "Writing %s to %s", writeStr, fileDir);
                }
                fclose(f);
            }
            else
            {
                syslog(LOG_ERR, "File not created");
            }
        }
    }
    closelog();
    
#endif
    return 0;
}