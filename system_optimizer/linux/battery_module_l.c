#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h> // Required for sleep()
#include <time.h>

#define BATTERY_PATH "/sys/class/power_supply/BAT0/"
#define CAPACITY_FILE BATTERY_PATH "capacity"
#define STATUS_FILE BATTERY_PATH "status"
#define NOTIFICATION_COOLDOWN 600
#define BASE_SLEEP 300
#define CRITICAL_SLEEP 60

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return NULL;

    char* buffer = malloc(64);
    if (!buffer) {
        fclose(file);
        return NULL;
    }

    if (fgets(buffer, 64, file) == NULL) {
        free(buffer);
        fclose(file);
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = 0;
    fclose(file);
    return buffer;
}

int get_battery_percentage() {
    char* capacity = read_file(CAPACITY_FILE);
    if (!capacity) return -1;

    int percentage = atoi(capacity);
    free(capacity);
    return (percentage >= 0 && percentage <= 100) ? percentage : -1;
}

int is_charging() {
    char* status = read_file(STATUS_FILE);
    if (!status) return -1;

    int charging = (strcmp(status, "Charging") == 0);
    free(status);
    return charging;
}

void* battery_monitor_thread(void* arg) {
    time_t last_notification = 0;
    int last_percentage = -1;
    int last_charging = -1;

    while (1) {
        time_t current_time = time(NULL);
        int battery_level = get_battery_percentage();
        int charging = is_charging();

        if (battery_level == -1 || charging == -1 ||
            (battery_level == last_percentage && charging == last_charging)) {
            sleep(BASE_SLEEP);
            continue;
        }

        last_percentage = battery_level;
        last_charging = charging;

        if (current_time - last_notification >= NOTIFICATION_COOLDOWN) {
            if (battery_level >= 80 && charging) {
                system("zenity --info --text='Battery is above 80% and still charging. Please unplug the charger.' --title='Battery Notification'");
                last_notification = current_time;
            } else if (battery_level < 40 && !charging) {
                system("zenity --warning --text='Battery is below 40% and not charging. Please plug in the charger.' --title='Battery Notification'");
                last_notification = current_time;
            }
        }

        sleep(battery_level < 20 && !charging ? CRITICAL_SLEEP : BASE_SLEEP);
    }

    return NULL;
}

int main() {
    if (access(CAPACITY_FILE, R_OK) != 0 || access(STATUS_FILE, R_OK) != 0) {
        fprintf(stderr, "Error: Battery information not available at %s\n", BATTERY_PATH);
        return 1;
    }

    pthread_t thread;
    int result = pthread_create(&thread, NULL, battery_monitor_thread, NULL);
    if (result != 0) {
        fprintf(stderr, "Failed to create thread: %s\n", strerror(result));
        return 1;
    }

    pthread_join(thread, NULL);
    return 0;
}