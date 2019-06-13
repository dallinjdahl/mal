#include <stdio.h>
#include <stdlib.h>

char *read(char *input) {
	return input;
}

char *eval(char *input) {
	return input;
}

char *print(char *input) {
	return input;
}

char *rep(char *input) {
	return print(eval(read(input)));
}

int main(int argc, char **argv) {
	char *prompt = "user";
	FILE *in = stdin;
	FILE *out = stdout;
	char *input = NULL;
	size_t size = 0;
	
	fputs(prompt, out);
	fputs("> ", out);
	while(getline(&input, &size, in) + 1) {
		fputs(rep(input), out);
		fputs(prompt, out);
		fputs("> ", out);
		free (input);
		input = NULL;
	}
	fputc('\n', out);
}
