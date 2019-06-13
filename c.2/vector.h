#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/* public api macros */
#define vec_cap(a) ((a) ? vec_cap_raw(a) : 0)
#define vec_len(a) ((a) ? vec_len_raw(a) : 0)
#define vec_push(a, el) (vec_full(a) ? vec_grow(a, sizeof(*a), 1) : 0, \
	a[vec_len_raw(a)++] = el
#define vec_add(a, n) \  //does not initialize the empty slots
	(n < vec_free(a) ? vec_grow(a, sizeof(*a), n) : 0, \
	vec_len_raw(a) += n)
#define vec_ensure(a, n) ((n) > vec_cap(a) ? \
	vec_add(a, (vec_cap(a) - (n))) :\
	(vec_len_raw(a) = n))
#define vec_free(a) realloc(vec_h(a), 0)

/* implementation details (subject to change) */
#define vec_cap_raw(a) (vec_hd(a)->cap)
#define vec_len_raw(a) (vec_hd(a)->len) 
#define vec_free(a) ((a) ? vec_cap_raw(a) - vec_len_raw(a) : 0)
#define vec_hd(a)  ((vec_h*)((uint8_t *)a - sizeof(vec_h)))
#define vec_full(a) ((a) ? vec_cap_raw(a) == vec_len_raw(a) : true)

typedef struct {
	uint16_t cap;
	uint16_t len;
} vec_h;

//returns true if there is an error
bool vec_grow(void **v, size_t inc, uint16_t augment) {
	uint16_t dcap = vec_cap(*v) * 2;
	uint16_t min = vec_len(*v) + augment;
	uint16_t ncap = dcap > min ? dcap : min;
	void *p;
	
	p = realloc(*v ? vec_hd(*v) : NULL, sizeof(vec_h) + (inc * ncap));
	if (!p) {
		return true;
	}
	p += sizeof(vec_h);
	vec_cap_raw(p) = ncap;
	
	*v = p;
	return false;
}
