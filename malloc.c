#include <unistd.h>
#include <assert.h>
#include <string.h>

struct block {
	struct block *next;
	size_t size;
	int free;
};

#define BSIZE    sizeof(struct block)
#define USED    0x1234

static struct block *chunks = NULL;

struct block *allocate_chunk(size_t size);
struct block *get_block_ptr(void *ptr);
void *malloc(size_t size)
{

	if (size <= 0)
		return NULL;
	if (!chunks) {
		chunks = allocate_chunk(size);
		if (!chunks)	// sbrk didn't suceed
			return NULL;
	} else {
		struct block *current = chunks;
		while (current && (current->free == 0 || current->size < size)) {
			current = current->next;
		}

		if (current && current->free == 1 && current->size >= size) {
			current->free = 0;
			assert(current->free == 0);
			return (current + 1);
		} else {
			struct block *new_chunk = allocate_chunk(size);
			new_chunk->next = chunks;
			chunks = new_chunk;
		}
	}
	return (chunks + 1);
}

void free(void *ptr)
{
	struct block *b = get_block_ptr(ptr);
	if (!ptr)
		return;
	assert(b->free == 0);
	b->free = 1;
	assert(b->free == 1);
	return;
}

void *calloc(size_t nemb, size_t size)
{
	size_t s = nemb * size;
	void *ptr = malloc(s);
	if (!ptr)
		return NULL;
	memset(ptr, 0, s);
	return ptr;
}

void *realloc(void *ptr, size_t size)
{
	if (!ptr) {
		return malloc(size);
	}

	struct block *b = get_block_ptr(ptr);
	if (!b)
		return NULL;
	// we have enough space
	if (b->size >= size)
		return ptr;
	else {
		// need to allocate new memory chunk
		void *new_chunk = malloc(size);
		if (!new_chunk) {
			free(ptr);
			return NULL;
		}
		struct block *b = get_block_ptr(ptr);
		memcpy(new_chunk, ptr, b->size);
		assert(b->free == 0);
		free(b);
		assert(b->free == 1);
		return new_chunk;
	}
}

struct block *allocate_chunk(size_t size)
{
	void *top = sbrk(0);
	void *memory = sbrk(size + BSIZE);
	assert(top == memory);
	struct block *b = (struct block *)memory;
	b->free = 0;
	b->next = NULL;
	b->size = size;
	assert(b->free == 0);
	assert(b->next == NULL);
	return memory;
}

struct block *get_block_ptr(void *ptr)
{
	if (!ptr)
		return NULL;

	return (struct block *)ptr - 1;
}
