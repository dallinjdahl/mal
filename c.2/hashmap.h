

uint64_t hash_string(char *str)
{
	uint64_t hash = 5381;
	uint8_t c;
	
	while (c = *str++) {
		hash = ((hash << 5) + hash) + c; 
	}

	return hash;
}
#define hash_cap(a) ((a) ? hash_cap_raw(a) : 0)
#define hash_entries(a) ((a) ? hash_entries_raw(a) : 0)
#define hash_gfactor(a) ((a) ? hash_gfactor_raw(a) : 0)
#define hash_sfactor(a) ((a) ? hash_sfactor_raw(a) : 0)

#define hash64_defval(a) ((a) ? hash64_defval_raw(a) : 0)
#define hash32_defval(a) ((a) ? hash32_defval_raw(a) : 0)

#define hash_hd(a) ((hash_h *)((uint8_t *)(a) - sizeof(hash_h)))
#define hash_cap_raw(a) (hash_hd(a)->cap)
#define hash_entries_raw(a) (hash_hd(a)->entries)
#define hash_gfactor_raw(a) (hash_hd(a)->gfactor)
#define hash_sfactor_raw(a) (hash_hd(a)->sfactor)

#define hash64_defval_raw(a) (hash_hd(a)->default64_v)

#define hash32_defval_raw(a) (hash_hd(a)->default32_v)


typedef struct {
	uint32_t entries;
	uint32_t cap;
	uint32_t gfactor;
	uint32_t sfactor;
	union {
	uint64_t default64_v;
	uint32_t default32_v;
	}
} hash_h;

void h64_rehash(void *old, void *new) {
	if (!old) {
		return;
	}
	uint64_t *o = old;
	uint64_t *n = new;
	uint32_t len = hash64_cap(old);
	uint32_t nlen = hash64_cap(new);
	uint32_t ni;
	uint64_t k, v;
	
	hash64_defval_raw(new) = hash64_defval_raw(old);
	for(uint32_t i = 0; i < len*2; i += 2) {
		if(o[i]) {
			k = o[i];
			v = o[i+1];
			ni = k % nlen;
			for(uint32_t j = 0; j < nlen; j++) {
				if(n[ni+j]) {
					n[ni+j] = k;
					n[ni+j+1] = v;
					break;
				}
			}
		}
	}
}

void h32_rehash(void *old, void *new) {
	if (!old) {
		return;
	}
	uint64_t *ok = old;
	uint64_t *nk = new;
	uint32_t *ov = old;
	uint32_t *nv = new;
	uint32_t len = hash64_cap(old);
	uint32_t nlen = hash64_cap(new);
	ov += 2*len;
	nv += 2*nlen;
	uint32_t ni;
	uint64_t k;
	uint32_t v;
	
	hash64_defval_raw(new) = hash64_defval_raw(old);
	for(uint32_t i = 0; i < len; i++) {
		if(ok[i]) {
			k = ok[i]; v = ov[i];
			ni = k % nlen;
			for(uint32_t j = 0; j < nlen; j++) {
				if(!nk[ni+j]) {
					nk[ni+j] = k;
					nv[ni+j] = v;
					break;
				}
			}
		}
	}
}

#define LFACTOR .85f
#define SFACTOR 2.25f
#define INIT_SIZE 8
#define h64_grow(a) (h64_resize((a), \
	2*hash_cap(*a) > INIT_SIZE ? 2*hash_cap(*a) : INIT_SIZE))

#define h64_shrink(a) (h64_resize((a), \
	hash_cap(*a)/2 > INIT_SIZE ? hash_cap(*a)/2 : INIT_SIZE))

#define h64_grow(a) (h32_resize((a), \
	2*hash_cap(*a) > INIT_SIZE ? 2*hash_cap(*a) : INIT_SIZE))

#define h64_shrink(a) (h32_resize((a), \
	hash_cap(*a)/2 > INIT_SIZE ? hash_cap(*a)/2 : INIT_SIZE))

bool h64_resize(void **h, uint32_t m) {
	hash_h *p = calloc(2 * m + 3, sizeof(uint64_t));
	p->entries = hash_entries(*h);
	p->cap = m;
	p->gfactor = LFACTOR*m;
	p->sfactor = m *(LFACTOR/SFACTOR);
	uint64_t * nh = (uint8_t *)p + sizeof(hash_h);
	h64_rehash(*h, nh);
	free(*h);
	*h = nh;
}
	
bool h32_resize(void **h, uint32_t m) {
	hash_h *p = calloc(3 * m + 6, sizeof(uint32_t));
	p->entries = hash_entries(*h);
	p->cap = m;
	p->gfactor = LFACTOR*m;
	p->sfactor = m *(LFACTOR/SFACTOR);
	uint64_t * nh = (uint8_t *)p + sizeof(hash_h);
	h32_rehash(*h, nh);
	free(*h);
	*h = nh;
}

