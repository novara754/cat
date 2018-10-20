#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

enum flags_t {
	b_flag = 1 << 0,
	e_flag = 1 << 1,
	n_flag = 1 << 2,
	s_flag = 1 << 3,
	t_flag = 1 << 4,
	u_flag = 1 << 5,
	v_flag = 1 << 6,
};

void handle_file(const char* file_name, enum flags_t flags);
void print_char(char c, enum flags_t flags);
void print_nonprint(unsigned char c);

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: cat <filename...>\n");
		return 1;
	}

	// Disable automatic error messages from getopt.
	opterr = 0;

	enum flags_t flags = 0;

	char opt;
	while ((opt = getopt(argc, argv, "benstuv")) != -1) {
		switch(opt) {
			case 'b': flags |= b_flag; break;
			case 'e': flags |= v_flag | e_flag; break;
			case 'n': flags |= n_flag; break;
			case 's': flags |= s_flag; break;
			case 't': flags |= v_flag | t_flag; break;
			case 'u': flags |= u_flag; break;
			case 'v': flags |= v_flag; break;
		}	
	}

	for (int i = optind; i < argc; i++) {
		handle_file(argv[i], flags);
	}

	return 0;
}

// TODO: Change handling of flags so these long parameter lists aren't needed anymore.
void handle_file(const char* file_name, enum flags_t flags) {
	FILE *file = strcmp(file_name, "-") == 0 ? stdin : fopen(file_name, "r");

	int i = 1;
	char *content = NULL;
	size_t len = 0;
	int last_blank;
	while (getline(&content, &len, file) != -1) {
		if (!(flags & s_flag) || !last_blank || *content != '\n') {
			if ((flags & b_flag && *content != '\n') || (flags & n_flag))
				printf("%6d ", i++);

			if (!(flags & v_flag)) {
				printf("%s", content);
			} else {
				for (int j = 0; content[j]; j++) {
					print_char(content[j], flags);
				}
			}
		}

		if (*content == '\n')
			last_blank = 1;
		else
			last_blank = 0;
	}
		
	free(content);
	fclose(file);
}

void print_char(char c, enum flags_t flags) {
	if ((c >= 32 && c <= 126) || c == '\n' || c == '\t') {
		if (c == '\t' && (flags & t_flag))
			printf("^I");
		else if (c == '\n' && (flags & e_flag))
			printf("$\n");
		else
			printf("%c", c);
	} else {
		print_nonprint(c);
	}
}

void print_nonprint(unsigned char c) {
	if (c < 32)
		printf("^%c", c + 64);
	else if (c == 127)
		printf("^?");
	else if (c < 160)
		printf("M-^%c", c - 128 + 64);
	else if (c < 255)
		printf("M-%c", c - 160 + 32);
	else
		printf("M-^?");
}

