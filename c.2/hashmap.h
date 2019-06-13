
#define LFACTOR .85f
#define SFACTOR 2.25f

uint64_t hash_string(char *str)
{
	uint64_t hash = 5381;
	uint8_t c;
	
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; 
	}

	return hash;
}

typedef struct {
	uint32_t entries;
	uint32_t cap;
	uint32_t gfactor;
	uint32_t sfactor;
} hash_h;

uint8_t constrain_h(void **h) {
	hash_h *header = (*h) - sizeof(hash_h) - 1;
	if(header->entries > header->gfactor) {
		grow(h);
		return 1;
	}
	if(header->entries < header->sfactor) {
		shrink(h);
		return -1;
	}
	return 0;
}

//gfactor = cap*LFACTOR
//sfactor = cap*(LFACTOR/SFACTOR)
