TextureSynthesis
================
This is a homework in my Introduction to MultiMedia Information Systems course.  
The code and Algorithms may not correct.

Compile
---------
<pre><code>$ gcc -O3 -c -fPIC synth.c -o synth.o
$ gcc -shared -Wl,-soname,libsynth.so.1 -o libsynth.so synth.o</code></pre>
or just
<pre><code>$ make</code></pre>

Reference
---------
http://graphics.cs.cmu.edu/people/efros/research/EfrosLeung.html  
http://www.rubinsteyn.com/comp_photo/texture/  
