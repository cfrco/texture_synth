all: libsynth.so
libsynth.so : synth.o
	gcc -shared -Wl,-soname,libsynth.so.1 synth.o -o libsynth.so

synth.o : synth.c
	gcc -O3 -c -fPIC synth.c -o synth.o
