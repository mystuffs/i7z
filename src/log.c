#include <stdbool.h>
#include <stdio.h>
#include <inttypes.h>
#include "i7z.h"

extern struct program_options prog_options;

char* CPU_FREQUENCY_LOGGING_FILE_single="cpu_freq_log.txt";
char* CPU_FREQUENCY_LOGGING_FILE_dual="cpu_freq_log_dual_%d.txt";
char* CSTATE_LOGGING_FILE_single="cpu_cstate_log.txt";
char* CSTATE_LOGGING_FILE_dual="cpu_cstate_log_dual_%d.txt";

void debug (bool quiet, char* message) {
    if (quiet) {
        return;
    }
    printf("i7z DEBUG: %s\n", message);
}

void error (char* message) {
    debug(false, message);
}

void print_model(bool quiet, int model, int extended_model) {
    switch (extended_model) {
        case 0x1:
            debug(quiet, "Detected i3/i5/i7 Nehalem");
            break;
        case 0x2:
            switch (model) {
                case 0x5:
                    debug(quiet, "Detected i3/i5/i7 Westmere");
                    break;
                case 0xA:
                case 0xD:
                    debug(quiet, "Detected i3/i5/i7 Sandy Bridge");
                    break;
                default:
                    debug(quiet, "Detected Xeon");
            }
            break;
        case 0x3:
            switch (model) {
                case 0xA:
                    debug(quiet, "Detected i3/i5/i7 Ivy Bridge");
                    break;
                case 0xC:
                    debug(quiet, "Detected i3/i5/i7 Haswell");
                    break;
            }
    }
}

// old log stuff TODO: condense this mess

FILE *fp_log_file_freq;
FILE *fp_log_file_freq_1, *fp_log_file_freq_2;

FILE *fp_log_file_Cstates;
FILE *fp_log_file_Cstates_1, *fp_log_file_Cstates_2;

void logOpenFile_single()
{
    if(prog_options.logging==1) {
        fp_log_file_freq = fopen(CPU_FREQUENCY_LOGGING_FILE_single,"w");
        fp_log_file_Cstates = fopen(CSTATE_LOGGING_FILE_single,"w");
    } else if(prog_options.logging==2) {
        fp_log_file_freq = fopen(CPU_FREQUENCY_LOGGING_FILE_single,"a");
        fp_log_file_Cstates = fopen(CSTATE_LOGGING_FILE_single,"a");
    }
}

void logCloseFile_single()
{
    if(prog_options.logging!=0){
        if(prog_options.logging==2)
        fprintf(fp_log_file_freq,"\n");
        //the above line puts a \n after every freq is logged.
        fclose(fp_log_file_freq);

        fprintf(fp_log_file_Cstates,"\n");
        //the above line puts a \n after every CSTATE is logged.
        fclose(fp_log_file_Cstates);

    }
}

// For dual socket make filename based on the socket number
void logOpenFile_dual(int socket_num)
{
    char str_file1[100];
    snprintf(str_file1,100,CPU_FREQUENCY_LOGGING_FILE_dual,socket_num);

    char str_file2[100];
    snprintf(str_file2,100,CSTATE_LOGGING_FILE_dual,socket_num);

    if(socket_num==0){
        if(prog_options.logging==1)
            fp_log_file_freq_1 = fopen(str_file1,"w");
        else if(prog_options.logging==2)
            fp_log_file_freq_1 = fopen(str_file1,"a");
    }
    if(socket_num==1){
        if(prog_options.logging==1)
            fp_log_file_freq_2 = fopen(str_file1,"w");
        else if(prog_options.logging==2)
            fp_log_file_freq_2 = fopen(str_file1,"a");
    }

    if(socket_num==0){
        if(prog_options.logging==1)
            fp_log_file_Cstates_1 = fopen(str_file2,"w");
        else if(prog_options.logging==2)
            fp_log_file_Cstates_1 = fopen(str_file2,"a");
    }
    if(socket_num==1){
        if(prog_options.logging==1)
            fp_log_file_Cstates_2 = fopen(str_file2,"w");
        else if(prog_options.logging==2)
            fp_log_file_Cstates_2 = fopen(str_file2,"a");
    }
}

void logCloseFile_dual(int socket_num)
{
    if(socket_num==0){
        if(prog_options.logging!=0){
            if(prog_options.logging==2)
                fprintf(fp_log_file_freq_1,"\n");
                //the above line puts a \n after every freq is logged.
            fclose(fp_log_file_freq_1);
        }
    }
    if(socket_num==1){
        if(prog_options.logging!=0){
            if(prog_options.logging==2)
                fprintf(fp_log_file_freq_2,"\n");
                //the above line puts a \n after every freq is logged.
            fclose(fp_log_file_freq_2);
        }
    }


    if(socket_num==0){
        if(prog_options.logging!=0){
            if(prog_options.logging==2)
                fprintf(fp_log_file_Cstates_1,"\n");
                //the above line puts a \n after every freq is logged.
            fclose(fp_log_file_Cstates_1);
        }
    }
    if(socket_num==1){
        if(prog_options.logging!=0){
            if(prog_options.logging==2)
                fprintf(fp_log_file_Cstates_2,"\n");
                //the above line puts a \n after every freq is logged.
            fclose(fp_log_file_Cstates_2);
        }
    }
}


void logCpuFreq_single(float value)
{
    //below when just logging
    if(prog_options.logging==1) {
        fprintf(fp_log_file_freq,"%f\n",value); //newline, replace \n with \t to get everything separated with tabs
    }
    //below when appending
    if(prog_options.logging==2) {
        fprintf(fp_log_file_freq,"%f\t",value);
    }
}

void logCpuFreq_single_c(char* value)
{
    //below when just logging
    if(prog_options.logging==1) {
        fprintf(fp_log_file_freq,"%s\n",value); //newline, replace \n with \t to get everything separated with tabs
    }
    //below when appending
    if(prog_options.logging==2) {
        fprintf(fp_log_file_freq,"%s\t",value);
    }
}

void logCpuFreq_single_d(int value)
{
    //below when just logging
    if(prog_options.logging==1) {
        fprintf(fp_log_file_freq,"%d\n",value); //newline, replace \n with \t to get everything separated with tabs
    }
    //below when appending
    if(prog_options.logging==2) {
        fprintf(fp_log_file_freq,"%d\t",value);
    }
}

// fix for issue 48, suggested by Hakan
void logCpuFreq_single_ts(struct timespec  *value) //HW use timespec to avoid floating point overflow
{
    //below when just logging
    if(prog_options.logging==1) {
        fprintf(fp_log_file_freq,"%ld.%.9ld\n",value->tv_sec,value->tv_nsec); //newline, replace \n with \t to get everything separated with tabs
    }
    //below when appending
    if(prog_options.logging==2) {
        fprintf(fp_log_file_freq,"%ld.%.9ld\t",value->tv_sec,value->tv_nsec);
    }
}


void logCpuFreq_dual(float value,int socket_num)
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_1,"%f\n",value); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
            fprintf(fp_log_file_freq_1,"%f\t",value);
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_2,"%f\n",value); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
            fprintf(fp_log_file_freq_2,"%f\t",value);
    }
}

void logCpuFreq_dual_c(char* value,int socket_num)
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_1,"%s\n",value); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
            fprintf(fp_log_file_freq_1,"%s\t",value);
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_2,"%s\n",value); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
            fprintf(fp_log_file_freq_2,"%s\t",value);
    }
}

void logCpuFreq_dual_d(int value,int socket_num)
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_1,"%d\n",value); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
            fprintf(fp_log_file_freq_1,"%d\t",value);
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_2,"%d\n",value); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
            fprintf(fp_log_file_freq_2,"%d\t",value);
    }
}

void logCpuFreq_dual_ts(struct timespec  *value, int socket_num) //HW use timespec to avoid floating point overflow
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_1,"%ld.%.9ld\n",value->tv_sec,value->tv_nsec); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
             fprintf(fp_log_file_freq_1,"%ld.%.9ld\t",value->tv_sec,value->tv_nsec);
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging==1)
            fprintf(fp_log_file_freq_2,"%ld.%.9ld\n",value->tv_sec,value->tv_nsec); //newline, replace \n with \t to get everything separated with tabs

        //below when appending
        if(prog_options.logging==2)
             fprintf(fp_log_file_freq_2,"%ld.%.9ld\t",value->tv_sec,value->tv_nsec);
    }
}

void logCpuCstates_single(float value)
{
    //below when just logging
    if(prog_options.logging != 0) {
        fprintf(fp_log_file_Cstates,"%f",value); //newline, replace \n with \t to get everything separated with tabs
    }
}

void logCpuCstates_single_c(char* value)
{
    //below when just logging
    if(prog_options.logging != 0) {
        fprintf(fp_log_file_Cstates,"%s",value); //newline, replace \n with \t to get everything separated with tabs
    }
}

void logCpuCstates_single_d(int value)
{
    //below when just logging
    if(prog_options.logging != 0) {
        fprintf(fp_log_file_Cstates,"%d",value); //newline, replace \n with \t to get everything separated with tabs
    }
}

// fix for issue 48, suggested by Hakan
void logCpuCstates_single_ts(struct timespec  *value) //HW use timespec to avoid floating point overflow
{
    //below when just logging
    if(prog_options.logging != 0) {
        fprintf(fp_log_file_Cstates,"%ld.%.9ld",value->tv_sec,value->tv_nsec); //newline, replace \n with \t to get everything separated with tabs
    }
}

void logCpuCstates_dual(float value,int socket_num)
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_1,"%f",value); //newline, replace \n with \t to get everything separated with tabs
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_2,"%f",value); //newline, replace \n with \t to get everything separated with tabs
    }
}

void logCpuCstates_dual_c(char* value,int socket_num)
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_1,"%s",value); //newline, replace \n with \t to get everything separated with tabs
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_2,"%s",value); //newline, replace \n with \t to get everything separated with tabs
    }
}

void logCpuCstates_dual_d(int value,int socket_num)
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_1,"%d",value); //newline, replace \n with \t to get everything separated with tabs
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_2,"%d",value); //newline, replace \n with \t to get everything separated with tabs
    }
}

void logCpuCstates_dual_ts(struct timespec  *value, int socket_num) //HW use timespec to avoid floating point overflow
{
    if(socket_num==0){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_1,"%ld.%.9ld",value->tv_sec,value->tv_nsec); //newline, replace \n with \t to get everything separated with tabs
    }
    if(socket_num==1){
        //below when just logging
        if(prog_options.logging != 0)
            fprintf(fp_log_file_Cstates_2,"%ld.%.9ld",value->tv_sec,value->tv_nsec); //newline, replace \n with \t to get everything separated with tabs
    }
}


