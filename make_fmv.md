# Tutorial : How to use Function Multi Version patch generator

CPU architectures often gain interesting new instructions as they evolve, but
application developers often find it difficult to take advantage of those
instructions. Reluctance to lose backward-compatibility is one of the main
roadblocks slowing developers from using advancements in newer computing
architectures. Function multi-versioning (FMV), which first appeared in GCC
4.8, is a way to have multiple implementations of a function, each using a
different architecture's specialized instruction-set extensions. GCC 6
introduces changes to FMV that make it even easier to bring architecture-based
optimizations to application code.

This tutorial show how to use FMV on a general code and on FFT library code. In
the end the user will know how to use this technology on their code and used
libraries to deploy architecture-based optimizations to application code.

## Install and configure a Clear Linux Host on bare metal

First, follow our instructions to install [Clear Linux on bare
metal](https://clearlinux.org/documentation/clear-linux/get-started/bare-metal-install/bare-metal-install.html#bare-metal-install)
Once the bare metal installation and initial configuration are complete, you
will have all you need to run this tutorial example

## Detect loop vectorization candidates

Now we need to detect the loop vectorization candidates to be cloned for
multiple platforms with FMV. As an example we will use the following simple C
code: 

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX 1000000

int a[256], b[256], c[256];

void foo(){
    int i,x;
    for (x=0; x<MAX; x++){
        for (i=0; i<256; i++){
            a[i] = b[i] + c[i];
        }
    }
}


int main(){
    foo();
    return 0;
}
'''

After saving the code with the name example.c, we can build with the following
flags: 

'''
    gcc -O3  -fopt-info-vec  example.c -o example
'''

This will generate the following output:

'''
example.c:11:9: note: loop vectorized
example.c:11:9: note: loop vectorized
'''

As we can see on the source code that specific line 11 is a good candidate for
vectorization:

'''c
for (i=0; i<256; i++){
    a[i] = b[i] + c[i];

'''

In order to genearte the FMV patch with the project
[make-fmv-patch](https://github.com/clearlinux/make-fmv-patch) we need to clone
it and generate a log file with the loop vectorized information: 

'''
    git clone https://github.com/clearlinux/make-fmv-patch.git
    gcc -O3  -fopt-info-vec  example.c -o example &> log
'''

In order to generate the patch files we have to execute:

'''
    perl ./make-fmv-patch/make-fmv-patch.pl log .
'''

The make-fmv-patch.pl take two arguments: 

'''
    perl make-fmv-patch.pl <buildlog> <sourcecode>
'''

This will generate the patch: example.c.patch

'''
--- ./example.c	2017-09-27 16:05:42.279505430 +0000
+++ ./example.c~	2017-09-27 16:19:11.691544026 +0000
@@ -5,6 +5,7 @@

 int a[256], b[256], c[256];

+__attribute__((target_clones("avx2","arch=atom","default")))
 void foo(){
     int i,x;
     for (x=0; x<MAX; x++){
'''

We can see that make-fmv-patch is recomended to add the atribute that generates
target clones on the function foo. When we do this is possible to have the
folllwoing code: 

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#define MAX 1000000

int a[256], b[256], c[256];

__attribute__((target_clones("avx2","arch=atom","default")))
void foo(){
    int i,x;
    for (x=0; x<MAX; x++){
        for (i=0; i<256; i++){
            a[i] = b[i] + c[i];
        }
    }
}


int main(){
    foo();
    return 0;
}
'''

We can change the target clones when we add the patches or in the
make-fmv-patch.pl script , changing the value of this variable:

'''perl
my $avx2 = '__attribute__((target_clones("avx2","arch=atom","default")))'."\n";
'''

When we compile again the code with FMV and the capability to analize the
objdump: 

'''
gcc -O3 example.c -o example -g
objdump -S example | less
'''

We will be able to see that there are multiple clones for the foo funciton: 

'''
foo
foo.avx2.0
foo.arch_atom.1
'''
The avx2 funciton use specific AVX2 registers: 

'''
vmovdqu %ymm0,(%rcx,%rax,1)
'''


Congratulations!  You have successfully installed an FMV  development
environment  on Clear Linux. Furthermore, you use cutting edge compiler
technology to improve the performance of your application based on IA
technology and profiling of the specific execution of your application. 

