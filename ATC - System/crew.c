#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crew.h"

static struct Crew crew[NUM_CREW];

void initializeCrew(void) {
    for (int i = 0; i < NUM_CREW; i++) {
        crew[i].totalHoursWorked = 0;
        crew[i].lastRestTime = 0;
        crew[i].isAvailable = 1;
    }
}

int calculateFlightDuration(int departureTime, int arrivalTime) {
    int departureMinutes = (departureTime / 100) * 60 + (departureTime % 100);
    int arrivalMinutes = (arrivalTime / 100) * 60 + (arrivalTime % 100);
    
    if (arrivalMinutes < departureMinutes) {
        arrivalMinutes += 24 * 60; 
    }
    
    int duration = arrivalMinutes - departureMinutes;
    return duration;
}

void allocateCrew(struct Flight flights[], int len, int airportOpeningTime) {
    
    if (flights == NULL || len <= 0) {

        return;
    }
    if (airportOpeningTime < 0 || airportOpeningTime > 2359 || (airportOpeningTime % 100) > 59) {
       
        return;
    }

 
    FILE *f = fopen("flights.bin", "rb+");
    if (f == NULL) {
        return;
    }

    int currentTime = airportOpeningTime;
    int crewCount = NUM_CREW;

    for (int i = 0; i < len; i++) {
        int assigned = 0;
        flights[i].assignedCrewID = 0;

        if (strcmp(flights[i].origin, "XYZ Intl") == 0) {
            int flightDurationMinutes = calculateFlightDuration(flights[i].departureTime, flights[i].arrivalTime);
            int departureTimeMinutes = (flights[i].departureTime / 100) * 60 + (flights[i].departureTime % 100);

            for (int j = 0; j < crewCount; j++) {
                if (!crew[j].isAvailable && crew[j].lastRestTime <= departureTimeMinutes) {
                    crew[j].isAvailable = 1;
                    crew[j].totalHoursWorked = 0;
    
                }
            }
    
            int bestCrew = -1;
            int minHours = 13; 
            for (int j = 0; j < crewCount; j++) {
                if (crew[j].isAvailable &&
                    crew[j].totalHoursWorked + (flightDurationMinutes / 60) <= 12 &&
                    crew[j].totalHoursWorked < minHours) {
                    minHours = crew[j].totalHoursWorked;
                    bestCrew = j;
                }
            }
    
            if (bestCrew >= 0) {
                crew[bestCrew].totalHoursWorked += flightDurationMinutes / 60;
                crew[bestCrew].lastRestTime = departureTimeMinutes + flightDurationMinutes;
                crew[bestCrew].isAvailable = 0;
                flights[i].assignedCrewID = bestCrew + 1;
                assigned = 1;
            }
    
            if (!assigned) {
                char message[256];
                snprintf(message, sizeof(message), "No crew available for flight %s!", flights[i].flightID);


                flights[i].assignedCrewID = 0;
            }
    
            flights[i].runway = 1;
        } else {
           
            int arrivalTimeMinutes = (flights[i].arrivalTime / 100) * 60 + (flights[i].arrivalTime % 100);
            for (int j = 0; j < crewCount; j++) {
                if (!crew[j].isAvailable && crew[j].lastRestTime <= arrivalTimeMinutes) {
                    crew[j].isAvailable = 1;
                    crew[j].totalHoursWorked = 0;
                   
                }
            }
        }
    }
    size_t written = fwrite(flights, sizeof(struct Flight), len, f);
    if (written != (size_t)len) {
        return;
    }
    fclose(f);
}