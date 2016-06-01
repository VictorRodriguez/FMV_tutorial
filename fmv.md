<h1>Introduction to Function Multi-Versioning (FMV) in GCC 6 <h1>

<p>While the latest architectures offer powerful instruction set
extensions, developers may find it challenging to integrate an existing
code base with a new architecture. Reluctance to lose backward-compatibility
is one of the main roadblocks slowing developers from realizing advancements
in newer computing architectures. With FMV in GCC 4.8, the overhead for backward
compatibility was rather large, but GCC 6 introduces changes that make it
easier to bring architectural-based optimizations to your code.</p>

<p>Newer versions of GCC and the kernel attempt to expose tools for using the
new architecture features before the platforms appear in the market; however, it can be
tough for developers to use them. Currently, C code developers have a few choices:</p>


<ul class="spacylist"> 
    <li>Write multiple versions of their code, each targeting different instruction set
        extensions; this requires they also manually handle runtime dispatching of
        these versions.</li> 
    <li> Generate multiple versions of their binary, each targeting a different
        platform.</li>
    <li>Choose a minimum hardware requirement that doesn't take advantage of
        technology in newer platforms. </li>
</ul> 


<p>Oftentimes, the benefits of using the new architecture's technologies are compelling
enough to outweigh any integration challenges. Math-heavy code, for example, can be
significantly optimized by turning on <a href="https://software.intel.com/en-us/node/513926">Advanced Vector
    Extensions (AVX)</a> 
technology.  The second version of the AVX set extension (Intel <a
    href="https://software.intel.com/en-us/node/513926">AVX2</a>)),
introduced in the 4th-Gen Intel Core processor family AKA <a
    href="https://software.intel.com/en-us/haswell">Haswell</a>)
is one option with compelling reasons to do the work. The benefits of AVX2 are well understood
in scientific computing fields.  The use of AVX2 in OPENBLAS libraries can give a project like R
language a boost up to 
<a href="https://openbenchmarking.org/embed.php?i=1603014-GA-CLEARUBUN61&amp;sha=8f667cf&amp;p=2">
2X faster execution time</a>; it can also 
yield significant improvement in <a href=
    "https://01.org/blogs/2016/improving-python-performance-scientific-tools-and-libraries">python
    scientific libraries</a></p>

<p>These performance improvements are gained from doubling the number of FLOPS (floating-point operations
per second) per clock cycle, using 256-bit integer instructions, floating-point fused
multiply-add instructions, and gather operations.</p>

<p>However, the use of Vector eXtensions (VX) technology means a lot of work in
terms of development, deployment and manageability. The idea of having to maintain
multiple versions of binaries (one for each architecture) discourages developers and OS
distributions from supporting these features.</p>

<p> Would it be better to optimize some key hot functions for multiple architectures and
execute them when the binary detects the CPU capabilities at runtime? Actually, this feature has existed
since GCC 4.8 (but only for C++), and has become known as Function Multi-Versioning (FMV). FMV
in GCC 4.8 made it easy for a developer to specify multiple versions of a function; each could be
catered to a specific target ISA feature. GCC then took care of creating the dispatching code necessary
to execute the right function version.</p>

<p> If we want to use FMV in our C++ code we might specify multiple versions of
a function. For example, the code presented in the  
<a
    href="https://gcc.gnu.org/onlinedocs/gcc-4.9.2/gcc/Function-Multiversioning.html"> FMV documentation for GCC 4.8
</a> shows:</p>

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

<p>Here, for each function, the developer needed to create specific functions and code
for each target. Historically, this would have required extra overhead in the code: FMV
increasing the number of lines of code in a program makes it more clunky to manage and maintain.</p>

<p>Fortunately, GCC 6 solves this issue: it supports Function Multi-Versioning in <strong>both</strong> C and C++
code with a <i>single attribute</i> to define the minimum set of architectures to support.  This
actually makes it easier to develop Linux applications that can take advantage of enhanced instructions
of the new architectures, without the overhead of replicate functions for each target.</p>

<p>A simple example where FMV can be written to take advantage of AVX technology features could be
represented with an arrays addition (named <i>array_addition.c</i> for this example): </p>

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
The developer needs only to select the minimum set of architectures to support:
AVX2, Intel Atom, AMD, or almost any architecture option that GCC
accepts from the command line. </p>

<p>Ultimately, the object dump of this code will have the optimized assembly instructions
for each architecture. For example:</p>

<pre>
None AVX code :

add    %eax,%edx

Intel AVX :

vmovdqa 0x0(%rax),%xmm0
vpaddd 0x0(%rax),%xmm0,%xmm0
vmovaps %xmm0,0x0(%rax)
 
Intel AVX2 :

vmovdqu (%r8,%rax,1),%ymm0
vpaddd (%r9,%rax,1),%ymm0,%ymm0
vmovdqu %ymm0,(%rdi,%rax,1)
</pre>

<p>Notice that the new implementation of FMV provides <i>array_addition.c</i> ability to
use registers and instructions for Intel AVX, AVX2, and even Atom platforms. This
capability increases the range of platforms where our application can run without
illegal instruction errors exploiting hardware capabilities of a platform where
our code is executed.</p>

<p>Before GCC 6, telling the compiler to use Intel AVX2 instructions would limit
compatibility of our binary to only Haswell and newer processors. With FMV, the
compiler can generate AVX-optimized versions of the code; and it will automatically,
at runtime, ensure that <i>only</i> the appropriate versions are used. In other words,
when the binary is run on Haswell or later generation CPUs, it will use
Haswell-specific optimizations; and when that <i>same</i> binary is run on a
pre-Haswell generation processor, it will fall back to using the standard
instructions supported by the older processor. The object dump will have the
three kinds of instructions sets and registers available.</p>

<h3>CPUID selection</h3> 

<p>In GCC 4, <a href="https://gcc.gnu.org/wiki/FunctionMultiVersioning"> 
FMV had a dispatch priority rather than a CPUID selection</a>. 
The dispatch order was prioritized to each function version, based on the target
attributes. Function versions with more advanced features got higher priority.
For example, a version targeted for AVX2 would have a higher dispatch priority
than a version targeted for SSE2.</p>

<p>To keep the cost of dispatching low, the IFUNC mechanism is used for
dispatching. The GNU indirect function support (IFUNC) is a feature of the GNU
toolchain that allows a developer to create multiple implementations of a given
function and to select amongst them at runtime using a resolver function. The
resolver function is called by the dynamic loader during early startup to
resolve which of the implementations will be used by the application. Once an
implementation choice is made it is fixed and may not be changed for the
lifetime of the process.</p>

<p>In GCC 6, the resolver checks CPUID and then calls the corresponding function. It
does this once per binary execution. So when you have multiple calls to the FMV
function, only the first call will execute the CPUID comparison; the incoming
executions will call the required version by a pointer. This technique is already
used for almost all GLIBC functions. The GLIBC has ``memcpy`` optimized for each
architecture, so when you call memcpy, GLIBC will call optimized memcpy
functions for each architecture.</p>

<h3>Code size impact</strong></h3>

<p> How FMV will increase binary code size? The answer to this question depends on
how big are functions where FMV will be applied and a number of requested
versions. Let consider <i>N</i> be number of requested versions (including default)
and <i>R</i> the ratio of the functions code to the whole application code size.
When new size should be: </p>

<p> ( 1 - R ) * C + R * C* N </p> 

<p>Where <i>C</i> is initial binary code size. With FMV the code
size increase should be : </p>

<p> ((1 - R) * C + R * C * N) / C = 1 + R *(N - 1) </p>

<p> Say, we have an application where hottest part takes 1% of the whole code size.
When we apply FMV to this part to support 3 architecture features(default,
sse4.2, avx2) the overall code size increase will be: </p>

<p> 1 + (0.01 * (3 - 1)) = 1.02 or just 2% </p>


</pre>

<p>With today's capacity of high volume hard disks, the impact in the code size is
relative to the application and infrastructure where the application will run.
From data centers to embedded and IoT applications, the importance of the code
size impact against the performance improvement and portability must be decided
in each scenario.</p>

<h3>Results</h3>

<p>The following table shows how with FMV you can get the binary still executable
on all architectures, taking the advantages of each instruction set (~1.5 times
faster on most of them, comparing to just -O3 optimization):</p>

<table>

<tbody>
  <tr>
    <th></th>
    <th colspan="6">Execution Time (ms) </th>
 </tr>

  <tr>
    <th> GCC flags </th>
    <th> Intel Core <br> i7-4700MQ CPU  <br> 2.40GHz <br> (Haswell)</th>
    <th> Intel Core <br> i5-6260U CPU   <br> 1.80GHz <br> (Skylake)</th>
    <th> Intel Core <br> i7-5557U CPU   <br> 3.10GHz <br> (Broadwell)</th>
    <th> Intel Xeon <br> E5-2603 v3 CPU <br> 1.60GHz <br> (Xeon)</th>
    <th> Intel Atom <br> C2750 CPU      <br> 2.40GHz <br> (Atom) </th>
    <th> Intel Core <br> i7-4960X CPU   <br> 3.60GHz <br> (Ivy bridge) </th>
  </tr>

  <tr>
    <td > gcc array_addition.c</td>
    <td align="center"> 603   </td>
    <td align="center"> 645   </td>
    <td align="center"> 580   </td>
    <td align="center"> 1413  </td>
    <td align="center"> 2369  </td>
    <td align="center"> 517   </td>
 </tr>


  <tr>
    <td > gcc -O3 array_addition.c</td>
    <td align="center"> 38  </td>
    <td align="center"> 44  </td>
    <td align="center"> 37  </td>
    <td align="center"> 107 </td>
    <td align="center"> 96  </td>
    <td align="center"> 60  </td>
 </tr>

  <tr>
    <td > gcc -O3 array_addition.c -mavx</td>
    <td align="center"> 26 </td>
    <td align="center"> 32 </td>
    <td align="center"> 26 </td>
    <td align="center"> 73 </td>
    <td align="center"> Illegal instruction (core dumped) </td>
    <td align="center"> 45 </td>
  </tr>

  <tr>
    <td > gcc -O3 array_addition.c -mavx2</td>
    <td align="center"> 26 </td>
    <td align="center"> 32 </td>
    <td align="center"> 26 </td>
    <td align="center"> 73 </td>
    <td align="center"> Illegal instruction (core dumped) </td>
    <td align="center"> Illegal instruction (core dumped) </td>
 </tr>
  <tr>
   <td > gcc -O3 array_addition.c ( with FMV
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



<p> The default CFLAGS (nothing special) and configurations are <a href=
    "https://download.clearlinux.org/current/source/SRPMS/clr-rpm-config-35-38.src.rpm">
    specified</a> in Clear Linux for Intel Architecture </p>

<h3>Real world example</strong></h3>

<p>Today more and more industry segments are benefiting from the use of cloud-based
scientific computing. These segments include chemical engineering, digital
content creation, financial services and analytics applications.</p> 

<p>One of the most popular scientific computing libraries is the NumPy library. NumPy
is the fundamental package for scientific computing with Python. Among other things,
it includes support for large, multidimensional arrays and matrices. This
library has special features for linear algebra, Fourier transform, and random
number capabilities among others.</p>

<p>The advantages of using FMV technology in a scientific library such as NumPy are well
understood and accepted. If vectorization is not enabled, a lot of unused space in
SIMD registers goes to waste. If vectorization is enabled, the compiler gets to use the
additional registers to perform more operations (for the additions of more integers, in our
example) in a single instruction. </p>

<p>As we can see in the following table, the performance boost due to FMV technology (running in
a Haswell machine with AVX2 instructions) can be up to 3 percent in terms of execution time for scientific
workloads. The benchmark that is used in this is example is the one from 
 <a href="https://openbenchmarking.org/test/pts/numpy">openbenchmarking numpy-1.0.2</a> test. </p>

<table>
    <tbody>
    <tr>
        <th></th>
        <th> Execution Time (seconds) </td>
    </tr>
    <tr> 
        <th> GCC flags </td>
        <th>  Intel Core i5-6260U @ 1.80GHz</td>
    </tr>
    <tr>
        <td>Numpy with -O3</td>
        <td align="center"> 8600 </td>
    </tr>
    <tr>
        <td>Numpy with FMV patches</td>
        <td align="center">8400 </td> 
    </tr>
    </tbody>
</table>

<p>This performance improvement is due to the functions in numpy code that benefit
from vectorization. In order to detect these functions, GCC
provides the flag ``-fopt-info-vec`` . This flag detects the
vectorization-candidate functions. For example, building numpy with this flag
will tell us that the file ``fftpack.c`` has code that can use vectorization: </p>

<pre>
numpy/fft/fftpack.c:813:7: note: loop peeled for vectorization to enhance alignment
</pre>

<p>When we check the 
<a href="https://github.com/numpy/numpy/blob/master/numpy/fft/fftpack.c">numpy
    source code </a>, it's easy to see how the ``static void radfg`` function performs heavy array additions
that use AVX technology.</p>

<h3>Next steps</strong></h3>

<p>The 
<a href="https://clearlinux.org/">Clear Linux Project for Intel
    Architecture</a> is currently focusing on
applying FMV technology to packages where it is detected that AVX instructions can yield an improvement
(AVX / AVX2 / AVX-512).  To solve some of the issues involved with supporting FMV in a full Linux
distribution, Clear Linux Project for Intel Architecture provides a 
<a href="https://download.clearlinux.org/current/source/SRPMS/clr-rpm-config-35-38.src.rpm"> patch generator </a> based
on vectorization-candidate detection (using the ``-fopt-info-vec`` flag). This tool can provide all
the FMV patches that a Linux distribution might use.  We are selecting the ones that gave a significant
performance improvement based on our benchmarks. There are other compiler optimization techniques that take
advantage of the profile data to perform additional optimizations based on how the code behaves. Clear
Linux Project for Intel Architecture will use these profiling features as well as the FMV approach to
improve the performance of user applications as much as possible.</p>
