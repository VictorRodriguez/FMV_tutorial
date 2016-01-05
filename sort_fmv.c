#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX 1000000

static struct timeval tm1;
int a[256], b[256], c[256];

void foo();
void foo_1();
void bubble_sort();

static inline void start()
{
    gettimeofday(&tm1, NULL);
}

static inline void stop()
{
    struct timeval tm2;
    gettimeofday(&tm2, NULL);

    unsigned long long t = 1000 * (tm2.tv_sec - tm1.tv_sec) +\
                           (tm2.tv_usec - tm1.tv_usec) / 1000;
    printf("%llu ms\n", t);

}

int main(){
    start();
    foo();
    bubble_sort(a,256);
    foo_1();
    stop();
    return 0;
}

__attribute__((ctarget("arch=core-avx2","arch=atom","arch=slm","default"),noinline))
void foo(){
    int i,x;
    for (x=0; x<MAX; x++){
        for (i=0; i<256; i++){
            a[i] = b[i] + c[i];
        }
    }
}


__attribute__((ctarget("arch=core-avx2","arch=atom","arch=slm","default"),noinline))
void bubble_sort (int *a, int n) {
    int i, t, s = 1;
    while (s) {
        s = 0;
        for (i = 1; i < n; i++) {
            if (a[i] < a[i - 1]) {
                t = a[i];
                a[i] = a[i - 1];
                a[i - 1] = t;
                s = 1;
            }
        }
    }
}


__attribute__((ctarget("arch=core-avx2","arch=atom","arch=slm","default"),noinline))
void foo_1(){
    int i,x;
    for (x=0; x<MAX; x++){
        for (i=0; i<256; i++){
            a[i] = b[i] - c[i];
        }
    }
}
