#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "input.csv"

typedef enum { False, True } bool;

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

typedef struct options_t {
	bool print_matrix, count_zeros, find_duplicates;	
	char* input_file;
	char* output_file;
} options_t;

node_node_t* new_node_node(node_node_t* self);
int_node_t* new_int_node(int_node_t* self);
node_node_t* read_csv(char* filename);
void print_node(node_node_t* self);

/*
 * TODO
 * free nodes
 *
 */

options_t* options_new() {
	options_t *self = (options_t *)malloc(sizeof(options_t));

	self->print_matrix = 0;
	self->count_zeros = 0;
	self->find_duplicates = 0;	
	self->input_file = NULL;
	self->output_file = NULL;

	return self;
}

options_t* parse_inputs(int argc, char** argv) {
	options_t *options = options_new();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            options->input_file = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            options->output_file = argv[i+1];
            i++;
        }
        else if (strcmp(argv[i], "--print") == 0) {
            options->print_matrix = 1;
        }
        else if (strcmp(argv[i], "--nzeros") == 0) {
            options->count_zeros = 1;
        }
        else if (strcmp(argv[i], "--duplicates") == 0) {
            options->find_duplicates = 1;
        }
        else {
            fprintf(stderr, "Error: unknown option '%s'\n", argv[i]);
            exit(1);
        }
    }
	return options;
}

int main(int argc, char** argv) {
	options_t* options = parse_inputs(argc, argv);
	printf("print_matrix: %d\n", options->print_matrix);
	printf("count_zeros: %d\n", options->count_zeros);
	printf("find_duplicates: %d\n", options->find_duplicates);
	
	/* node_node_t *data = read_csv(FILENAME); */
	/* print_node(data); */
	return 0;
}


void print_node(node_node_t* self) {
	node_node_t *column_head = self;
	int_node_t *row_head;
	bool first_row;

	while (column_head != NULL) {

		row_head = column_head->row_start;
		first_row = True;
		while (row_head != NULL) {
			if (!first_row) {
				printf(", ");
			} first_row = False;
			printf("%d", row_head->value);
			row_head = row_head->next;
		}
		printf("\n");
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

