#include <utf.h>
#include <fmt.h>
#include <regexp9.h>

#include "reader.h"

//typedef enum tokenType {
//	oparen, cparen, obracket, cbracket, obrace, cbrace, quote,
//	backquote, tilde, caret, at, tildeat, string, comment, ident
//} tokenType;

typedef struct token {
	char * data;
	int len;
} token;

typedef struct Reader {
	char *in;
	Resub *match;
	int8_t error;
} Reader;

void ReaderInit(Reader *r, char *in) {
	char *regexstr = "[\t ,]*(~@|[\\[\\]{}()'`~@]|\"(?:[\\\\].|[^\\\\\"])*\"?|;.*|[^\t \\[\\]{}()'\"`~@,;]*)";
	regex = regcomp(regexstr);
	r->in = in;
	if (r->match == NULL) {
		r->match = malloc(sizeof Resub);
	}
	r->error = regexec(regex, r->in, r->match, 2) ? 0 : -1;
}

token ReaderNext(Reader *r) {
	token t = ReaderPeek(r);
	r->error = regexec(regex, r->in, r->match, 2) ? 0 : -1;
	return t;
}

token ReaderPeek(Reader *r) {
	return token{r->match.sp, r->match.ep - r->match.sp};
}

void ReaderDestroy(Reader *r) {
	free(regex);
	regex = NULL;
	free(r->match);
	r->match = NULL;
}

MalDatum read_form(Reader *r) {
	token t = ReaderPeek(r);
	switch (*(t.data)) {
	case '(':
		return read_list(r);
		break;
	default:
		return read_atom(r);
	}
}

MalDatum read_list(Reader *r) {
	MalDatum m;
	vector v = newVector();
	ReaderNext(r);
	while(!(r->error) && ReaderPeek(r).data[0] != ')') { 
		m = read_form(r);
		vpush(&v, m);
	}
	if(r->error) {
		return MalDatum{.tag = merror, .string = "Expected ')'"};
	}
	return MalDatum{.tag = mlist, .list = v};
}

MalDatum read_atom(Reader *r) {
	long int ival;
	float fval;
	char *sval;
	char *endparse;

	token tok = ReaderPeek(r);
	ival = strtol(tok.data, &endparse, 0);
	if (endparse == tok.data + tok.len) {
		return MalDatum{.tag = mnum, .num = ival};
	}
	fval = strtof(tok.data, &endparse);
	if (endparse == tok.data + tok.len) {
		return MalDatum{.tag = mfnum, .fnum = fval};
	}

	return MalDatum{.tag = msymbol, .string = extractString(tok.data, tok.len) };
}


char *extractString(char *s, int l) {
	char *res = malloc( (sizeof char) * l + 1);
	for(int i = 0; i < l; i++) {
		res[i] = s[i];
	}
	res[l] = '\0';
	return res;
}
