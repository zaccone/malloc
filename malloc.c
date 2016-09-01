#include <unistd.h>
#include <assert.h>

struct block {
	struct block *next;
	size_t size;
	int free;
	int magic;
};

#define BSIZE    sizeof(struct block)
static struct block *chunks = NULL;

struct block *allocate_chunk(size_t size);

void *malloc(size_t size)
{
	if (!chunks) {
		chunks = allocate_chunk(size);
	} else {
		struct block *current = chunks;
		while (current && (current->free == 0 || current->size < size)) {
			current = current->next;
		}

		if (current && current->free == 1 && current->size >= size) {
			current->free = 0;
			return (current + 1);
		} else {
			struct block *new_chunk = allocate_chunk(size);
			new_chunk->next = chunks;
			chunks = new_chunk;
		}
	}
	return (chunks + 1);
}

struct block *allocate_chunk(size_t size)
{
	void *top = sbrk(0);
	void *memory = sbrk(size + BSIZE);
	assert(top == memory);
	struct block *b = (struct block *)memory;
	b->free = 1;
	b->next = NULL;
	b->size = size;
	return memory;
}
