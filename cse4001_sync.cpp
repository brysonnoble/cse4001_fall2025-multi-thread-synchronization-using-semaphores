#include "semaphore_class.h"
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Global variables for problem selection
int problem_number = 0;

// Problem 1: No-starve Readers-Writers
const int NUM_READERS_1 = 5;
const int NUM_WRITERS_1 = 5;
Semaphore *roomEmpty_1, *turnstile_1;
int readers_1 = 0;
pthread_mutex_t mutex_1;

// Problem 2: Writer-priority Readers-Writers
const int NUM_READERS_2 = 5;
const int NUM_WRITERS_2 = 5;
Semaphore *noReaders_2, *noWriters_2;
int readers_2 = 0;
int writers_2 = 0;
pthread_mutex_t readSwitch_2, writeSwitch_2;

// Problem 3 & 4: Dining Philosophers
const int NUM_PHILOSOPHERS = 5;
Semaphore *forks[NUM_PHILOSOPHERS];
Semaphore *footman; // For solution 2

// Problem 1: No-starve Readers-Writers
void *reader_nostarve(void *threadID) {
    int id = (long)threadID;
    
    while(1) {
        sleep(rand() % 3 + 1);
        
        turnstile_1->wait();
        turnstile_1->signal();
        
        pthread_mutex_lock(&mutex_1);
        readers_1++;
        if(readers_1 == 1) {
            roomEmpty_1->wait();
        }
        pthread_mutex_unlock(&mutex_1);
        
        printf("Reader %d: Reading.\n", id);
        fflush(stdout);
        sleep(1);
        
        pthread_mutex_lock(&mutex_1);
        readers_1--;
        if(readers_1 == 0) {
            roomEmpty_1->signal();
        }
        pthread_mutex_unlock(&mutex_1);
    }
    return NULL;
}

void *writer_nostarve(void *threadID) {
    int id = (long)threadID;
    
    while(1) {
        sleep(rand() % 3 + 1);
        
        turnstile_1->wait();
        roomEmpty_1->wait();
        
        printf("Writer %d: Writing.\n", id);
        fflush(stdout);
        sleep(2);
        
        roomEmpty_1->signal();
        turnstile_1->signal();
    }
    return NULL;
}

// Problem 2: Writer-priority Readers-Writers
void *reader_writerpriority(void *threadID) {
    int id = (long)threadID;
    
    while(1) {
        sleep(rand() % 3 + 1);
        
        noReaders_2->wait();
        pthread_mutex_lock(&readSwitch_2);
        readers_2++;
        if(readers_2 == 1) {
            noWriters_2->wait();
        }
        pthread_mutex_unlock(&readSwitch_2);
        noReaders_2->signal();
        
        printf("Reader %d: Reading.\n", id);
        fflush(stdout);
        sleep(1);
        
        pthread_mutex_lock(&readSwitch_2);
        readers_2--;
        if(readers_2 == 0) {
            noWriters_2->signal();
        }
        pthread_mutex_unlock(&readSwitch_2);
    }
    return NULL;
}

void *writer_writerpriority(void *threadID) {
    int id = (long)threadID;
    
    while(1) {
        sleep(rand() % 3 + 1);
        
        pthread_mutex_lock(&writeSwitch_2);
        writers_2++;
        if(writers_2 == 1) {
            noReaders_2->wait();
        }
        pthread_mutex_unlock(&writeSwitch_2);
        
        noWriters_2->wait();
        
        printf("Writer %d: Writing.\n", id);
        fflush(stdout);
        sleep(2);
        
        noWriters_2->signal();
        
        pthread_mutex_lock(&writeSwitch_2);
        writers_2--;
        if(writers_2 == 0) {
            noReaders_2->signal();
        }
        pthread_mutex_unlock(&writeSwitch_2);
    }
    return NULL;
}

// Helper functions for philosophers
void get_forks(int id) {
    int right = id;
    int left = (id + 1) % NUM_PHILOSOPHERS;
    
    if(id % 2 == 0) {
        forks[right]->wait();
        forks[left]->wait();
    } else {
        forks[left]->wait();
        forks[right]->wait();
    }
}

void put_forks(int id) {
    int right = id;
    int left = (id + 1) % NUM_PHILOSOPHERS;
    
    forks[right]->signal();
    forks[left]->signal();
}

// Problem 3: Dining Philosophers Solution #1 (using asymmetric solution)
void *philosopher_solution1(void *threadID) {
    int id = (long)threadID;
    
    while(1) {
        printf("Philosopher %d: Thinking\n", id);
        fflush(stdout);
        sleep(rand() % 3 + 1);
        
        get_forks(id);
        
        printf("Philosopher %d: Eating\n", id);
        fflush(stdout);
        sleep(2);
        
        put_forks(id);
    }
    return NULL;
}

// Problem 4: Dining Philosophers Solution #2 (using footman)
void *philosopher_solution2(void *threadID) {
    int id = (long)threadID;
    
    while(1) {
        printf("Philosopher %d: Thinking\n", id);
        fflush(stdout);
        sleep(rand() % 3 + 1);
        
        footman->wait();
        
        int right = id;
        int left = (id + 1) % NUM_PHILOSOPHERS;
        forks[right]->wait();
        forks[left]->wait();
        
        printf("Philosopher %d: Eating\n", id);
        fflush(stdout);
        sleep(2);
        
        forks[right]->signal();
        forks[left]->signal();
        
        footman->signal();
    }
    return NULL;
}

void run_problem_1() {
    turnstile_1 = new Semaphore(1);
    roomEmpty_1 = new Semaphore(1);
    pthread_mutex_init(&mutex_1, NULL);
    
    pthread_t readers[NUM_READERS_1];
    pthread_t writers[NUM_WRITERS_1];
    
    for(long i = 0; i < NUM_READERS_1; i++) {
        pthread_create(&readers[i], NULL, reader_nostarve, (void*)(i+1));
    }
    
    for(long i = 0; i < NUM_WRITERS_1; i++) {
        pthread_create(&writers[i], NULL, writer_nostarve, (void*)(i+1));
    }
    
    pthread_exit(NULL);
}

void run_problem_2() {
    noReaders_2 = new Semaphore(1);
    noWriters_2 = new Semaphore(1);
    pthread_mutex_init(&readSwitch_2, NULL);
    pthread_mutex_init(&writeSwitch_2, NULL);
    
    pthread_t readers[NUM_READERS_2];
    pthread_t writers[NUM_WRITERS_2];
    
    for(long i = 0; i < NUM_READERS_2; i++) {
        pthread_create(&readers[i], NULL, reader_writerpriority, (void*)(i+1));
    }
    
    for(long i = 0; i < NUM_WRITERS_2; i++) {
        pthread_create(&writers[i], NULL, writer_writerpriority, (void*)(i+1));
    }
    
    pthread_exit(NULL);
}

void run_problem_3() {
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = new Semaphore(1);
    }
    
    pthread_t philosophers[NUM_PHILOSOPHERS];
    
    for(long i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher_solution1, (void*)i);
    }
    
    pthread_exit(NULL);
}

void run_problem_4() {
    for(int i = 0; i < NUM_PHILOSOPHERS; i++) {
        forks[i] = new Semaphore(1);
    }
    
    footman = new Semaphore(NUM_PHILOSOPHERS - 1);
    
    pthread_t philosophers[NUM_PHILOSOPHERS];
    
    for(long i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i], NULL, philosopher_solution2, (void*)i);
    }
    
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s <problem #>\n", argv[0]);
        printf("Problem 1: No-starve Readers-Writers\n");
        printf("Problem 2: Writer-priority Readers-Writers\n");
        printf("Problem 3: Dining Philosophers Solution #1\n");
        printf("Problem 4: Dining Philosophers Solution #2\n");
        return 1;
    }
    
    problem_number = atoi(argv[1]);
    srand(time(NULL));
    
    switch(problem_number) {
        case 1:
            run_problem_1();
            break;
        case 2:
            run_problem_2();
            break;
        case 3:
            run_problem_3();
            break;
        case 4:
            run_problem_4();
            break;
        default:
            printf("Invalid problem number. Please choose 1-4.\n");
            return 1;
    }
    
    return 0;
}
