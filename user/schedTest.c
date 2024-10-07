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
#include "kernel/spinlock.h"
#include "kernel/proc.h"

/*
* Returns 0 if the number is prime, 1 otherwise
*/
int checkPrime(int num)
{
    if(num < 2)
        return 1;

    for(int i = 2; i <= (num / 2); i++)
    {
        if(num % i == 0)
            return 1;
    }

    return 0;
}

int main()
{
    int MAX_PROCESS = 8;
    int LIMIT = 100000; // SWITCH BACK TO 1 MIL FOR FINAL
    int isParent = 0; //0 if parent function
    debug(1); //switch to debug ON
    int pid = 0;

    for(int i = MAX_PROCESS; i > 0; i--)
    {
        int num = 0;
        if(i % 2 == 0)
        {
            num = i;
        }
        else
        {
            num = i + 1;
        }
        num -= 2;

        if(isParent == 0)
        {
            printf("Creating a Child Process with runLength = %d\n", num);
            pid = forkBQ(num);
        }
        
        if(pid == 0)
        {
            isParent = 1;
            break;
        }
    }

    int numTwinPrimes = 0;
    for(int i = 2; i < LIMIT && isParent != 0; i++)
        if(checkPrime(i) == 0 && checkPrime(i+2) == 0)
            numTwinPrimes++;

    if(isParent != 0)
        printf("Child pid = %d, total twin primes = %d\n", getpid(), numTwinPrimes);

    if(isParent == 0)
    {
        for(int i = 0; i < MAX_PROCESS; i++)
            pid = wait((int *) 0);
        printf("Done\n");

        debug(0); // Switches to debug OFF
    }

    exit(0);
}