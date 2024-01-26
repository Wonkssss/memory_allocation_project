#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct Block { 
    size_t size;
    struct Block* next; 
} Block; 

static Block* freeList = NULL; 

void initAllocator(size_t initialSize){ 
    freeList = sbrk(initialSize); 
    if (freeList == (void*)-1) {
        perror("initAllocator - sbrk");
        exit(EXIT_FAILURE);
    }
    if(freeList != (void*)-1){
        freeList->size = initialSize - sizeof(Block);
        freeList->next = NULL; 
    }
}

void* myMalloc(size_t size){
    if (freeList == NULL){
        initAllocator(4096);
    }

    // find a free block of sufficient size
    Block* current = freeList;
    Block* previous = NULL;

    while (current!=NULL && current->size < size){
        previous = current;
        current = current->next;
    }

    if (current==NULL){
        // no block available, so let's increase memory available
        current = sbrk(size + sizeof(Block)); 

        if (current == (void*)-1){
            printf("Memory allocation failed...\n");
            perror("myMalloc - sbrk");
            return NULL;
        }
        current->size = size;
        current->next = NULL;
    }
    else{
        // split the block if larger than needed
        if (current->size > size + sizeof(Block)){
            Block* newBlock = (Block*)((char*) current + size + sizeof(Block)); //start of the new point 
            newBlock->size = current->size - size - sizeof(Block);
            newBlock->next = current->next; 
            current->next = newBlock; 
            current->size = size; 
        }

        //remove the allocated block from the free list
        if (previous!=NULL){
            previous->next = current->next; // directly points to the next one skipping current that allocated memory and that is no longer available
        }
        else{
            freeList = current->next;
        }
    }

    // Return a pointer to the user data (skip the block header)
    return (void*)(current + 1); //+1 to skip the header and go directly to the user data 
    
}

void myFree(void* ptr){
    if (ptr==NULL){return;}
    Block* block = (Block*)((char*)ptr - sizeof(Block)); //block that will be inserted to freeList (to get the memory that was allocated back)
    block->next = freeList; 
    freeList = block; //freelist points at the block, which is the recent memory block that was unallocated
}

void* myRealloc(void* ptr, size_t new_size){
    
    void* newPtr;

    Block* oldBlock = (Block*)((char*)ptr - sizeof(Block)); 

    if (new_size > oldBlock->size){
        newPtr = myMalloc(new_size);
        memcpy(newPtr, ptr, oldBlock->size);
        //myFree(ptr);
        printf("debogger 5\n");
        return newPtr;
    }
    return ptr;
    
}

void* myCalloc(size_t n, size_t elementSize){
    size_t neededSize = n * elementSize; // or totalSize
    void* ptr = myMalloc(neededSize); // allocate the needed memory
    if (ptr!=NULL){
        memset(ptr, 0, neededSize);
    }
    return ptr;
}

void printFreeList(){
    printf("freeList: ");
    Block* current = freeList;
    while (current!=NULL){
        printf("[%zu] -> ", current->size);
        if (current == current->next) {
            printf("Infinite loop detected!\n");
            break;
        }
        current = current->next;
    }

    printf("NULL\n"); 
}

int main(){

    void* ptr1 = myMalloc(96);
    printf("Malloc(96): ");
    printFreeList();
    void* ptr2 = myMalloc(100);
    printf("Malloc(100): ");
    printFreeList();
    myFree(ptr2);
    printf("freeList(ptr2): ");
    printFreeList();
    myFree(ptr1);
    printf("freeList(ptr1): ");
    printFreeList();

    void* ptr3 = myRealloc(ptr1, 100);
    printf("Realloc(100): ");
    printFreeList();

    myFree(ptr3);
    printf("freeList(ptr3): ");
    printFreeList();

    int* arr = (int*)myCalloc(5, sizeof(int));
    printf("Calloc(5 integers): ");
    printFreeList();

    return EXIT_SUCCESS;
}