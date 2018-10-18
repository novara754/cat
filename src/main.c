#include <stdio.h>
#include <stdlib.h>

int read_file(const char *file_name, char **buffer);

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: cat <filename...>\n");
		return 1;
	}

	for (int i = 1; i < argc; i++) {
		char *file_name = argv[i];
		char *content;
		if (*file_name == '-') {
			size_t len = 50;
			while (getline(&content, &len, stdin) != -1)
				printf("%s", content);
			// Clear EOF flag (and other error flags) so stdin can be read again.
			clearerr(stdin);
		} else {
			read_file(file_name, &content);
			printf("%s", content);
		}
	}

	return 0;
}

int read_file(const char *file_name, char **buffer) {
	FILE *file = fopen(file_name, "r");
	if (!file) {
		fprintf(stderr, "Could not open file %s.", file_name);
		return 0;
	}

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);
	*buffer = (char *) malloc(sizeof(char) * size);
	if (!buffer) {
		fprintf(stderr, "Could not allocate memory for file %s.", file_name);
		return 0;
	}
	int read = fread(*buffer, 1, size, file);
	fclose(file);

	return read;
}

