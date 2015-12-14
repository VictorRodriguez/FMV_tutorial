#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX 1000000

static struct timeval tm1;
int a[256], b[256], c[256];

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
__attribute__((ctarget("arch=core-avx2","arch=corei7-avx","arch=slm","default"),noinline))
void foo(){
    int i,x;
    for (x=0; x<MAX; x++){
        for (i=0; i<256; i++){
            a[i] = b[i] + c[i];
        }
    }
}


int main(){
    start();
    foo();
    stop();
    return 0;
}
