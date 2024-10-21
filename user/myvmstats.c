//Project #4

//Inclusions
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/vmstat.h"

int main()
{
    int SIZE = 32768; // 2^15

    struct vmstat stats; // Struct for memory info
    int status = vmstats(&stats); // Gets memory info

    //Returns with error message if error
    if(status != 0)
    {
        printf("Error with vmstats\n");
        exit(0);
    }

    //Prints memory info
    printf("Connor Palmira, xv6 Memory Stats\n");
    printf("Total memory: %ld\n", stats.total_memory);
    printf("Free memory: %ld\n", stats.free_memory);
    printf("Used memory: %ld\n", stats.used_memory);
    printf("Resident pages memory: %ld\n", stats.resident_pages);
    printf("Process memory: %ld\n\n", stats.process_memory);

    //Allocates 2^15 integers of memory with malloc()
    int* ptr = 0;
    ptr = (int*) malloc(SIZE * sizeof(int));

    //Returns with error message if error
    if(ptr == 0)
    {
        printf("Error with malloc\n");
        exit(0);
    }

    //If no error, prints success for memory allocation
    printf("Memory successfully allocated using malloc\n\n");

    //Gets new memory info
    status = vmstats(&stats);

    //Returns with error message if error
    if(status != 0)
    {
        printf("Error with vmstats\n");
        exit(0);
    }

    //Prints new memory info
    printf("Connor Palmira, xv6 Memory Stats\n");
    printf("Total memory: %ld\n", stats.total_memory);
    printf("Free memory: %ld\n", stats.free_memory);
    printf("Used memory: %ld\n", stats.used_memory);
    printf("Resident pages memory: %ld\n", stats.resident_pages);
    printf("Process memory: %ld\n", stats.process_memory);

    //Deallocation
    free(ptr);

    //End of program
    exit(0);
}