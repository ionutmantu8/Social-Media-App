#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feed.h"
#include "users.h"

// Displays a feed of posts for a user
void
feed(char *user, char *size
, post_tree_t *post_tree[MAX_FOREST], matrix_graph_t *graph, int *post_count)
{
	int size_int = atoi(size);
	int copy_post_id = *post_count;
	int user_id = get_user_id(user);
	// Loop through posts until the requested
	//number of posts is reached or no more posts are available
	while (size_int > 0 && copy_post_id > 0)
	{
		if (post_tree[copy_post_id - 1]) {
			post_t *current = post_tree[copy_post_id - 1]->root;
			// Check if the current post belongs to the user or is from a friend
			if (current && (current->user_id
			== user_id || graph->matrix[user_id][current->user_id] == 1))
			{
				if (strcmp(current->title, "\0") != 0)
				{
					printf("%s: %s\n", get_user_name(current->user_id)
					, current->title);
					size_int--;
				}
			}
			copy_post_id--;
		} else {
			break;
		}
	}
}

// Lists friends who have reposted a specific post
void friends_repost(char *user, int post_id
, post_tree_t *post_tree[MAX_FOREST], matrix_graph_t *graph)
{
	post_t *post = post_tree[post_id]->root;
	for (int i = 0; i < post->num_children; i++)
	{
		int user_id = post->children[i]->user_id;
		if (graph->matrix[user_id][get_user_id(user)] == 1)
		{
			printf("%s\n", get_user_name(user_id));
		}
	}
}

// Views the profile of a user by displaying their posts and reposts
void view_profile(char *name, post_tree_t *post_tree[MAX_FOREST])
{
	int user_id = get_user_id(name);
	if (user_id == -1)
	{
		printf("User not found.\n");
		return;
	}
	// Display posts created by the user
	for (int i = 0; i < MAX_FOREST; i++)
	{
		if (post_tree[i])
		{
			post_t *current = post_tree[i]->root;
			if (current && current->user_id == user_id)
			{
				if (strcmp(current->title, "\0") != 0)
				{
					printf("Posted: %s\n", current->title);
				}
			}
		}
	}
	// Display posts reposted by the user
	for (int i = 0; i < MAX_FOREST; i++)
	{
		if (post_tree[i])
		{
			post_t *current = post_tree[i]->root;
			if (current)
			{
				for (int j = 0; j < current->num_children; j++)
				{
					if (current->children[j]->user_id == user_id)
					{
						printf("Reposted: %s\n", current->title);
					}
				}
			}
		}
	}
}

int cmpfunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

// Recursive function to find the maximum
//clique using the Bron-Kerbosch algorithm
void bron_kerbosch(int *R, int size_R, int *P, int size_P, int *X
, int size_X, int **graph, int n, int user_id
, int *best_clique, int *best_size)
{
	if (size_P == 0 && size_X == 0)
	{
		int contains_user = 0;
		for (int i = 0; i < size_R; i++)
		{
			if (R[i] == user_id)
			{
				contains_user = 1;
				break;
			}
		}
		if (contains_user && size_R > *best_size)
		{
			*best_size = size_R;
			memcpy(best_clique, R, size_R * sizeof(int));
		}
		return;
	}
	int *P_copy = (int *)malloc(size_P * sizeof(int));
	if (!P_copy)
	{
		fprintf(stderr, "Memory allocation error for P_copy\n");
		return;
	}
	memcpy(P_copy, P, size_P * sizeof(int));
	for (int i = 0; i < size_P; i++)
	{
		int v = P_copy[i];
		int *new_R = (int *)malloc((size_R + 1) * sizeof(int));
		if (!new_R)
		{
			free(P_copy);
			return;
		}
		memcpy(new_R, R, size_R * sizeof(int));
		new_R[size_R] = v;
		int *new_P = (int *)malloc(n * sizeof(int));
		if (!new_P)
		{
			free(new_R);
			free(P_copy);
			return;
		}
		int new_size_P = 0;
		int *new_X = (int *)malloc(n * sizeof(int));
		if (!new_X)
		{
			free(new_R);
			free(new_P);
			free(P_copy);
			return;
		}
		int new_size_X = 0;
		for (int j = 0; j < size_P; j++)
		{
			if (P[j] >= 0 && graph[v][P[j]] == 1)
			{
				new_P[new_size_P++] = P[j];
			}
		}
		for (int j = 0; j < size_X; j++)
		{
			if (X[j] >= 0 && graph[v][X[j]] == 1)
			{
				new_X[new_size_X++] = X[j];
			}
		}
		bron_kerbosch(new_R, size_R + 1, new_P, new_size_P, new_X, new_size_X
		, graph, n, user_id, best_clique, best_size);
		free(new_R);
		free(new_P);
		free(new_X);
		P[i] = -1;
		X[size_X++] = v;
	}
	free(P_copy);
}

// Finds and prints the largest clique containing the specified user
void common_group(char *user, matrix_graph_t *graph)
{
	int user_id = get_user_id(user);
	if (user_id == -1)
	{
		printf("User not found.\n");
		return;
	}

	int n = graph->nodes;
	int *R = (int *)calloc(n, sizeof(int));
	int *P = (int *)calloc(n, sizeof(int));
	int *X = (int *)calloc(n, sizeof(int));

	if (!R || !P || !X)
	{
		fprintf(stderr, "Memory allocation error for initial sets\n");
		free(R);
		free(P);
		free(X);
		return;
	}

	int size_R = 0;
	int size_P = 0;
	int size_X = 0;

	for (int i = 0; i < n; i++)
	{
		P[size_P++] = i;
	}

	int *best_clique = (int *)calloc(n, sizeof(int));
	int best_size = 0;

	if (!best_clique)
	{
		fprintf(stderr, "Memory allocation error for best_clique\n");
		free(R);
		free(P);
		free(X);
		return;
	}

	bron_kerbosch(R, size_R, P, size_P, X, size_X, graph->matrix
	, n, user_id, best_clique, &best_size);

	// Sort the best_clique by user IDs for correct output order
	qsort(best_clique, best_size, sizeof(int), cmpfunc);

	printf("The closest friend group of %s is:\n", user);
	for (int i = 0; i < best_size; i++)
	{
		printf("%s\n", get_user_name(best_clique[i]));
	}

	free(R);
	free(P);
	free(X);
	free(best_clique);
}

void
handle_input_feed(char *inp, post_tree_t *post_tree[MAX_FOREST]
, matrix_graph_t *graph, int *post_count)
{
	char *commands = strdup(inp);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "feed"))
	{
		char *user = strtok(NULL, " ");
		char *size = strtok(NULL, " ");
		feed(user, size, post_tree, graph, post_count);
	} else if (!strcmp(cmd, "view-profile")) {
		char *name = strtok(NULL, "\n");
		view_profile(name, post_tree);
	} else if (!strcmp(cmd, "friends-repost")) {
		char *user = strtok(NULL, " ");
		char *post_id = strtok(NULL, " ");
		int post_id_int = atoi(post_id);
		friends_repost(user, post_id_int, post_tree, graph);
	} else if (!strcmp(cmd, "common-group")) {
		char *user = strtok(NULL, "\n");
		common_group(user, graph);
	}
	free(commands);
}
