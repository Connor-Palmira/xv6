/* Connor Palmira, 5007380152, Project #5 B */

// Inclusions
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

//Function declarations
bool getArguments(int argc, char* argv[], unsigned int *count);
void* vehicleController(void * num);
void* lightController();

//Constants
const unsigned int VEHICLE_CNT_MIN = 10;
const unsigned int VEHICLE_CNT_MAX = 500;

const unsigned int LIGHT_WAIT = 4;
const unsigned int YELLOW_WAIT = 4;
const unsigned int TRANSIT_TIME_BASE = 1;
const unsigned int ARRIVAL_INTERVAL = 2;

const unsigned int NORTH = 0;
const unsigned int EAST = 1;
const unsigned int SOUTH = 2;
const unsigned int WEST = 3;

//Global variables
sem_t *sems;
char* dir[] = {"North", "East", "South", "West"};

//Main
int main(int argc, char* argv[])
{
    //Number of vehicles, given through command line
    unsigned int numVehicles = 0;

    //Exits if command line arguments fail
    if(getArguments(argc, argv, &numVehicles) == false) exit(0);

    //Prints header
    printf("CS 370 Project #5B -> Traffic Light Simulation Project\n");
    printf("  Vehicles: %d\n\n", numVehicles);

    //Allocates semaphores
    sems = malloc(4 * sizeof(sem_t));
    if(sems == NULL)
    {
        printf("Error when allocating semaphores\n");
        exit(0);
    }

    //Initializes semaphores
    for(int i = 0; i < 4; i++)
    {
        if(sem_init(&sems[i], 0, 0) == -1)
        {
            printf("Error when initializing semaphores\n");
            free(sems);
            exit(0);
        }
    }

    //Creates threads
    pthread_t threads[numVehicles + 1];
    int ids[numVehicles + 1];

    //Starts light controller thread
    ids[numVehicles] = numVehicles;
    int x = pthread_create(&threads[numVehicles], NULL, lightController, &ids[numVehicles]);
    if(x)
    {
        printf("Error when creating thread\n");
        exit(-1);
    }

    //Creates vehicle threads
    for(int i = 0; i < numVehicles; i++)
    {
        ids[i] = i;
        int num = rand() % 4;
        int x = pthread_create(&threads[i], NULL, vehicleController, &num);
        if(x)
        {
            printf("Error when creating thread\n");
            exit(-1);
        }

        sleep(ARRIVAL_INTERVAL);
    }

    //Waits for vehicles to finish, then stop lights
    for(int i = 0; i < numVehicles; i++) pthread_join(threads[i], NULL);
    pthread_cancel(threads[numVehicles]);

    //Deallocates semaphores
    for(int i = 0; i < 4; i++) sem_destroy(&sems[i]);
    free(sems);

    //Prints success message
    printf("\nAll vehicles successfully passed through the intersection\n");

    //End of program
    exit(0);
}

/// @brief Validates command line arguments, gives error if one is found
/// @param argc The number of arguments
/// @param argv Array of arguments
/// @param count Pointer to count to be passed by reference
/// @return Returns true if no errors, returns false otherwise
bool getArguments(int argc, char* argv[], unsigned int *count)
{
    //Checks if there are arguments
    if(argc <= 1)
    {
        printf("Usage: ./traffic -vc <vehicleCount>\n");
        return false;
    }

    //Checks for the correct amount of arguments
    if(argc != 3)
    {
        printf("Error, invalid command line options.\n");
        return false;
    }

    //Checks item count specifier
    if(strcmp(argv[1], "-vc") != 0)
    {
        printf("Error, invalid item count specifier.\n");
        return false;
    }

    //Checks if vehicle count is a number
    unsigned int vehicleCount = 0;
    for(int i = 0; argv[2][i] != '\0'; i++)
    {
        if(argv[2][i] < '0' || argv[2][i] > '9')
        {
            printf("Error, invalid vehicle count value.\n");
            return false;
        }
        else
        {
            vehicleCount *= 10;
            vehicleCount += (argv[2][i] - '0');
        }
    }

    *count = vehicleCount;

    //Checks if vehicle count is in range
    if(vehicleCount < VEHICLE_CNT_MIN || vehicleCount > VEHICLE_CNT_MAX)
    {
        printf("Error, vehicle count out of range.\n");
        return false;
    }

    //Returns true if no errors were found
    return true;
}

/// @brief Controls the vehicles that wait for the light be green
/// @return NULL
void* vehicleController(void * num)
{
    int direction = *(int*)num;
    //Print approaching message
    if(direction == NORTH || direction == SOUTH)
        printf("\u2195 Vehicle approaching from %s\n", dir[direction]);
    else
        printf("\u2194 Vehicle approaching from %s\n", dir[direction]);

    //Wait for light to be green
    int value;
    do
    {
        sem_getvalue(&sems[direction], &value);
    } while (value <= 0);
    
    //Print passing through message
    if(direction == NORTH || direction == SOUTH)
        printf("\u2195 Vehicle passing through from %s\n", dir[direction]);
    else
        printf("\u2194 Vehicle passing through from %s\n", dir[direction]);

    //Wait for vehicle to pass through the intersection
    sleep(TRANSIT_TIME_BASE + usleep(rand() % 5000));

    //Release, light turns red
    do
    {
        sem_getvalue(&sems[direction], &value);
    } while (value > 0);

    //End of function
    return NULL;
}

/// @brief Controls the lights for traffic based on seconds
/// @return NULL, but shouldn't get there
void* lightController()
{
    while(true)
    {
        //Print green light message
        printf("\033[0;92mGreen light for North-South\033[0m\n");
        //Turn north-south lights green-yellow
        sem_post(&sems[NORTH]);
        sem_post(&sems[SOUTH]);
        //Keep light green for LIGHT_WAIT seconds
        sleep(LIGHT_WAIT);
        //Print yellw light message
        printf("\033[0;93mYellow light for North-South\033[0m\n");
        //Wait for additional YELLOW_WAIT seconds
        sleep(YELLOW_WAIT);
        //Turn light red for north-south
        sem_trywait(&sems[NORTH]);
        sem_trywait(&sems[SOUTH]);
        printf("\033[0;31mRed light for North-South\033[0m\n");

        //Print green light message for east-west
        printf("\033[0;92mGreen light for East-West\033[0m\n");
        //Turn east-west lights green
        sem_post(&sems[EAST]);
        sem_post(&sems[WEST]);
        //Keep light green for LIGHT_WAIT seconds
        sleep(LIGHT_WAIT);
        //Print yellow lights message
        printf("\033[0;93mYellow light for East-West\033[0m\n");
        //Wait for additional YELLOW_WAIT seconds
        sleep(YELLOW_WAIT);
        //Turn light red for east-west direction
        sem_trywait(&sems[EAST]);
        sem_trywait(&sems[WEST]);
        printf("\033[0;31mRed light for East-West\033[0m\n");
    }

    //End of function, shouldn't be reached
    return NULL;
}