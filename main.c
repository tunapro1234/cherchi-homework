#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "input.csv"

typedef enum { False, True } bool;

enum options {
	OPT_NONE,
	OPT_PRINT,
	OPT_COUNT_ZEROS,
	OPT_DUPLICATES,
	OPT_COUNT,
};

typedef struct int_node_t {
	struct int_node_t *next;
	int value;
} int_node_t;


typedef struct node_node_t {
	/* next ile başlamıyor çünkü int_node_t alan */ 
	/* fonksiyonlarda normalde nexte yazdırılacak */ 
	/* şeylerin buraya yazdırılmasını istiyorum */
	int_node_t *row_start;
	struct node_node_t *next;
} node_node_t;


/*
 * TODO
 * free nodes
 *
 */

enum options parse_inputs(int argc, char** argv, int* input_index, int* output_index) {
	enum options rv = OPT_NONE;
	*input_index = -1;
	*output_index = -1;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
			*input_index = i+1;
			i++;
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
			*output_index = i+1;
            i++;
        }
        else if (strcmp(argv[i], "--print") == 0) {
			if (rv != OPT_NONE) {
				fprintf(stderr, "Only one option at a time.", argv[i]);
				exit(1);
			}
			rv = OPT_PRINT;
        }
        else if (strcmp(argv[i], "--nzeros") == 0) {
			if (rv != OPT_NONE) {
				fprintf(stderr, "Only one option at a time.", argv[i]);
				exit(1);
			}
			rv = OPT_COUNT_ZEROS;
        }
        else if (strcmp(argv[i], "--duplicates") == 0) {
			if (rv != OPT_NONE) {
				fprintf(stderr, "Only one option at a time.", argv[i]);
				exit(1);
			}
			rv = OPT_DUPLICATES;
        }
        else {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            exit(1);
        }
    }
	if (*input_index == -1) {
		fprintf(stderr, "No input file provided.\n(You can provide it with '-i <input.csv>)\n");
		exit(1);
	}
	if (*output_index == -1) {
		fprintf(stderr, "No output file provided.\n(You can provide it with '-o <output.txt>)\n");
		exit(1);
	}
	printf("%d, %d\n", *input_index, *output_index);
	return rv;
}


size_t count_number(node_node_t* self, int number_to_count) {
	node_node_t *column_head = self;
	int_node_t *row_head;
	bool first_row;

	size_t counter = 0;
	while (column_head != NULL) {
		row_head = column_head->row_start;
		while (row_head != NULL) {
			if (row_head->value == number_to_count)
				counter++;
			row_head = row_head->next;
		} column_head = column_head->next;
	} return counter;
}


void print_node(FILE* out, node_node_t* self) {
	node_node_t *column_head = self;
	int_node_t *row_head;
	bool first_row;

	while (column_head != NULL) {
		row_head = column_head->row_start;
		first_row = True;
		while (row_head != NULL) {
			if (!first_row) {
				fprintf(out, ", ");
			} first_row = False;
			fprintf(out, "%d", row_head->value);
			row_head = row_head->next;
		}
		fprintf(out, "\n");
		column_head = column_head->next;
	}
}


int_node_t* new_int_node(int_node_t* self) {
	int_node_t *new_object = (int_node_t *)malloc(sizeof(int_node_t));
	if (self != NULL) {
		self->next = new_object;
	}

	new_object->value = -1;
	new_object->next = NULL;
	return new_object;
}


node_node_t* new_node_node(node_node_t* self) {
	node_node_t *new_object = (node_node_t *)malloc(sizeof(node_node_t));
	if (self != NULL) {
		self->next = new_object;
	}
	
	new_object->row_start = NULL;
	new_object->next = NULL;
	return new_object;
}


node_node_t* read_csv(char* filename) {
    char *tok, line[1024];
	FILE* fp;
	
	node_node_t *column_head = NULL;
	node_node_t *rv = column_head;
	int_node_t  *row_head;

	/* try to read the file */ 
    fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open file '%s'\n", filename);
        exit(1);
    }

	bool first_col = True;
	bool first_row = True;

	/* dosya okuma işlemi */
    while (fgets(line, 1024, fp) != NULL) {
        tok = strtok(line, ",");
		column_head = new_node_node(column_head);
		if (first_col) {
			rv = column_head;
			first_col = False;
		}

		/* direkt olarak pointerı çevirebiliyorum çünkü int_node_t nin */ 
		/* 	nextinin bulunduğu yerde node_node_t nin row startı var */
		row_head = new_int_node((int_node_t *)column_head);
		first_row = True;

        while (tok != NULL) {
			if (!first_row) {
				row_head = new_int_node(row_head);
			} first_row = False;
			row_head->value = atoi(tok);
            tok = strtok(NULL, ",");
        }
    }
	
    fclose(fp);
	return rv;
}

int main(int argc, char** argv) {
	/* parse inputs and input data*/
	int input_index, output_index;
	enum options rv = parse_inputs(argc, argv, &input_index, &output_index);
	node_node_t *data = read_csv(argv[input_index]);
	/* done */

	/* write to output file */
    FILE *output_file = fopen(argv[output_index], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file!\n");
        return 1;
    }
	print_node(output_file, data);
    fclose(output_file);
	/* done */
	
	if (rv == OPT_PRINT)
		print_node(stdout, data);
	if (rv == OPT_COUNT_ZEROS)
		printf("number of zeros: %lu\n", count_number(data, 0));
	if (rv == OPT_DUPLICATES)
		;

	return 0;
}


