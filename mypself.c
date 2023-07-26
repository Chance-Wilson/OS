

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <ctype.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
//#define FORMAT "%5s %5s %s %s %s %s %s %8s %s"
#define COLFORMAT "%s %s %s \t%8s %8s %s\n"

int main(int argc, char** argv)
{
 DIR *dir;
 struct dirent *ent;
 int i, fd_self, fd;
 unsigned long time, stime;
 char flag;
 char cmd[256],path[256],time_s[256];
 FILE* file;

 dir = opendir("/proc");
 fd_self = open("/proc/self/fd/0",O_RDONLY);
 printf(COLFORMAT,"PID","PPID","LWP","STIME", "TIME","CMD");

//read every entry in the directory until 
//the Null character is found signifying the 
//end of the directory
 while ((ent = readdir(dir))!= NULL)
 {
    flag = 1;
   
    for (i=0; ent->d_name[i];i++)
    {
        int LWP = i;
        if(!isdigit(ent->d_name[i]))
        {
            flag = 0;
            break;
        }
        if(flag)
        {
            //if the flag is valid, you have found a process
            //that you can start extracting information from

            //grab pid and ppid first to 
            //know what file to target
            int pid = getpid();
            int ppid = getppid();
            //code for time and stime
            sprintf(path, "/proc/%d/fd/0", pid);
            fd = open(path,O_RDONLY);
            sprintf(path, "/proc/%d/stat", pid);
            file = fopen(path,"r");
            fscanf(file, "%lu", &time);
            fscanf(file,"%lu", &stime);
            time = (int)((double)(time+stime)/sysconf(_SC_CLK_TCK));
            //code for CMD Line
            sprintf(path, "/proc/%d/fd/0", pid);
            fd = open(path,O_RDONLY);
            sprintf(path, "/proc/%d/cmdline",pid);
            file = fopen(path,"r");
            fscanf(file,"%s", &cmd);
            //code to print Process info
            printf("%d %d %d \t%lu %lu %s\n", pid, ppid, LWP, time_s, time, cmd);
            fclose(file);
        }
        close(fd);
    }
 }
 close(fd_self);
 return 0;
}