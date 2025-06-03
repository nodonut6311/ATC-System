#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "crew.h"

#define MAX_FLIGHTS 50



static GtkWidget *window;
static GtkWidget *departures_tree_view;
static GtkWidget *arrivals_tree_view;
static GtkListStore *departures_store;
static GtkListStore *arrivals_store;
static int len;
static int airport_opening_time;
static struct Flight *flights;


int createFlights();
void quickSort(struct Flight flights[], int low, int high);
void quickSortByTime(struct Flight flights[], int low, int high);
void allocateDepartureTime(struct Flight flights[], int len, int airport_opening_time);
void allocateArrivalTime(struct Flight flights[], int len, int airport_opening_time);
void allocateRunway(struct Flight flights[], int len);
void printArray(int len);
void addFlight(int *len);
void modifyFlightPriority(int len);
void handleEmergency(int len);
void realTimeUpdate(int len);
void removeFlight(int *len);
struct Flight* updateFile(int len);
static void show_message(GtkMessageType type, const char *message);
static void get_airport_opening_time(GtkWidget *widget, gpointer data);
static void on_add_flight_clicked(GtkButton *button, gpointer data);
static void on_print_schedule_clicked(GtkButton *button, gpointer data);
static void on_modify_priority_clicked(GtkButton *button, gpointer data);
static void on_handle_emergency_clicked(GtkButton *button, gpointer data);
static void on_real_time_update_clicked(GtkButton *button, gpointer data);
static void on_cancel_flight_clicked(GtkButton *button, gpointer data);
static void on_exit_clicked(GtkButton *button, gpointer data);


static void show_message(GtkMessageType type, const char *message) {
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(window),
                                              GTK_DIALOG_MODAL,
                                              type,
                                              GTK_BUTTONS_OK,
                                              "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}


int createFlights() {
    int len = 44;

    struct Flight initial_flights[MAX_FLIGHTS] = {
        {"AI101", "XYZ Intl", "Mumbai", "Boeing 737", 3, 0, 0, 0, 0},
        {"LH303", "XYZ Intl", "Frankfurt", "Boeing 777", 2, 0, 0, 0, 0},
        {"EK505", "XYZ Intl", "Dubai", "Airbus A380", 2, 0, 0, 0, 0},
        {"DL909", "XYZ Intl", "Bangalore", "Boeing 737", 3, 0, 0, 0, 0},
        {"QF707", "XYZ Intl", "Sydney", "Boeing 787", 2, 0, 0, 0, 0},
        {"BA202", "London", "XYZ Intl", "Airbus A320", 2, 0, 0, 0, 0},
        {"SQ404", "Singapore", "XYZ Intl", "Boeing 787", 2, 0, 0, 0, 0},
        {"UA606", "New York", "XYZ Intl", "Boeing 737", 2, 0, 0, 0, 0},
        {"AI808", "Delhi", "XYZ Intl", "Airbus A320", 3, 0, 0, 0, 0},
        {"ER999", "Dubai", "XYZ Intl", "Private Jet", 1, 0, 0, 0, 0},
        {"VS210", "XYZ Intl", "London", "Boeing 777", 2, 0, 0, 0, 0},
        {"AF502", "Paris", "XYZ Intl", "Airbus A350", 2, 0, 0, 0, 0},
        {"CX880", "XYZ Intl", "Hong Kong", "Boeing 747", 2, 0, 0, 0, 0},
        {"TK721", "XYZ Intl", "Istanbul", "Boeing 787", 2, 0, 0, 0, 0},
        {"IB600", "Madrid", "XYZ Intl", "Airbus A320", 2, 0, 0, 0, 0},
        {"JL300", "Tokyo", "XYZ Intl", "Boeing 777", 2, 0, 0, 0, 0},
        {"AA921", "XYZ Intl", "Los Angeles", "Boeing 787", 2, 0, 0, 0, 0},
        {"LH789", "Berlin", "XYZ Intl", "Airbus A320", 2, 0, 0, 0, 0},
        {"SQ801", "XYZ Intl", "Singapore", "Boeing 787", 2, 0, 0, 0, 0},
        {"AI305", "Chennai", "XYZ Intl", "Airbus A320", 3, 0, 0, 0, 0},
        {"BA117", "XYZ Intl", "London", "Boeing 777", 2, 0, 0, 0, 0},
        {"EK430", "Dubai", "XYZ Intl", "Airbus A380", 2, 0, 0, 0, 0},
        {"TK910", "XYZ Intl", "Istanbul", "Boeing 787", 2, 0, 0, 0, 0},
        {"DL432", "XYZ Intl", "New York", "Boeing 767", 2, 0, 0, 0, 0},
        {"AF675", "XYZ Intl", "Paris", "Airbus A350", 2, 0, 0, 0, 0},
        {"LH456", "XYZ Intl", "Frankfurt", "Boeing 747", 2, 0, 0, 0, 0},
        {"AI450", "Hyderabad", "XYZ Intl", "Airbus A320", 1, 0, 0, 0, 0},
        {"CX765", "Hong Kong", "XYZ Intl", "Boeing 747", 1, 0, 0, 0, 0},
        {"JL107", "XYZ Intl", "Tokyo", "Boeing 787", 2, 0, 0, 0, 0},
        {"AI909", "XYZ Intl", "Delhi", "Airbus A320", 3, 0, 0, 0, 0},
        {"VS909", "London", "XYZ Intl", "Boeing 777", 2, 0, 0, 0, 0},
        {"QF609", "XYZ Intl", "Melbourne", "Boeing 787", 2, 0, 0, 0, 0},
        {"UA987", "XYZ Intl", "Chicago", "Boeing 767", 2, 0, 0, 0, 0},
        {"BA215", "XYZ Intl", "London", "Boeing 777", 2, 0, 0, 0, 0},
        {"EK333", "Dubai", "XYZ Intl", "Airbus A380", 2, 0, 0, 0, 0},
        {"DL820", "New York", "XYZ Intl", "Boeing 767", 1, 0, 0, 0, 0},
        {"AI560", "XYZ Intl", "Bangalore", "Airbus A320", 3, 0, 0, 0, 0},
        {"SQ120", "Singapore", "XYZ Intl", "Boeing 787", 2, 0, 0, 0, 0},
        {"LH120", "XYZ Intl", "Berlin", "Airbus A320", 2, 0, 0, 0, 0},
        {"BA401", "London", "XYZ Intl", "Boeing 777", 2, 0, 0, 0, 0},
        {"CX430", "Hong Kong", "XYZ Intl", "Boeing 747", 2, 0, 0, 0, 0},
        {"AF340", "XYZ Intl", "Paris", "Airbus A350", 2, 0, 0, 0, 0},
        {"JL520", "XYZ Intl", "Tokyo", "Boeing 787", 2, 0, 0, 0, 0},
        {"TK632", "XYZ Intl", "Istanbul", "Boeing 787", 2, 0, 0, 0, 0}
    };

    FILE *file = fopen("flights.bin", "wb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for writing!");
        return -1;
    }

    fwrite(initial_flights, sizeof(struct Flight), len, file);
    fclose(file);

    return len;
}

void quickSort(struct Flight flights[], int low, int high) {
    if (low < high) {
        int pivot = flights[high].priority;
        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (flights[j].priority <= pivot) {
                i++;
                struct Flight temp = flights[i];
                flights[i] = flights[j];
                flights[j] = temp;
            }
        }
        struct Flight temp = flights[i + 1];
        flights[i + 1] = flights[high];
        flights[high] = temp;
        int pi = i + 1;
        quickSort(flights, low, pi - 1);
        quickSort(flights, pi + 1, high);
    }
}

void quickSortByTime(struct Flight flights[], int low, int high) {
    if (low < high) {
        int pivot = (strcmp(flights[high].origin, "XYZ Intl") == 0) ? flights[high].departureTime : flights[high].arrivalTime;
        int i = low - 1;
        for (int j = low; j < high; j++) {
            int currentTime = (strcmp(flights[j].origin, "XYZ Intl") == 0) ? flights[j].departureTime : flights[j].arrivalTime;
            if (currentTime <= pivot) {
                i++;
                struct Flight temp = flights[i];
                flights[i] = flights[j];
                flights[j] = temp;
            }
        }
        struct Flight temp = flights[i + 1];
        flights[i + 1] = flights[high];
        flights[high] = temp;
        int pi = i + 1;
        quickSortByTime(flights, low, pi - 1);
        quickSortByTime(flights, pi + 1, high);
    }
}

void allocateDepartureTime(struct Flight flights[], int len, int airport_opening_time) {
    srand(time(NULL));
    int currentTime = airport_opening_time;
    quickSort(flights, 0, len - 1);
    for (int i = 0; i < len; i++) {
        if (strcmp(flights[i].origin, "XYZ Intl") == 0) {
            flights[i].departureTime = currentTime;
            int flightDuration = (rand() % 12) + 1;
            int arrivalHours = (flights[i].departureTime / 100) + flightDuration;
            int arrivalMinutes = flights[i].departureTime % 100;
            if (arrivalHours >= 24)
                arrivalHours -= 24;
            flights[i].arrivalTime = (arrivalHours * 100) + arrivalMinutes;
            int minutes = currentTime % 100;
            int hours = currentTime / 100;
            minutes += 15;
            if (minutes >= 60) {
                minutes -= 60;
                hours++;
            }
            currentTime = (hours * 100) + minutes;
        }
    }
    FILE *file = fopen("flights.bin", "wb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for writing!");
        return;
    }
    fwrite(flights, sizeof(struct Flight), len, file);
    fclose(file);
}

void allocateArrivalTime(struct Flight flights[], int len, int airport_opening_time) {
    srand(time(NULL));
    int currentTime = airport_opening_time;
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (flights[j].priority > flights[j + 1].priority) {
                struct Flight temp = flights[j];
                flights[j] = flights[j + 1];
                flights[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < len; i++) {
        if (strcmp(flights[i].destination, "XYZ Intl") == 0) {
            flights[i].arrivalTime = currentTime;
            int flightDuration = (rand() % 12) + 1;
            int departureHours = (flights[i].arrivalTime / 100) - flightDuration;
            int departureMinutes = flights[i].arrivalTime % 100;
            if (departureHours < 0)
                departureHours += 24;
            flights[i].departureTime = (departureHours * 100) + departureMinutes;
            int minutes = currentTime % 100;
            int hours = currentTime / 100;
            minutes += 15;
            if (minutes >= 60) {
                minutes -= 60;
                hours++;
            }
            currentTime = (hours * 100) + minutes;
        }
    }
    FILE *file = fopen("flights.bin", "wb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for writing!");
        return;
    }
    fwrite(flights, sizeof(struct Flight), len, file);
    fclose(file);
}

void allocateRunway(struct Flight flights[], int len) {
    for (int i = 0; i < len; i++) {
        if (strcmp(flights[i].origin, "XYZ Intl") == 0) {
            flights[i].runway = 1;
        } else {
            flights[i].runway = 2;
        }
    }
    FILE *file = fopen("flights.bin", "wb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for writing!");
        return;
    }
    fwrite(flights, sizeof(struct Flight), len, file);
    fclose(file);
}

void printArray(int len) {
    gtk_list_store_clear(departures_store);
    gtk_list_store_clear(arrivals_store);
    char dep_time[6], arr_time[6];
    GtkTreeIter iter;
    for (int i = 0; i < len; i++) {
        if (strcmp(flights[i].origin, "XYZ Intl") == 0) {
            snprintf(dep_time, sizeof(dep_time), "%02d:%02d", flights[i].departureTime / 100, flights[i].departureTime % 100);
            snprintf(arr_time, sizeof(arr_time), "%02d:%02d", flights[i].arrivalTime / 100, flights[i].arrivalTime % 100);
            gtk_list_store_append(departures_store, &iter);
            gtk_list_store_set(departures_store, &iter,
                               0, flights[i].flightID,
                               1, flights[i].origin,
                               2, flights[i].destination,
                               3, flights[i].aircraftType,
                               4, flights[i].priority,
                               5, flights[i].runway,
                               6, dep_time,
                               7, arr_time,
                               8, flights[i].assignedCrewID,
                               -1);
        }
    }
    for (int i = 0; i < len; i++) {
        if (strcmp(flights[i].destination, "XYZ Intl") == 0) {
            snprintf(arr_time, sizeof(arr_time), "%02d:%02d", flights[i].arrivalTime / 100, flights[i].arrivalTime % 100);
            snprintf(dep_time, sizeof(dep_time), "%02d:%02d", flights[i].departureTime / 100, flights[i].departureTime % 100);
            gtk_list_store_append(arrivals_store, &iter);
            gtk_list_store_set(arrivals_store, &iter,
                               0, flights[i].flightID,
                               1, flights[i].origin,
                               2, flights[i].destination,
                               3, flights[i].aircraftType,
                               4, flights[i].priority,
                               5, flights[i].runway,
                               6, arr_time,
                               7, dep_time,
                               -1);
        }
    }
}

void addFlight(int * zabud) {
    struct Flight flights[MAX_FLIGHTS];
    FILE *file = fopen("flights.bin", "rb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for reading!");
        return;
    }
    fread(flights, sizeof(struct Flight), *zabud, file);
    fclose(file);
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Add Flight", GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    GtkWidget *flight_id_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(flight_id_entry), 9);
    GtkWidget *origin_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(origin_entry), 29);
    GtkWidget *destination_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(destination_entry), 29);
    GtkWidget *aircraft_type_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(aircraft_type_entry), 19);
    GtkWidget *priority_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Flight ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), flight_id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Origin:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), origin_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Destination:"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), destination_entry, 1, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Aircraft Type:"), 0, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), aircraft_type_entry, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Priority (1-3):"), 0, 4, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), priority_entry, 1, 4, 1, 1);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        struct Flight newFlight;
        strncpy(newFlight.flightID, gtk_entry_get_text(GTK_ENTRY(flight_id_entry)), sizeof(newFlight.flightID) - 1);
        newFlight.flightID[sizeof(newFlight.flightID) - 1] = '\0';
        strncpy(newFlight.origin, gtk_entry_get_text(GTK_ENTRY(origin_entry)), sizeof(newFlight.origin) - 1);
        newFlight.origin[sizeof(newFlight.origin) - 1] = '\0';
        strncpy(newFlight.destination, gtk_entry_get_text(GTK_ENTRY(destination_entry)), sizeof(newFlight.destination) - 1);
        newFlight.destination[sizeof(newFlight.destination) - 1] = '\0';
        strncpy(newFlight.aircraftType, gtk_entry_get_text(GTK_ENTRY(aircraft_type_entry)), sizeof(newFlight.aircraftType) - 1);
        newFlight.aircraftType[sizeof(newFlight.aircraftType) - 1] = '\0';
        newFlight.priority = atoi(gtk_entry_get_text(GTK_ENTRY(priority_entry)));
        newFlight.runway = 0;
        newFlight.departureTime = 0;
        newFlight.arrivalTime = 0;
        newFlight.assignedCrewID = 0;
        if (newFlight.priority < 1 || newFlight.priority > 3) {
            show_message(GTK_MESSAGE_ERROR, "Priority must be between 1 and 3!");
            gtk_widget_destroy(dialog);
            return;
        }
        if (strlen(newFlight.flightID) == 0 || strlen(newFlight.origin) == 0 ||
            strlen(newFlight.destination) == 0 || strlen(newFlight.aircraftType) == 0) {
            show_message(GTK_MESSAGE_ERROR, "All fields must be filled!");
            gtk_widget_destroy(dialog);
            return;
        }
        flights[*zabud] = newFlight;
        file = fopen("flights.bin", "ab+");
        if (file == NULL) {
            show_message(GTK_MESSAGE_ERROR, "Error opening file for writing!");
            gtk_widget_destroy(dialog);
            return;
        }
        fwrite(&newFlight, sizeof(struct Flight), 1, file);
        fclose(file);
        (*zabud)++;
        show_message(GTK_MESSAGE_INFO, "Flight added successfully!");
    }
    gtk_widget_destroy(dialog);
}

void modifyFlightPriority(int len) {
    struct Flight flights[MAX_FLIGHTS];
    FILE *file = fopen("flights.bin", "rb+");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for modification!");
        return;
    }
    fread(flights, sizeof(struct Flight), len, file);
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Modify Flight Priority", GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    GtkWidget *flight_id_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(flight_id_entry), 9);
    GtkWidget *priority_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Flight ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), flight_id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("New Priority (1-3):"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), priority_entry, 1, 1, 1, 1);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        const char *flight_id = gtk_entry_get_text(GTK_ENTRY(flight_id_entry));
        int new_priority = atoi(gtk_entry_get_text(GTK_ENTRY(priority_entry)));
        int found = 0;
        for (int i = 0; i < len; i++) {
            if (strcmp(flights[i].flightID, flight_id) == 0) {
                flights[i].priority = new_priority;
                if (new_priority < 1 || new_priority > 3) {
                    show_message(GTK_MESSAGE_ERROR, "Invalid priority! Keeping old value.");
                } else {
                    show_message(GTK_MESSAGE_INFO, "Priority updated successfully!");
                }
                found = 1;
                break;
            }
        }
        if (!found) {
            show_message(GTK_MESSAGE_ERROR, "Flight ID not found!");
        } else {
            rewind(file);
            fwrite(flights, sizeof(struct Flight), len, file);
        }
    }
    fclose(file);
    gtk_widget_destroy(dialog);
}

void handleEmergency(int len) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Handle Emergency", GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    GtkWidget *flight_id_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(flight_id_entry), 9);
    GtkWidget *emergency_type_entry = gtk_entry_new();
    GtkWidget *emergency_time_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Flight ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), flight_id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Emergency Type (1=Landing, 2=Takeoff):"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), emergency_type_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Emergency Time (HHMM):"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), emergency_time_entry, 1, 2, 1, 1);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        char id[10];
        int emergencyType = atoi(gtk_entry_get_text(GTK_ENTRY(emergency_type_entry)));
        int emergencyTime = atoi(gtk_entry_get_text(GTK_ENTRY(emergency_time_entry)));
        strncpy(id, gtk_entry_get_text(GTK_ENTRY(flight_id_entry)), sizeof(id) - 1);
        id[sizeof(id) - 1] = '\0';
        if (emergencyType != 1 && emergencyType != 2) {
            show_message(GTK_MESSAGE_ERROR, "Invalid emergency type! Use 1 for Landing or 2 for Takeoff.");
            gtk_widget_destroy(dialog);
            return;
        }
        if (emergencyTime < 0 || emergencyTime > 2359 || (emergencyTime % 100) > 59) {
            show_message(GTK_MESSAGE_ERROR, "Invalid time format! Use HHMM (0000-2359).");
            gtk_widget_destroy(dialog);
            return;
        }
        struct Flight flights[MAX_FLIGHTS];
        FILE *file = fopen("flights.bin", "rb+");
        if (file == NULL) {
            show_message(GTK_MESSAGE_ERROR, "Error opening file for reading!");
            gtk_widget_destroy(dialog);
            return;
        }
        fread(flights, sizeof(struct Flight), len, file);
        int found = 0;
        for (int i = 0; i < len; i++) {
            if (strcmp(flights[i].flightID, id) == 0) {
                found = 1;
                int runwayToAllocate = 1;
                for (int j = 0; j < len; j++) {
                    if (flights[j].departureTime == emergencyTime || flights[j].arrivalTime == emergencyTime) {
                        if (flights[j].priority > flights[i].priority) {
                            runwayToAllocate = flights[j].runway;
                            break;
                        } else if (flights[j].priority == flights[i].priority) {
                            runwayToAllocate = (flights[j].runway == 1) ? 2 : 1;
                            break;
                        }
                    }
                }
                int originalDuration = flights[i].arrivalTime - flights[i].departureTime;
                flights[i].runway = runwayToAllocate;
                char message[256];
                if (emergencyType == 1) {
                    flights[i].arrivalTime = emergencyTime;
                    flights[i].departureTime = emergencyTime - originalDuration;
                    if (flights[i].departureTime < 0) {
                        flights[i].departureTime += 2400;
                    }
                    snprintf(message, sizeof(message), "Runway %d allocated for emergency landing of flight %s.", runwayToAllocate, flights[i].flightID);
                } else {
                    flights[i].departureTime = emergencyTime;
                    flights[i].arrivalTime = emergencyTime + originalDuration;
                    if (flights[i].arrivalTime >= 2400) {
                        flights[i].arrivalTime -= 2400;
                    }
                    snprintf(message, sizeof(message), "Runway %d allocated for emergency takeoff of flight %s.", runwayToAllocate, flights[i].flightID);
                }
                for (int k = 0; k < len; k++) {
                    if (k != i &&
                        ((emergencyType == 1 && flights[k].arrivalTime >= emergencyTime) ||
                         (emergencyType == 2 && flights[k].departureTime >= emergencyTime))) {
                        int delayMinutes = 15;
                        int depH = flights[k].departureTime / 100;
                        int depM = flights[k].departureTime % 100;
                        int arrH = flights[k].arrivalTime / 100;
                        int arrM = flights[k].arrivalTime % 100;
                        depM += delayMinutes;
                        if (depM >= 60) {
                            depM -= 60;
                            depH++;
                            if (depH >= 24) depH -= 24;
                        }
                        arrM += delayMinutes;
                        if (arrM >= 60) {
                            arrM -= 60;
                            arrH++;
                            if (arrH >= 24) arrH -= 24;
                        }
                        flights[k].departureTime = depH * 100 + depM;
                        flights[k].arrivalTime = arrH * 100 + arrM;
                    }
                }
                show_message(GTK_MESSAGE_INFO, message);
                break;
            }
        }
        if (!found) {
            show_message(GTK_MESSAGE_ERROR, "Flight ID not found!");
        } else {
            quickSortByTime(flights, 0, len - 1);
            rewind(file);
            fwrite(flights, sizeof(struct Flight), len, file);
        }
        fclose(file);
    }
    gtk_widget_destroy(dialog);
}

void realTimeUpdate(int len) {
    struct Flight flights[MAX_FLIGHTS];
    FILE *file = fopen("flights.bin", "rb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for reading!");
        return;
    }
    fread(flights, sizeof(struct Flight), len, file);
    fclose(file);
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Real Time Update", GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    GtkWidget *flight_id_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(flight_id_entry), 9);
    GtkWidget *delay_time_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Flight ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), flight_id_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Delay Time (minutes):"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), delay_time_entry, 1, 1, 1, 1);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        char id[10];
        int delayTime = atoi(gtk_entry_get_text(GTK_ENTRY(delay_time_entry)));
        strncpy(id, gtk_entry_get_text(GTK_ENTRY(flight_id_entry)), sizeof(id) - 1);
        id[sizeof(id) - 1] = '\0';
        if (delayTime <= 0) {
            show_message(GTK_MESSAGE_ERROR, "Delay time must be positive!");
            gtk_widget_destroy(dialog);
            return;
        }
        file = fopen("flights.bin", "rb+");
        if (file == NULL) {
            show_message(GTK_MESSAGE_ERROR, "Error opening file for reading!");
            gtk_widget_destroy(dialog);
            return;
        }
        fread(flights, sizeof(struct Flight), len, file);
        int delayedFlightIndex = -1;
        for (int i = 0; i < len; i++) {
            if (strcmp(id, flights[i].flightID) == 0) {
                delayedFlightIndex = i;
                break;
            }
        }
        if (delayedFlightIndex == -1) {
            show_message(GTK_MESSAGE_ERROR, "Flight ID not found!");
            fclose(file);
            gtk_widget_destroy(dialog);
            return;
        }
        for (int i = delayedFlightIndex; i < len; i++) {
            int depMinutes = flights[i].departureTime % 100;
            int depHours = flights[i].departureTime / 100;
            int arrMinutes = flights[i].arrivalTime % 100;
            int arrHours = flights[i].arrivalTime / 100;
            depMinutes += delayTime;
            arrMinutes += delayTime;
            if (depMinutes >= 60) {
                depMinutes -= 60;
                depHours++;
            }
            if (depHours >= 24) {
                depHours -= 24;
            }
            if (arrMinutes >= 60) {
                arrMinutes -= 60;
                arrHours++;
            }
            if (arrHours >= 24) {
                arrHours -= 24;
            }
            flights[i].departureTime = (depHours * 100) + depMinutes;
            flights[i].arrivalTime = (arrHours * 100) + arrMinutes;
        }
        rewind(file);
        fwrite(flights, sizeof(struct Flight), len, file);
        fclose(file);
        show_message(GTK_MESSAGE_INFO, "Flight times updated successfully!");
    }
    gtk_widget_destroy(dialog);
}

void removeFlight(int *len) {
    struct Flight flights[MAX_FLIGHTS];
    FILE *file = fopen("flights.bin", "rb");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file for reading!");
        return;
    }
    fread(flights, sizeof(struct Flight), *len, file);
    fclose(file);
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Cancel Flight", GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    "_Cancel", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 10);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 10);
    GtkWidget *flight_id_entry = gtk_entry_new();
    gtk_entry_set_max_length(GTK_ENTRY(flight_id_entry), 9);
    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Flight ID:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), flight_id_entry, 1, 0, 1, 1);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        char flightID[10];
        strncpy(flightID, gtk_entry_get_text(GTK_ENTRY(flight_id_entry)), sizeof(flightID) - 1);
        flightID[sizeof(flightID) - 1] = '\0';
        file = fopen("flights.bin", "rb");
        if (file == NULL) {
            show_message(GTK_MESSAGE_ERROR, "Error opening file for reading!");
            gtk_widget_destroy(dialog);
            return;
        }
        fread(flights, sizeof(struct Flight), *len, file);
        fclose(file);
        int found = 0;
        for (int i = 0; i < *len; i++) {
            if (strcmp(flights[i].flightID, flightID) == 0) {
                found = 1;
                for (int j = i; j < *len - 1; j++) {
                    flights[j] = flights[j + 1];
                }
                (*len)--;
                break;
            }
        }
        if (!found) {
            show_message(GTK_MESSAGE_ERROR, "Flight ID not found!");
        } else {
            file = fopen("flights.bin", "wb");
            if (file == NULL) {
                show_message(GTK_MESSAGE_ERROR, "Error opening file for writing!");
                gtk_widget_destroy(dialog);
                return;
            }
            fwrite(flights, sizeof(struct Flight), *len, file);
            fclose(file);
            show_message(GTK_MESSAGE_INFO, "Flight removed successfully!");
        }
    }
    gtk_widget_destroy(dialog);
}

struct Flight* updateFile(int len) {
    static struct Flight flights[MAX_FLIGHTS];
    FILE *file = fopen("flights.bin", "rb+");
    if (file == NULL) {
        show_message(GTK_MESSAGE_ERROR, "Error opening file!");
        return flights;
    }
    fread(flights, sizeof(struct Flight), len, file);
    fclose(file);
    return flights;
}

static void on_add_flight_clicked(GtkButton *button, gpointer data) {
    addFlight(&len);
}

static void on_print_schedule_clicked(GtkButton *button, gpointer data) {
        flights = updateFile(len);
        allocateDepartureTime(flights, len, airport_opening_time);
        allocateArrivalTime(flights, len, airport_opening_time);
        allocateRunway(flights, len);
        initializeCrew();
        allocateCrew(flights, len, airport_opening_time);

        printArray(len);
}

static void on_modify_priority_clicked(GtkButton *button, gpointer data) {
    modifyFlightPriority(len);
}

static void on_handle_emergency_clicked(GtkButton *button, gpointer data) {
    handleEmergency(len);
}

static void on_real_time_update_clicked(GtkButton *button, gpointer data) {
    realTimeUpdate(len);
}

static void on_cancel_flight_clicked(GtkButton *button, gpointer data) {
    removeFlight(&len);
}

static void on_exit_clicked(GtkButton *button, gpointer data) {
    gtk_main_quit();
}

static void get_airport_opening_time(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Airport Opening Time", GTK_WINDOW(window),
                                                    GTK_DIALOG_MODAL,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter time (HHMM)");
    gtk_container_add(GTK_CONTAINER(content_area), entry);
    gtk_widget_show_all(dialog);
    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_OK) {
        airport_opening_time = atoi(gtk_entry_get_text(GTK_ENTRY(entry)));
        if (airport_opening_time < 0 || airport_opening_time > 2359 || (airport_opening_time % 100) > 59) {
            show_message(GTK_MESSAGE_ERROR, "Invalid time format! Use HHMM (0000-2359).");
            gtk_widget_destroy(dialog);
            return;
        }
        len = createFlights();
        if (len == -1) {
            show_message(GTK_MESSAGE_ERROR, "Error creating flights!");
            gtk_main_quit();
        }
        flights = updateFile(len);
        initializeCrew();
        show_message(GTK_MESSAGE_INFO, "Airport simulation started.");
    } else {
        gtk_main_quit();
    }
    gtk_widget_destroy(dialog);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Flight Simulation Program");
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 600);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), button_box, FALSE, FALSE, 0);
    GtkWidget *add_flight_button = gtk_button_new_with_label("Add Flight");
    GtkWidget *print_schedule_button = gtk_button_new_with_label("Print Schedule");
    GtkWidget *modify_priority_button = gtk_button_new_with_label("Modify Priority");
    GtkWidget *handle_emergency_button = gtk_button_new_with_label("Handle Emergency");
    GtkWidget *real_time_update_button = gtk_button_new_with_label("Real Time Update");
    GtkWidget *cancel_flight_button = gtk_button_new_with_label("Cancel Flight");
    GtkWidget *exit_button = gtk_button_new_with_label("Exit");
    gtk_box_pack_start(GTK_BOX(button_box), add_flight_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), print_schedule_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), modify_priority_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), handle_emergency_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), real_time_update_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), cancel_flight_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(button_box), exit_button, TRUE, TRUE, 0);
    g_signal_connect(add_flight_button, "clicked", G_CALLBACK(on_add_flight_clicked), NULL);
    g_signal_connect(print_schedule_button, "clicked", G_CALLBACK(on_print_schedule_clicked), NULL);
    g_signal_connect(modify_priority_button, "clicked", G_CALLBACK(on_modify_priority_clicked), NULL);
    g_signal_connect(handle_emergency_button, "clicked", G_CALLBACK(on_handle_emergency_clicked), NULL);
    g_signal_connect(real_time_update_button, "clicked", G_CALLBACK(on_real_time_update_clicked), NULL);
    g_signal_connect(cancel_flight_button, "clicked", G_CALLBACK(on_cancel_flight_clicked), NULL);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_clicked), NULL);
    GtkWidget *tables_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_box_pack_start(GTK_BOX(vbox), tables_vbox, TRUE, TRUE, 0);
    GtkWidget *departures_label = gtk_label_new("<b>Departures</b>");
    gtk_label_set_use_markup(GTK_LABEL(departures_label), TRUE);
    gtk_box_pack_start(GTK_BOX(tables_vbox), departures_label, FALSE, FALSE, 0);
    GtkWidget *departures_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(departures_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tables_vbox), departures_scrolled, TRUE, TRUE, 0);
    departures_store = gtk_list_store_new(9,
                                         G_TYPE_STRING, // Flight ID
                                         G_TYPE_STRING, // Origin
                                         G_TYPE_STRING, // Destination
                                         G_TYPE_STRING, // Aircraft Type
                                         G_TYPE_INT,    // Priority
                                         G_TYPE_INT,    // Runway
                                         G_TYPE_STRING, // Departure Time
                                         G_TYPE_STRING, // Arrival Time
                                         G_TYPE_INT);   // Crew ID
    departures_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(departures_store));
    gtk_container_add(GTK_CONTAINER(departures_scrolled), departures_tree_view);
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column;
    column = gtk_tree_view_column_new_with_attributes("Flight ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Origin", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Destination", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Aircraft Type", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Priority", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Runway", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Departure Time", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Arrival Time", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Crew ID", renderer, "text", 8, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(departures_tree_view), column);
    GtkWidget *arrivals_label = gtk_label_new("<b>Arrivals</b>");
    gtk_label_set_use_markup(GTK_LABEL(arrivals_label), TRUE);
    gtk_box_pack_start(GTK_BOX(tables_vbox), arrivals_label, FALSE, FALSE, 0);
    GtkWidget *arrivals_scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(arrivals_scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(tables_vbox), arrivals_scrolled, TRUE, TRUE, 0);
    arrivals_store = gtk_list_store_new(8,
                                       G_TYPE_STRING, // Flight ID
                                       G_TYPE_STRING, // Origin
                                       G_TYPE_STRING, // Destination
                                       G_TYPE_STRING, // Aircraft Type
                                       G_TYPE_INT,    // Priority
                                       G_TYPE_INT,    // Runway
                                       G_TYPE_STRING, // Arrival Time
                                       G_TYPE_STRING); // Departure Time
    arrivals_tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(arrivals_store));
    gtk_container_add(GTK_CONTAINER(arrivals_scrolled), arrivals_tree_view);
    column = gtk_tree_view_column_new_with_attributes("Flight ID", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Origin", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Destination", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Aircraft Type", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Priority", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Runway", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Arrival Time", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    column = gtk_tree_view_column_new_with_attributes("Departure Time", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(arrivals_tree_view), column);
    gtk_widget_show_all(window);
    get_airport_opening_time(NULL, NULL);
    gtk_main();
    return 0;
}