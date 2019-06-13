
enum MalType {mstring, mnil, mtrue, mfalse, mkeyword, mvector, mhashmap, mlist, mnum, msymbol, merror, mfnum};
typedef struct MalDatum {
	MalType tag;
	union {
		char *string;
		long int num;
		float fnum;
		vector list;
	}
}

typedef MalDatum *vector; 
	
#define VSIZE 2
vector newVector();
#define vraw(v)		((uint16_t *) (v) - 3)
#define vfree(v)	free(raw(v))
#define vcap(v)		raw(v)[0]
#define vlen(v)		raw(v)[1]
#define vlast(v)	v[len(v)-1]
#define verror(v)	raw(v)[2]
#define vfull(v)	(cap(v) == len(v))

vector *vgrow(vector *v);	
vector *vpush(vector *v, MalDatum x);
MalDatum vpop(vector v);
