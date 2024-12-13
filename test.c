#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // for usleep

void *myFunction(void *arg) {
    int interval_ms = *(int *)arg;
    while (1) {
        printf("Function executed!\n");
        usleep(interval_ms * 1000); // Sleep for specified milliseconds
    }
    return NULL;
}

int main() {
    pthread_t thread;
    int interval_ms = 100; // 1000 milliseconds = 1 second

    // Create a thread that executes myFunction with interval_ms as argument
    if (pthread_create(&thread, NULL, myFunction, &interval_ms) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Continue with main program
    while (1) {
        printf("ewe\n");
        usleep(500 * 1000); // Sleep for 500 milliseconds
    }

    // This part of the code is unreachable in this example,
    // as the program will run indefinitely until interrupted.
    return 0;
}

