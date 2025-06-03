#ifndef CREW_H
#define CREW_H

#define NUM_CREW 50


struct Flight {
    char flightID[10];
    char origin[30];
    char destination[30];
    char aircraftType[20];
    int priority;
    int runway;
    int departureTime;
    int arrivalTime;
    int assignedCrewID;
};

struct Crew {
    int totalHoursWorked;
    int lastRestTime;
    int isAvailable;
};

void initializeCrew(void);
void allocateCrew(struct Flight flights[], int len, int airportOpeningTime);
int calculateFlightDuration(int departureTime, int arrivalTime);

#endif