#include "types.h"

vector newVector() {
	unsigned int *res = malloc((sizeof unsigned int)*3 + (sizeof MalDatum) * VSIZE)
	res[0] = VSIZE; //capacity
	res[1] = 0; //length
	res[2] = 0; //error value
	return (vector)(res+3);
}

vector *vgrow(vector *v) {
	unsigned int ncap = vcap(*v)*2;
	unsigned int *nv = realloc(*vraw(v), sizeof(MalDatum)*ncap + 3*sizeof(unsigned int));
	if (nv == NULL) {
		verror(*v) = 1;
		return v;
	}
	nv[0] = ncap; //capacity
	nv[2] = 0; //error value
	*v = (MalDatum *)(nv + 3);
	return v;
}
	
vector *vpush(vector *v, MalDatum x) {
	if(vfull(*v)) {
		vgrow(v);
		if (verror(*v)) {
			return v;
		}
	}
	(*v)[vlen(*v)++] = x;
}

MalDatum vpop(vector v) {
	if(vlen(v) == 0) {
		return MalDatum{.tag = merror, .string = "buffer underrun"};
	}
	return v[--vlen(v)];
}

