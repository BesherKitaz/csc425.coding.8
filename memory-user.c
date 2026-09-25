#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        fprintf(stderr, "Usage: %s <megabytes> [seconds]\n", argv[0]);
        return 1;
    }

    long megabytes = strtol(argv[1], NULL, 10);
    if (megabytes <= 0) {
        fprintf(stderr, "Megabytes must be a positive number.\n");
        return 1;
    }

    long seconds = 0;   /* 0 means run indefinitely */
    if (argc == 3) {
        seconds = strtol(argv[2], NULL, 10);
        if (seconds <= 0) {
            fprintf(stderr, "Seconds must be a positive number.\n");
            return 1;
        }
    }

    size_t bytes = (size_t)megabytes * 1024 * 1024;
    unsigned char *memory = malloc(bytes);

    if (memory == NULL) {
        fprintf(stderr, "Could not allocate %ld MB of memory.\n", megabytes);
        return 1;
    }

    printf("PID: %d\n", getpid());
    printf("Allocated %ld MB of memory.\n", megabytes);

    if (seconds == 0) {
        printf("Running indefinitely. Press Ctrl+C to stop.\n");
    } else {
        printf("Running for %ld seconds.\n", seconds);
    }

    fflush(stdout);

    time_t start = time(NULL);
    unsigned char value = 0;

    while (seconds == 0 || time(NULL) - start < seconds) {
        /*
         * Stream through the entire allocated array and touch every byte.
         * Changing the value each pass ensures that the memory is actively used.
         */
        for (size_t i = 0; i < bytes; i++) {
            memory[i] = value;
        }
        value++;
    }

    free(memory);
    return 0;
}