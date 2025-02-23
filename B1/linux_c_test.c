#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

#define _BAI_1_ 1
#define _BAI_2_ 2

#define MODE _BAI_2_ 

#if(MODE == _BAI_1_)

// Function for thread 2
void* thread2_function(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d bắt đầu\n", id);
    sleep(1);
    printf("Thread %d tiếp tục lần 1\n", id);
    sleep(1);
    printf("Thread %d tiếp tục lần 2\n", id);
    sleep(1);
    printf("Thread %d tiếp tục lần 3\n", id);
    sleep(1);
    printf("Thread %d tiếp tục lần 4\n", id);
    printf("Thread %d kết thúc\n", id);
    return NULL;
}

// Function for thread 1
void* thread1_function(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d bắt đầu\n", id);
    for (int count = 0; count < 20; count++) {
        printf("Thread %d chạy vòng lặp %d\n", id, count);
        usleep(300000);
    }
    printf("Thread %d kết thúc\n", id);
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_attr_t attr1, attr2;
    struct sched_param param1, param2;

    // 🛠 Initialize attributes for thread 1
    pthread_attr_init(&attr1);
    pthread_attr_setschedpolicy(&attr1, SCHED_FIFO);
    param1.sched_priority = 60;
    pthread_attr_setschedparam(&attr1, &param1);

    // 🛠 Initialize attributes for thread 2
    pthread_attr_init(&attr2);
    pthread_attr_setschedpolicy(&attr2, SCHED_FIFO);
    param2.sched_priority = 90;
    pthread_attr_setschedparam(&attr2, &param2);

    // 🏁 Create thread 2 first to ensure it runs immediately
    pthread_create(&t2, &attr2, thread2_function, &(int){2});
    pthread_create(&t1, &attr1, thread1_function, &(int){1});

    // 🛠 Cleanup thread attributes
    pthread_attr_destroy(&attr1);
    pthread_attr_destroy(&attr2);

    // Wait for threads to finish
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Tất cả thread đã kết thúc\n");
    return 0;
}

#else

// Function to simulate a process workload
void run_process(int nice_value) {
    if (nice(nice_value) == -1) {
        perror("nice");
    }
    printf("Tiến trình PID = %d, nice = %d bắt đầu\n", getpid(), nice_value);
    
    long long sum = 0;
    for (long long i = 0; i < 0x15; i++) {  // Simulated computation workload
        sum += i;
        printf("Tiến trình PID = %lld kết thúc\n", i);
    }
    sleep(10);
    printf("Tiến trình PID = %d kết thúc, sum = %lld\n", getpid(), sum);
}

int main() {
    pid_t pid1, pid2;

    pid2 = fork();
    if (pid2 == 0) {
        // Child process 2 with nice +10 (lower priority)
        run_process(17);
        exit(0);
    }

    pid1 = fork();
    if (pid1 == 0) {
        // Child process 1 with default nice (0)
        run_process(13);
        exit(0);
    }

    // Wait for both child processes to finish
    wait(NULL);
    wait(NULL);

    printf("Tất cả tiến trình đã kết thúc\n");
    return 0;
}

#endif
