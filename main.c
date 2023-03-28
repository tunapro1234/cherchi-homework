#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


int main() {
	node_node_t *head = read_csv("file.csv");
	while (head != NULL){
		printf("%d\n", head->row_start->value);
		head = head->next;
	}
	return 0;
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

	bool first = True;
	/* dosya okuma işlemi */
    while (fgets(line, 1024, fp) != NULL) {
        tok = strtok(line, ",");
		column_head = new_node_node(column_head);
		if (first) {
			rv = column_head;
			first = False;
		}

		/* direkt olarak pointerı çevirebiliyorum çünkü int_node_t nin */ 
		/* 	nextinin bulunduğu yerde node_node_t nin row startı var */
		row_head = new_int_node((int_node_t *)column_head);

        while (tok != NULL) {
			row_head->value = atoi(tok);
			row_head = new_int_node(row_head);
            tok = strtok(NULL, ",");
        }
    }
	

    fclose(fp);
	return rv;
}

