#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

void print_nonprint(unsigned char c);

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: cat <filename...>\n");
		return 1;
	}

	// Disable automatic error messages from getopt.
	opterr = 0;

	int b_flag = 0,
		e_flag = 0,
		n_flag = 0,
		s_flag = 0,
		t_flag = 0,
		u_flag = 0,
		v_flag = 0;

	char opt;
	while ((opt = getopt(argc, argv, "benstuv")) != -1) {
		switch(opt) {
			case 'b': b_flag = 1; break;
			case 'e': v_flag = e_flag = 1; break;
			case 'n': n_flag = 1; break;
			case 's': s_flag = 1; break;
			case 't': v_flag = t_flag = 1; break;
			case 'u': u_flag = 1; break;
			case 'v': v_flag = 1; break;
		}	
	}

	for (int i = optind; i < argc; i++) {
		char *file_name = argv[i];
		FILE *file = strcmp(file_name, "-") == 0 ? stdin : fopen(file_name, "r");

		int i = 1;
		char *content = NULL;
		size_t len = 0;
		int last_blank;
		while (getline(&content, &len, file) != -1) {
			if (!s_flag || !last_blank || *content != '\n') {
				if ((b_flag && *content != '\n') || n_flag)
					printf("%6d ", i++);

				if (!v_flag) {
					printf("%s", content);
				} else {
					for (int j = 0; content[j]; j++) {
						char c = content[j];
						if ((c >= 32 && c <= 126) || c == '\n' || c == '\t') {
							if (c == '\t' && t_flag)
								printf("^I");
							else if (c == '\n' && e_flag)
								printf("$\n");
							else
								printf("%c", c);
						} else {
							print_nonprint(c);
						}
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

	return 0;
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

