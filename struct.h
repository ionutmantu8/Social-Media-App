#ifndef STRUCT_H
#define STRUCT_H
#define MAX_COMMAND_LEN 500
#define MAX_PEOPLE 550
#define MAX_FOREST 500
#define MAX_TITLE 281

typedef struct post_t
{
	int id;
	int user_id;
	char title[MAX_TITLE];
	struct post_t *parent;
	struct post_t **children;
	int num_children;
	int like[MAX_PEOPLE];
	int total_likes;
} post_t;

typedef struct post_tree_t
{
	post_t *root;
	struct post_tree_t *next;
} post_tree_t;

typedef struct
{
	int **matrix; /* Matricea de adiacenta a grafului */
	int nodes;	  /* Numarul de noduri din graf. */
} matrix_graph_t;

/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
 */
void
handle_input_feed(char *input
, post_tree_t *post_tree[MAX_FOREST], matrix_graph_t *graph, int *post_id);
void handle_input_friends(char *input, matrix_graph_t *graph);
void handle_input_posts(char *input
, post_tree_t *post_tree[MAX_FOREST], int *post_id);
#endif
