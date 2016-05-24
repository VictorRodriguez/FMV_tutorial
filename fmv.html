<p>Introduction to Function Multiversioning (FMV) in GCC 6</p>

<p>
While new architecture provides many powerful instruction set extensions, it is
challenging for developers to generate code that takes advantage of these
capabilities. Current Linux* developers can't always take advantages of all new
architectures coming to the market every year without losing backward
compatibility.  For example, for math-heavy code one may want to turn on
Intel&reg;
<a href="https://software.intel.com/en-us/node/513926">AVX</a>, while also
wanting the binary to run on earlier CPUs. This problem stops users from
benefiting from the best of the new computing architecture technology, from
low-level kernel features to complex applications.</p>

<p>Despite the fact that newer versions of GCC and kernel try to provide the tools
to use the new architecture features before the platforms appear in the market,
is not always easy for developers to use them. Currently, C code developers
have few choices to solve this problem: </p>

<ul class="spacylist"> 
    <li>Write multiple versions of their code, each targeting different
    instruction set extensions, and manually handle runtime dispatching of
    these versions</li> 
    <li>Generate multiple versions of their binary, each targeting a different
    platform</li>
    <li>Choose a minimum hardware requirement that will not take advantage of
    newer platforms</li>
</ul> 

<p>Despite these challenges, the benefit of using the new architecture
technology is compelling. For example, consider the second version of the
advanced vector extension instruction set extension (Intel&reg; <a
    href="https://software.intel.com/en-us/node/513926">AVX</a>), introduced in
the 4th Generation Intel&reg; Core&trade; processor family (formerly known as <a
    href="https://software.intel.com/en-us/haswell">Haswell</a>).  The benefits
of using this technology in scientific computing fields are well understood.
Such is the case of the Basic Linear Algebra libraries (OPENBLAS).
The use of AVX2 in OPENBLAS libraries gave projects like R language a boost in
performance of up to <a
    href="https://openbenchmarking.org/embed.php?i=1603014-GA-CLEARUBUN61&amp;sha=8f667cf&amp;p=2">
    2x in execution time</a> and the improvement in <a href=
    "https://01.org/blogs/2016/improving-python-performance-scientific-tools-and-libraries">python
    scientific libraries</a> .</p>

<p>This performance improvements of the Intel&reg; Advanced Vector Extensions 2
(Intel&reg; AVX2) came from doubling the number of FLOPS (floating-point operations
per second) per clock cycle, 256 bit integer instructions, floating-point fused
multiply-add instructions, and gather operations.</p>

<p>This instruction set has proved  significant performance improvements. Some
<a href="https://software.intel.com/en-us/articles/how-intel-avx2-improves-performance-on-server-applications">publications</a>
shows how with new Intel&reg; AVX2 instructions and the 256 bit registers on Intel
E5 processor family, LINPACK was able to take advantage of the new instructions
to achieve over 2x performance in comparison to running
LINPACK with SSE instructions and over 1.3x performance against LINPACK running
with AVX instructions.</p>

<p>However, the use of the vector extensions technology means a lot of work in
terms of development, deployment and manageability in the long term. The idea
of having multiple versions of their binaries, one for each architecture,
discourages the developers and OS distributions to support these features. With
the advances in computing architecture growing every year, this seems like a
herculean task.</p>

<p>It would be better to optimize some key hot functions for multiple
architectures and execute them when the binary detects the CPU capabilities at
runtime. This feature exists since  GCC 4.8 only for C++ and is known as
Function Multi Versioning (FMV). FMV in GCC 4.8 makes it easy for the developer
to specify multiple versions of a function, each catered to a specific target
ISA feature. GCC then takes care of creating the dispatching code necessary to
execute the right function version.</p>

<p> If we want to use FMV in our C++ code we might specify multiple versions of
a function. For example in the code presented in the GCC 4.8 <a
    href="https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Function-Multiversioning.html">
</a> :</p>

<pre>   
__attribute__ ((target ("sse4.2")))
int foo (){
    // foo version for SSE4.2
    return 1;
}
__attribute__ ((target ("arch=atom")))
int foo (){
    // foo version for the Intel ATOM processor
    return 2;
}

int main () {
    int (*p)() = &foo;
    assert ((*p) () ==foo ());
    return 0;
}
</pre>

<p>Here, on each function the developer need to create specific functions and code
for each target. This means a lot of overhead in the code. FMV increase the
number of lines of code in a program, its maintainability and manageability.</p>

<p>On the other hand, the new GCC 6 solves this problems. It supports Function
Multi Versioning in C  and C++ code with a single attribute that define the
minimum set of architectures to support, making it easier to develop Linux
applications that take advantage of the enhanced instructions of the new
architectures without the overhead of replicate the functions for each
target.</p>

<p>A simple example where FMV can take advantage of the AVX technology could be
represented with an arrays addition (known as array_adition.c for future
references) : </p>

<pre>    
    #define MAX 1000000
    int a[256], b[256], c[256];

    __attribute__((target_clones("avx2","arch=atom","default")))
    void foo(){
        int i,x;
            for (x=0; x&lt;MAX; x++){
                for (i=0; i&lt;256; i++){
                    a[i] = b[i] + c[i];
                }
            }
     }
    int main () {
        foo();
        return 0;
    }
</pre>

<p>As we can see, the selection of the supported architecture is pretty simple.
The developer just needs to select the minimum set of architectures to support,
such as: AVX2, Intel&reg; Atom&trade;, AMD&reg; or almost every architecture option that GCC
accepts from command line. At the end, the object dump of this code will have
the optimized assemble instructions for each architecture, for example:</p>

<pre>
None AVX code :

add    %eax,%edx

Intel® SSE :

vmovdqa 0x0(%rax),%xmm0
vpaddd 0x0(%rax),%xmm0,%xmm0
vmovaps %xmm0,0x0(%rax)
 
Intel&reg; AVX2 :

vmovdqu (%r8,%rax,1),%ymm0
vpaddd (%r9,%rax,1),%ymm0,%ymm0
vmovdqu %ymm0,(%rdi,%rax,1)

</pre>

<p>As shown in this example,  the FMV technology provides to our array_adition.c
with the capabilities to use registers and instructions to run in Intel&reg; AVX2,
Intel&reg; AVX2, Intel&reg; SSE and even Atom platforms. This capability  increases
the range of platforms where our application can run without illegal
instructions errors as well as exploit the hardware capabilities of the
platform where our code is executed.</p>

<p>Normally before GCC6, telling the compiler to use Intel&reg; AVX2 instructions
would limit our binary to Haswell and newer processors. With FMV, the compiler
can generate AVX-optimized versions of the code and will automatically, at
runtime, ensure that only the appropriate versions are used. In other words,
when the binary is run on Haswell or later generation CPUs, it will use
Haswell-specific optimizations, and when that same binary is run on a
pre-Haswell generation processor, it will fall back to using the standard
instructions supported by the older processor. The object dump will have the
three kinds of instructions sets and registers available.</p>

<p><strong>CPUD selection</strong></p> 

<p>In <a href="https://gcc.gnu.org/wiki/FunctionMultiVersioning"> GCC4 version of
    FMV</a> there was a  dispatch priority instead of a CPUID selection . The order in
which the versions should be dispatched is prioritized to each function version
based on the target attributes. Function versions with more advanced features
get higher priority. For example a version targeted for AVX2 will have a higher
dispatch priority than a version targeted for SSE2.</p>

<p>To keep the cost of dispatching low, the IFUNC mechanism is used for
dispatching. The GNU indirect function support (IFUNC) is a feature of the GNU
toolchain that allows a developer to create multiple implementations of a given
function and to select amongst them at runtime using a resolver function. The
resolver function is called by the dynamic loader during early startup to
resolve which of the implementations will be used by the application. Once an
implementation choice is made it is fixed and may not be changed for the
lifetime of the process.</p>

<p>In GCC6 the resolver check CPUID and then call corresponding function. It does
this once per binary execution.It means if you have multiple calls to the FMV
function, only the first call will execute the CPUID comparison, the incoming
executions will call the required version by pointer. This technique is already
used for almost all GLIBC functions. The GLIBC has memcpy optimize for each
architecture, so when you call memcpy, GLIBC will call optimized memcpy
functions for each architecture.</p>

<p><strong>Code size impact</strong></p>

<p> How FMV will increase binary code size? The answer on this question depends on
how big are functions where FMV will be applied and number of requested
versions. Let consider “N” be number of requested versions (including default)
and “R” the ratio of the functions code to the whole application code size.
When new size should be:</p>

<p> (1 – R) * C + R * C * N </p> 

<p>Where “C” is initial binary code size. With FMV the code
size increase should be : </p>

<p> ((1 – R) * C + R * C * N) / C = 1 + R *(N - 1) </p>

<p> Say, we have an application where hottest part takes 1% of the whole code size.
When we apply FMV to this part to support 3 architecture features(“default”,
“sse4.2”, “avx2”) the overall code size increase will be: </p>

<p> 1 + (0.01 * (3 – 1)) = 1.02 or just 2% </p>


<p>With today capacity of high volume hard disks the impact in the code size is
relative to the application and infrastructure where the application will run.
From data centers to embedded and IoT applications the importance of the code
size impact against the performance improvement and portability is personal
decision for each project.</p>

<p><strong>Results</strong></p>

<p>The following table shows that on a Haswell platform, the FMV version of the
binary executes with the same performance as a binary build with just AVX2
instructions. On older processors, the same binary would still work, but the
performance would be lower due to the lack of optimized instructions.</p>

<table>

<tbody>
  <tr>
    <th></th>
    <th colspan="6">Execution Time (ms) </th>
 </tr>

  <tr>
    <th> GCC flags </th>
    <th> Intel&reg; Core&trade; <br> i7-4700MQ CPU  <br> 2.40GHz <br> (Haswell)</th>
    <th> Intel&reg; Core&trade; <br> i5-6260U CPU   <br> 1.80GHz <br> (Skylake)</th>
    <th> Intel&reg; Core&trade; <br> i7-5557U CPU   <br> 3.10GHz <br> (Broadwell)</th>
    <th> Intel&reg; Xeon&reg;   <br> E5-2603 v3 CPU <br> 1.60GHz <br> (Xeon)</th>
    <th> Intel&reg; Atom&trade; <br> C2750 CPU      <br> 2.40GHz <br> (Atom) </th>
    <th> Intel&reg; Core&trade; <br> i7-4960X CPU   <br> 3.60GHz <br> (Ivy bridge) </th>
  </tr>

  <tr>
    <td > gcc array_adition.c</td>
    <td align="center"> 603   </td>
    <td align="center"> 645   </td>
    <td align="center"> 580   </td>
    <td align="center"> 1413  </td>
    <td align="center"> 2369  </td>
    <td align="center"> 517   </td>
 </tr>


  <tr>
    <td > gcc -O3 array_adition.c</td>
    <td align="center"> 38  </td>
    <td align="center"> 44  </td>
    <td align="center"> 37  </td>
    <td align="center"> 107 </td>
    <td align="center"> 96  </td>
    <td align="center"> 60  </td>
 </tr>

  <tr>
    <td > gcc -O3 array_adition.c -mavx</td>
    <td align="center"> 26 </td>
    <td align="center"> 32 </td>
    <td align="center"> 26 </td>
    <td align="center"> 73 </td>
    <td align="center"> Illegal instruction (core dumped) </td>
    <td align="center"> 45 </td>
  </tr>

  <tr>
    <td > gcc -O3 array_adition.c -mavx2</td>
    <td align="center"> 26 </td>
    <td align="center"> 32 </td>
    <td align="center"> 26 </td>
    <td align="center"> 73 </td>
    <td align="center"> Illegal instruction (core dumped) </td>
    <td align="center"> Illegal instruction (core dumped) </td>
 </tr>


  <tr>
   <td > gcc -O3 array_adition.c ( with FMV
       )<br>__attribute__((target_clones("avx2","arch=atom","default")))</td>
    <td align="center"> 26 </td>
    <td align="center"> 32 </td>
    <td align="center"> 26 </td>
    <td align="center"> 73 </td>
    <td align="center"> 96 </td>
    <td align="center"> 45 </td>
 </tr>

</tbody>
</table>

<p> The *default flags are: </p>

<pre>
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib64/gcc/x86_64-generic-linux/6.1.0/lto-wrapper
Target: x86_64-generic-linux
../gcc-6.1.0/configure --prefix=/usr --with-pkgversion='Clear Linux OS for
Intel Architecture' --libdir=/usr/lib64 --enable-libstdcxx-pch
--libexecdir=/usr/lib64 --with-system-zlib --enable-shared
--enable-gnu-indirect-function --disable-vtable-verify --enable-threads=posix
--enable-__cxa_atexit --enable-plugin --enable-ld=default --enable-clocale=gnu
--disable-multiarch --disable-multilib --enable-lto --enable-linker-build-id
--build=x86_64-generic-linux --target=x86_64-generic-linux
--enable-languages=c,c++,fortran,go --enable-bootstrap --with-ppl=yes
--with-isl --includedir=/usr/include --with-gxx-include-dir=/usr/include/c++/
--exec-prefix=/usr --with-glibc-version=2.19 --with-system-libunwind
--with-gnu-ld --with-tune=haswell --with-arch=westmere --enable-libmpx
Thread model: posix
gcc version 6.1.0 (Clear Linux OS for Intel Architecture)

</pre>


<p><strong>Real world example</strong></p>

<p>Today more and more industry segments are benefiting from the use of cloud
scientific computing. These segments include chemical engineering, digital
content creation, and financial service and analytics applications among
others. As we can see, the importance of improving the performance of
scientific computing libraries is mandatory for a cloud OS.</p>

<p>One of these scientific computing libraries is the NumPy library. NumPy is the
fundamental package for scientific computing with Python. Among other things,
it includes support for large, multidimensional arrays and matrices. This
library has special features for linear algebra, Fourier transform, and random
number capabilities among others.</p>

<p>The advantages of using FMV technology in a scientific library as NumPy is well
understood and accepted. If vectorization is not enabled, there will be a lot
of unused space in SIMD registers. If vectorization is enabled, the compiler
may use the additional registers to perform more operations (in our example the
additions of more integers) in a single instruction. As we can see in the
following table, the performance boost thanks to the FMV technology (running in
a Haswell machine with AVX2 instructions) can be up to 3% in terms of execution time for scientific
workloads. The benchmark that is used in this is example is the one from the
openbenchmarking, the  <a href="https://openbenchmarking.org/test/pts/numpy">numpy-1.0.2 </a> test </p>

<table>
<tbody>
    <tr> 
        <td> <strong>GCC flags</strong> </td>
        <td> <strong>Execution Time</strong></td>
    </tr>
    <tr>
        <td><strong>Numpy with -O3</strong></td>
        <td> 8600 seconds </td>
    </tr>
    <tr>
        <td><strong>Numpy with FMV patches</strong></td>
        <td>8400 seconds </td> 
    </tr>
</tbody>
</table>

<p><strong>Next steps</strong></p>

<p>The <a href="https://clearlinux.org/">Clear Linux Project for Intel
    Architecture</a> is focusing on applying FMV technology on packages where
it is detected that AVX instructions can give an improvement ( AVX / Avx2
/ AVX-512) . In order to solve the problems of supporting FMV in a full Linux
distribution, Clear Linux Project for Intel Architecture provides a patch
generator based on vectorization-candidate detection (gcc flag -fopt-info-vec).
This tool can provide all the FMV patches that a Linux distribution might use.
We are selecting the ones that gave a significant performance improvement based
on our benchmarks. There are other compiler optimization techniques that take
advantage of the profile data to perform additional optimizations based on how
the code behaves. Clear Linux Project for Intel Architecture will use these
profiling features as well as the FMV approach to improve the performance of
user applications as much as possible.</p>
