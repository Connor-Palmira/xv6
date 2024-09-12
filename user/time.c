//Connor Palmira
//5007380152
//CS 370, Section 1001

//Inclusions
#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "kernel/memlayout.h"
#include "kernel/riscv.h"

/// @brief Prints the results of given command to given file
/// @param argc Number of args
/// @param argv Arg array
/// @return 0 if successful
int main(int argc, char *argv[])
{
    //Ends if not enough args
    if (argc < 2)
    {
        printf("Usage: time COMMAND\n");
        exit(0);
    }

    //Gets starting uptime
    int time1 = uptime();
    //Preps ending uptime
    int time2 = 0;
    //Forks process
    int pid = fork();
    //Waits if its the parent process
    if(pid > 0)
    {
        pid = wait((int *) 0);
    }
    //Does the command from the argv if its the child process
    else if(pid == 0)
    {
        char* arr[] = {argv[1], 0};
        time2 = exec(argv[1], arr);
        exit(0);
    }
    //Error message if something went wrong
    else
    {
        printf("fork() is unsuccessful\n");
    }

    //Gets end time
    time2 = uptime();

    //Calculates the subseconds
    int timeDifference = time2 - time1;
    int beforeDecimal = timeDifference / 100;
    int afterDecimal = (timeDifference % 100) * 1000;

    //Prints the subseconds
    fprintf(2, "Real-time: %d.%d\n", beforeDecimal, afterDecimal);
    
    //End of program
    exit(0);
}