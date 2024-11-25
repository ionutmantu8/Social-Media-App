/**
 * The entrypoint of the homework. Every initialization must be done here
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "users.h"
#include "struct.h"
/**
 * Initializez every task based on which task we are running
 */
void init_tasks(void)
{
#ifdef TASK_1

#endif

#ifdef TASK_2

#endif

#ifdef TASK_3

#endif
}

matrix_graph_t *init_graph(void)
{
	matrix_graph_t *mg = malloc(sizeof(matrix_graph_t));
	if (!mg)
	{
		printf("Error: Unable to allocate memory for new graph\n");
		return NULL;
	}
	mg->nodes = MAX_PEOPLE;

	mg->matrix = malloc(mg->nodes * sizeof(int *));
	if (!mg->matrix)
	{
		printf("Error: Unable to allocate memory for new graph\n");
		return NULL;
	}
	for (int i = 0; i < mg->nodes; i++)
	{
		mg->matrix[i] = calloc(mg->nodes, sizeof(int));
	}

	return mg;
}

void free_post_tree(post_t *post)
{
	if (!post)
		return;

	// Free the array of children
	free(post->children);

	// Free the post itself
	free(post);
}

int main(void)
{
	matrix_graph_t *graph = init_graph();
	post_tree_t **post_tree = calloc(MAX_FOREST, sizeof(post_tree_t *));
	int *post_id = malloc(sizeof(int));
	*post_id = 1;
	init_users();

	init_tasks();

	char *input = (char *)malloc(MAX_COMMAND_LEN);
	while (1)
	{
		char *command = fgets(input, MAX_COMMAND_LEN, stdin);

		// If fgets returns null, we reached EOF
		if (!command)
			break;

#ifdef TASK_1
		handle_input_friends(input, graph);
#endif

#ifdef TASK_2
		handle_input_posts(input, post_tree, post_id);

#endif

#ifdef TASK_3
		handle_input_feed(input, post_tree, graph, post_id);
#endif
	}
	graph->nodes = MAX_PEOPLE;
	for (int i = 0; i < graph->nodes; i++)
		free(graph->matrix[i]);
	free(graph->matrix);
	free(graph);

	for (int i = 0; i < MAX_FOREST; i++)
	{
		if (post_tree[i])
		{
			if (post_tree[i]->root)
				free_post_tree(post_tree[i]->root);
			free(post_tree[i]);
		}
	}
	free(post_tree);
	free_users();
	free(input);
	free(post_id);
	return 0;
}
