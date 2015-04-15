#define NULL 0
#define HEAP_SIZE 8000

unsigned char heap[HEAP_SIZE];

typedef struct heapCell {
	int size;
	unsigned char free;
	struct heapCell *next;
} *heapPtr;

heapPtr heapBase;

heapPtr initHeap() {
	
	heapBase = (heapPtr)heap;
	heapBase->size = HEAP_SIZE - sizeof(struct heapCell);
	heapBase->free = 1;
	heapBase->next = heapBase;
	
	return heapBase;
}

/***
	allocates a segment of heap memory and returns a pointer to that segment
	Parameter:
		size - bytes of memory to allocate
	Returns a pointer to a memory segment or NULL if not enough memory
***/
void *getMem( unsigned int size ) {
	
	int currSize;
	heapPtr temp = heapBase;
	heapPtr retPtr;
	
	/*while not at a free and spacially sufficient cell or last heap cell*/
	while( !(temp->free && temp->size >= size ) && temp->next != heapBase ) { 
		temp = temp->next;
	}
	
	/*if not enough memory*/
	if( temp->size < size ) {
		return NULL;
	} else {
		currSize = temp->size; /*store size*/
		
		/*update remaining memory size*/
		currSize -= ( size + sizeof(struct heapCell) ); 
		
		temp->size = size; /*update size*/
		temp->free = 0; /*set to occupied*/
		retPtr = temp + 1; /*go to next free memory address*/
		
		/*go to next cell*/
		temp->next = (heapPtr)(temp->next + sizeof(struct heapCell) + size); 
		temp = temp->next; /*go to next heapCell header*/
		temp->size = currSize; /*put new size*/
		temp->free = 1; /*set to free*/
		temp->next = heapBase; /*retain circular linked list*/
		
		return retPtr;
	}
}
