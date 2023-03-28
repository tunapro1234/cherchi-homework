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
	int_node_t *row_start;
	struct node_node_t *next;
} node_node_t;


node_node_t* read_csv(char* filename);
node_node_t* new_node_node(node_node_t* self);
int_node_t* new_int_node(int_node_t* self);
void print_node(node_node_t* self);

int main() {
    /* char* input_file = "input.csv"; */
    /* char* output_file = "output.txt"; */
    /* char* option = "--print"; */
    /* for (int i = 1; i < argc; i++) { */
    /*     if (argv[i][0] == '-') { */
    /*         option = argv[i]; */
    /*         if (i + 1 < argc && argv[i+1][0] != '-') { */
    /*             if (argv[i][1] == 'i') input_file = argv[i+1]; */
    /*             else if (argv[i][1] == 'o') output_file = argv[i+1]; */
    /*         } */
    /*     } */
    /* } */
	
	node_node_t *data = read_csv(FILENAME);
	print_node(data);
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

