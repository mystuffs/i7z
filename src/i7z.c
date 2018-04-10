//i7z.c
/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2009 Abhishek Jaiantilal
 *
 *   Under GPL v2
 *
 * ----------------------------------------------------------------------- */

#include <memory.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include "getopt.h"
#include "i7z.h"

struct program_options prog_options;

int Single_Socket();
int Dual_Socket();

int socket_0_num=0, socket_1_num=1;
void atexit_runsttysane()
{
    printf("Quitting i7z\n");
    system("stty sane");
}

void modprobing_msr()
{
    system("modprobe msr");
}

#define MAX_FILENAME_LENGTH 1000

int main (int argc, char **argv)
{
    atexit(atexit_runsttysane);

    //char log_file_name[MAX_FILENAME_LENGTH], log_file_name2[MAX_FILENAME_LENGTH+3];
    prog_options.logging=0; //0=no logging, 1=logging, 2=appending
    prog_options.quiet = false;
    prog_options.use_ncurses = true;

    struct cpu_heirarchy_info chi;
    struct cpu_socket_info socket_0={.max_cpu=0, .socket_num=0, .processor_num={-1,-1,-1,-1,-1,-1,-1,-1}};
    struct cpu_socket_info socket_1={.max_cpu=0, .socket_num=1, .processor_num={-1,-1,-1,-1,-1,-1,-1,-1}};

    
    int c;
    bool presupplied_socket_info = false;

    static struct option long_options[]=
    {
        {"write", required_argument, 0, 'w'},
        {"socket0", required_argument,0 ,'z'},
        {"socket1", required_argument,0 ,'y'},
        {"logfile", required_argument,0,'l'},
        {"help", no_argument, 0, 'h'},
        {"nogui", no_argument, 0, 'n'},
        {"quiet", no_argument, 0, 'q'}
    };

    prog_options.logging = 0;
    while(1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv,"w:z:y:l:hn", long_options, &option_index);
        if (c==-1)
            break;
        switch(c)
        {
            case 'z':
                socket_0_num = atoi(optarg);
                presupplied_socket_info = true;
                printf("Socket_0 information will be about socket %d\n", socket_0.socket_num);
                break;
            case 'y':
                socket_1_num = atoi(optarg);
                presupplied_socket_info = true;
                printf("Socket_1 information will be about socket %d\n", socket_1.socket_num);
                break;
            case 'w':
                //printf("write options specified %s\n", optarg);
                if (strcmp("l",optarg)==0)
                {
                    prog_options.logging = 1;
                    printf("Logging is ON and set to replace\n");
                }
                if (strcmp("a",optarg)==0)
                {
                    prog_options.logging = 2;
                    printf("Logging is ON and set to append\n");
                }
                break;
            case 'q':
                prog_options.quiet = true;
                break;
                /*
            case 'l':
                strncpy(log_file_name, optarg, MAX_FILENAME_LENGTH-3);
                strcpy(log_file_name2, log_file_name);
                strcat(log_file_name2, "_%d");
                CPU_FREQUENCY_LOGGING_FILE_single = log_file_name;
                CPU_FREQUENCY_LOGGING_FILE_dual = log_file_name2;
                printf("Logging frequencies to %s for single sockets, %s for dual sockets(0,1 for multiple sockets)\n", prog_options.CPU_FREQUENCY_LOGGING_FILE_single, prog_options.CPU_FREQUENCY_LOGGING_FILE_dual);
                break;
                */
            case 'n':
                prog_options.use_ncurses = false;
                printf("Not Spawning the GUI\n");
                break;

            case 'h':
                printf("\ni7z Tool Supports the following functions:\n");
                printf("Append to a log file:  ");
                printf("%c[%d;%d;%dm./i7z --write a ", 0x1B,1,31,40);
                printf("%c[%dm[OR] ",0x1B,0);
                printf("%c[%d;%d;%dm./i7z -w a\n", 0x1B,1,31,40);
                printf("%c[%dm",0x1B,0);

                printf("Replacement instead of Append:  ");
                printf("%c[%d;%d;%dm./i7z --write l ", 0x1B,1,31,40);
                printf("%c[%dm[OR]", 0x1B,0);
                printf(" %c[%d;%d;%dm./i7z -w l\n", 0x1B,1,31,40);
                printf("%c[%dm",0x1B,0);

                //printf("Default log file name is %s (single socket) or %s (dual socket)\n", CPU_FREQUENCY_LOGGING_FILE_single, CPU_FREQUENCY_LOGGING_FILE_dual);
                //printf("Specifying a different log file: ");
                //printf("%c[%d;%d;%dm./i7z --logfile filename ", 0x1B,1,31,40);
                printf("%c[%dm[OR] ", 0x1B,0);
                printf("%c[%d;%d;%dm./i7z -l filename\n", 0x1B,1,31,40);
                printf("%c[%dm",0x1B,0);
                printf("Specifying a particular socket to print: %c[%d;%d;%dm./i7z --socket0 X \n", 0x1B,1,31,40);
                printf("%c[%dm",0x1B,0);
                printf("In order to print to a second socket use: %c[%d;%d;%dm./i7z --socket1 X \n", 0x1B,1,31,40);
                printf("%c[%dm",0x1B,0);
                printf("To turn the ncurses GUI off use: %c[%d;%d;%dm./i7z --nogui\n", 0x1B, 1, 31, 40);
                printf("%c[%dm",0x1B,0);
                printf("Example: To print for two sockets and also change the log file %c[%d;%d;%dm./i7z --socket0 0 --socket1 1 -logfile /tmp/logfilei7z -w l\n", 0x1B, 1, 31, 40);
                printf("%c[%dm",0x1B,0);

                exit(1);
                break;
        }
    }

    Print_Information_Processor (&prog_options.proc_version.nehalem, &prog_options.proc_version.sandy_bridge, &prog_options.proc_version.ivy_bridge, &prog_options.proc_version.haswell);

    Test_Or_Make_MSR_DEVICE_FILES ();
    modprobing_msr();

    construct_CPU_Hierarchy_info(&chi);
    construct_sibling_list(&chi);
    print_CPU_Hierarchy(chi);
    construct_socket_information(&chi, &socket_0, &socket_1, socket_0_num, socket_1_num);
    print_socket_information(&socket_0);
    print_socket_information(&socket_1);

    if (!presupplied_socket_info){
        if (socket_0.max_cpu>0 && socket_1.max_cpu>0) {
            //Path for Dual Socket Code
            printf("i7z DEBUG: Dual Socket Detected\n\r");
            //Dual_Socket(&prog_options);
            Dual_Socket();
        } else {
            //Path for Single Socket Code
            printf("i7z DEBUG: Single Socket Detected\n\r");
            //Single_Socket(&prog_options);
            Single_Socket();
        }
    } else {
        Dual_Socket();
    }
    return(1);
}
