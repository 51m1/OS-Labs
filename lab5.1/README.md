Linux provides applications with a clean view of a huge directly addressable memory space. Additionally, it provides protection so that different applications are protected from each other, and it allows applications to apparently access more memory than is physically present in the machine, provided the machine is at least well configured and has enough swap space.

You allocate memory using the malloc call in the standard C library:

#include <stdlib.h> void *malloc(size_t size);

The malloc function is guaranteed to return memory that is aligned so that it can be cast to a pointer of any type.

In the first example code the program asks the malloc library to give it a pointer to a megabyte of memory. You check to ensure that malloc was successful and then use some of the memory to show that it exists. When you run the program, you should see Hello World printed out, showing that malloc did indeed return the megabyte of usable memory.

The next program asks to allocate somewhat more memory than is physically present in the machine, so you might expect malloc to start failing somewhere a little short of the actual amount of memory present, because the kernel and all the other running processes are using some memory. The surprise is that it works at all, because we appear to have created a program that uses every single byte of physical memory on the author’s machine. The other interesting feature is that the program ran in the blink of an eye. After some extra tests the system protects itself from this rather aggressive program and kills it. On some systems it may simply exit quietly when malloc fails. Initially, the kernel was simply able to use free physical memory to satisfy the application’s request for memory, but once physical memory was full, it started using what’s called swap space.

Suppose you try to do “bad” things with memory. In this exercise, you allocate some memory and then attempt to write past the end.

The output is simply

$ /memory4 Segmentation fault

The Linux memory management system has protected the rest of the system from this abuse of memory. To ensure that one badly behaved program can’t damage any other programs, Linux has terminated it.

Modern Linux systems are very protective about writing or reading from the address referred to by a null pointer, although the actual behavior is implementation-specific.

A call to free should be made only with a pointer to memory allocated by a call to malloc, calloc, or realloc.

The last program simply shows how to call free with a pointer to some previously allocated memory.

Although calloc allocates memory that can be freed with free, it has somewhat different parameters from malloc: It allocates memory for an array of structures and requires the number of elements and the size of each element as its parameters. The allocated memory is filled with zeros; and if calloc is successful, a pointer to the first element is returned. Like malloc, subsequent calls are not guaranteed to return contiguous space, so you can’t enlarge an array created by calloc by simply calling calloc again and expecting the second call to return memory appended to that returned by the first call.

The realloc function changes the size of a block of memory that has been previously allocated. It’s passed a pointer to some memory previously allocated by malloc, calloc, or realloc and resizes it up or down as requested. The realloc function may have to move data around to achieve this, so it’s important to ensure that once memory has been realloced, you always use the new pointer and never try to access the memory using pointers set up before realloc was called.
