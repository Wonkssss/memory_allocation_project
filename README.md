# memory_allocation_project

This C implementation provides a custom memory allocator with functions for dynamic memory allocation (`myMalloc`), deallocation (`myFree`), reallocation (`myRealloc`), and calloc (`myCalloc`). The allocator uses a linked list to manage free memory blocks.

## Overview

- **initAllocator:** Initializes the allocator with a specified initial size using `sbrk` system call.

- **myMalloc:** Allocates memory of the specified size. If no suitable block is found in the free list, it increases the memory size using `sbrk`.

- **myFree:** Frees the memory block pointed to by `ptr`, adding it back to the free list.

- **myRealloc:** Changes the size of the memory block pointed to by `ptr` to `new_size`. If the new size is larger, it allocates a new block and copies the data.

- **myCalloc:** Allocates memory for an array of elements, initializing the memory to zero.

- **printFreeList:** Prints the current state of the free list, displaying the sizes of free memory blocks.
