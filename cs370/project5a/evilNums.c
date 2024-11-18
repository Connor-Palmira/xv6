/* Connor Palmira, 5007380152, Project #5 A */

// Inclusions
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h>
#include <errno.h>

//Function declarations
bool getArguments(int argc, char* argv[]);
void* findEvilNumbersCnt1();
void* findEvilNumbersCnt2();

//Spinlock definition
pthread_mutex_t lock;

//Constants
const unsigned long MIN_THREADS = 1;
const unsigned long MAX_THREADS = 24;
const unsigned long MIN_LIMIT = 100;
const unsigned long BLOCK = 5000;

//Global variables
unsigned long currentIndex = 1;
unsigned long userLimit = 0;
unsigned long evilNumberCount = 0;
unsigned long odiousNumberCount = 0;

//Main
int main(int argc, char* argv[])
{
    //Exits if command line arguments fail
    if(getArguments(argc, argv) == false) exit(0);


    //Gets thread count
    unsigned threadCount = 0;
    for(int i = 0; argv[2][i] != '\0'; i++)
    {
        threadCount *= 10;
        threadCount += (argv[2][i] - '0');
    }

    //Gets number limit
    for(int i = 0; argv[4][i] != '\0'; i++)
    {
        userLimit *= 10;
        userLimit += (argv[4][i] - '0');
    }

    //Prints header
    printf("CS 370 - Project #5-A\n");
    printf("Evil/Odious Numbers Program\n\n");

    //Prints info
    printf("Hardware Cores: %ld\n", sysconf(_SC_NPROCESSORS_ONLN));
    printf("Thread Count:   %d\n", threadCount);
    printf("Numbers Limit:  %ld\n\n", userLimit);

    //Prints running info
    printf("Please wait. Running...\n\n");

    //Calls function 1 with threads
    if(argv[6][0] == 1)
    {
        pthread_t threads[threadCount];
        int ids[8];

        for(int i = 0; i < threadCount; i++)
        {
            ids[i] = i;
            int x = pthread_create(&threads[i], NULL, findEvilNumbersCnt1, &ids[i]);
            if(x)
            {
                printf("Error when creating thread\n");
                exit(-1);
            }
        }

        for(int i = 0; i < threadCount; i++)
            pthread_join(threads[i], NULL);
    }
    //Calls function 2 with threads
    else
    {
        pthread_t threads[threadCount];
        int ids[8];

        for(int i = 0; i < threadCount; i++)
        {
            ids[i] = i;
            int x = pthread_create(&threads[i], NULL, findEvilNumbersCnt2, &ids[i]);
            if(x)
            {
                printf("Error when creating thread\n");
                exit(-1);
            }
        }

        for(int i = 0; i < threadCount; i++)
            pthread_join(threads[i], NULL);
    }

    //Prints evil and odious numbers
    printf("Evil/Odious Numbers Results\n");
    printf("  Evil Number Count:   %ld\n", evilNumberCount);
    printf("  Odious Number Count: %ld\n", odiousNumberCount);

    //End of program
    exit(0);
}

/// @brief Validates command line arguments, gives error if one is found
/// @param argc The number of arguments
/// @param argv Array of arguments
/// @return Returns true if no errors, returns false otherwise
bool getArguments(int argc, char* argv[])
{
    //Checks if there are arguments
    if(argc <= 1)
    {
        printf("Usage: ./evilNums -t <threadCount> -l <limitValue> -f <1|2>\n");
        return false;
    }

    //Checks for the correct amount of arguments
    if(argc != 7)
    {
        printf("Error, invalid command line options.\n");
        return false;
    }

    //Checks thread count specifier
    if(strcmp(argv[1], "-t") != 0)
    {
        printf("Error, invalid thread count specifier.\n");
        return false;
    }

    //Checks limit specifier
    if(strcmp(argv[3], "-l") != 0)
    {
        printf("Error, invalid limit specifier.\n");
        return false;
    }

    //Checks function option specifier
    if(strcmp(argv[5], "-f") != 0)
    {
        printf("Error, invalid function option specifier.\n");
        return false;
    }

    //Checks if thread count is a number
    unsigned long threadCount = 0;
    for(int i = 0; argv[2][i] != '\0'; i++)
    {
        if(argv[2][i] < '0' || argv[2][i] > '9')
        {
            printf("Error, invalid thread count value.\n");
            return false;
        }
        else
        {
            threadCount *= 10;
            threadCount += (argv[2][i] - '0');
        }
    }

    //Checks if thread count is in range
    if(threadCount < MIN_THREADS || threadCount > MAX_THREADS)
    {
        printf("Error, thread count out of range.\n");
        return false;
    }

    //Checks if limit value is a number
    unsigned long limit = 0;
    for(int i = 0; argv[4][i] != '\0'; i++)
    {
        if(argv[4][i] < '0' || argv[4][i] > '9')
        {
            printf("Error, invalid limit value.\n");
            return false;
        }
        else
        {
            limit *= 10;
            limit += (argv[4][i] - '0');
        }
    }

    //Checks if limit value is in range
    if(limit < MIN_LIMIT)
    {
        printf("Error, limit must be > 100.\n");
        return false;
    }

    //Checks if function option is a number
    unsigned long funct = 0;
    for(int i = 0; argv[6][i] != '\0'; i++)
    {
        if(argv[6][i] < '0' || argv[6][i] > '9')
        {
            printf("Error, invalid function option value.\n");
            return false;
        }
        else
        {
            funct *= 10;
            funct += (argv[6][i] - '0');
        }
    }

    //Checks if function option is in range
    if(funct != 1 && funct != 2)
    {
        printf("Error, function option must be 1 or 2\n");
        return false;
    }

    //Returns true if no errors were found
    return true;
}


/// @brief Finds count of evil and odious numbers, updates every time one is found
/// @return Returns null pointer
void* findEvilNumbersCnt1()
{
    //Runs until no numbers left
    while(currentIndex <= userLimit)
    {
        //Gets index in lock
        pthread_mutex_lock(&lock);
        int index = currentIndex;
        currentIndex += BLOCK;
        pthread_mutex_unlock(&lock);

        //Checks all numbers and adds to count
        for(int i = index; i < index + BLOCK && i <= userLimit; i++)
        {
            int num = i;
            int numOnes = 0;
            while(num > 0)
            {
                numOnes += (num % 2);
                num /= 2;
            }
            
            //Adds to count in lock
            pthread_mutex_lock(&lock);
            if(numOnes % 2 == 0) evilNumberCount += 1;
            else odiousNumberCount += 1;
            pthread_mutex_unlock(&lock);
        }
    }

    //End of function
    return NULL;
}

/// @brief Finds count of evil and odious numbers, updates at end of block
/// @return Returns null pointer
void* findEvilNumbersCnt2()
{
    //Runs until no numbers left
    while(currentIndex <= userLimit)
    {
        //Gets index in lock
        pthread_mutex_lock(&lock);
        int index = currentIndex;
        currentIndex += BLOCK;
        pthread_mutex_unlock(&lock);
        int evilNumsFound = 0;
        int odiNumsFound = 0;

        //Checks all numbers in block
        for(int i = index; i < index + BLOCK && i <= userLimit; i++)
        {
            int num = i;
            int numOnes = 0;
            while(num > 0)
            {
                numOnes += (num % 2);
                num /= 2;
            }

            if(numOnes % 2 == 0) evilNumsFound += 1;
            else odiNumsFound += 1;
        }

        //Adds to count in lock
        pthread_mutex_lock(&lock);
        evilNumberCount += evilNumsFound;
        odiousNumberCount += odiNumsFound;
        pthread_mutex_unlock(&lock);
    }

    //End of function
    return NULL;
}