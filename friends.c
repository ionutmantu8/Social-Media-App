#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "friends.h"
#include "users.h"

typedef struct
{
	int **matrix; // Adjacency matrix
	int nodes;	  // Number of nodes in the graph
} matrix_graph_t;

// Function to add an edge in the adjacency matrix
void mg_add_edge(matrix_graph_t *graph, int src, int dest)
{
	graph->matrix[src][dest] = 1;
	graph->matrix[dest][src] = 1;
}

// Function to add a friend connection between two users
void add_friend(char *user1, char *user2, matrix_graph_t *graph)
{
	int id1 = get_user_id(user1);
	int id2 = get_user_id(user2);

	mg_add_edge(graph, id1, id2);
	printf("Added connection %s - %s\n", user1, user2);
}

// Function to remove a friend connection between two users
void rm_fr(int id1, int id2, char *user1, char *user2, matrix_graph_t *graph)
{
	if (graph->matrix[id1][id2] == 0)
	{
		return;
	}
	graph->matrix[id1][id2] = 0;
	graph->matrix[id2][id1] = 0;
	printf("Removed connection %s - %s\n", user1, user2);
}

// Function to calculate the distance (shortest path) between two users
void
bfs_dist(matrix_graph_t *graph, int start, int end, char *user1, char *user2)
{
	int result = -1;
	bool *visited = malloc(graph->nodes * sizeof(bool));
	if (!visited) {
		printf("Memory allocation failed\n");
		return;
	}
	int *distance = malloc(graph->nodes * sizeof(int));
	if (!distance) {
		printf("Memory allocation failed\n");
		return;
	}
	for (int i = 0; i < graph->nodes; i++) {
		visited[i] = false;
		distance[i] = INT_MAX;
	}

	visited[start] = true;
	distance[start] = 0;

	int *queue = malloc(graph->nodes * sizeof(int));
	if (!queue) {
		printf("Memory allocation failed\n");
		return;
	}
	int front = 0, rear = 0;
	queue[rear++] = start;

	while (front < rear)
	{
		int u = queue[front++];
		for (int v = 0; v < graph->nodes; v++)
		{
			if (graph->matrix[u][v] == 1 && !visited[v])
			{
				visited[v] = true;
				distance[v] = distance[u] + 1;
				queue[rear++] = v;
			}
		}
	}

	result = distance[end];
	if (result == INT_MAX)
		printf("There is no way to get from %s to %s\n", user1, user2);
	else
		printf("The distance between %s - %s is %d\n", user1, user2, result);
	free(visited);
	free(distance);
	free(queue);
}

// Function to find direct friends of a user
void find_friends(matrix_graph_t *graph, int node, bool *friends)
{
	for (int i = 0; i < graph->nodes; i++)
	{
		if (graph->matrix[node][i] == 1)
		{
			friends[i] = true;
		}
	}
}

// Function to find friends of friends
void find_friends_of_friends(matrix_graph_t *graph, bool *friends, bool *fof)
{
	for (int i = 0; i < graph->nodes; i++)
	{
		if (friends[i])
		{
			for (int j = 0; j < graph->nodes; j++)
			{
				if (graph->matrix[i][j] == 1)
				{
					fof[j] = true;
				}
			}
		}
	}
}

// Function to suggest friends for a user based on friends of friends
void suggest_friends(matrix_graph_t *graph, char *user)
{
	int node = get_user_id(user);

	bool *friends = calloc(graph->nodes, sizeof(bool));
	bool *fof = calloc(graph->nodes, sizeof(bool));

	find_friends(graph, get_user_id(user), friends);
	find_friends_of_friends(graph, friends, fof);

	printf("Suggestions for %s:\n", user);
	for (int i = 0; i < graph->nodes; i++)
	{
		if (fof[i] && !friends[i] && i != node)
		{
			printf("%s\n", get_user_name(i));
		}
	}

	free(friends);
	free(fof);
}

// Function to find common friends between two users
void common_friends(matrix_graph_t *graph, char *name1, char *name2)
{
	int id1 = get_user_id(name1);
	int id2 = get_user_id(name2);

	if (id1 == -1 || id2 == -1)
	{
		printf("One or both users not found.\n");
		return;
	}

	char *common_friends[graph->nodes];
	int num_common = 0;
	for (int i = 0; i < graph->nodes; i++)
	{
		if (graph->matrix[id1][i] && graph->matrix[id2][i])
		{
			common_friends[num_common++] = get_user_name(i);
		}
	}

	if (num_common > 0) {
		printf("The common friends between %s and %s are:\n", name1, name2);
		for (int i = 0; i < num_common; i++)
		{
			printf("%s\n", common_friends[i]);
		}
	} else {
		printf("No common friends for %s and %s\n", name1, name2);
	}
}

// Function to find the most popular friend of a user
void find_most_popular(matrix_graph_t *graph, char *name)
{
	int id = get_user_id(name);
	if (id == -1)
	{
		printf("User not found.\n");
		return;
	}

	int max_friends = 0;
	for (int j = 0; j < graph->nodes; j++)
	{
		if (graph->matrix[id][j] == 1)
		{
			max_friends++;
		}
	}

	int most_popular_id = id;
	for (int i = 0; i < graph->nodes; i++)
	{
		if (graph->matrix[id][i] == 1)
		{
			int count = 0;
			for (int j = 0; j < graph->nodes; j++)
			{
				if (graph->matrix[i][j] == 1)
				{
					count++;
				}
			}
			if (count > max_friends)
			{
				max_friends = count;
				most_popular_id = i;
			}
		}
	}

	if (max_friends == 0)
		printf("No friends found for %s.\n", name);
	else if (most_popular_id == id)
		printf("%s is the most popular\n", name);
	else
		printf("%s is the most popular friend of %s\n"
		, get_user_name(most_popular_id), name);
}

// Function to print the number of friends a user has
void print_friend_count(matrix_graph_t *graph, char *name)
{
	int id = get_user_id(name);
	if (id == -1)
	{
		printf("User not found.\n");
		return;
	}

	int friend_count = 0;
	for (int j = 0; j < graph->nodes; j++)
	{
		if (graph->matrix[id][j] == 1)
		{
			friend_count++;
		}
	}

	printf("%s has %d friends\n", name, friend_count);
}

void handle_input_friends(char *input, matrix_graph_t *graph)
{
	char *commands = strdup(input);
	char *cmd = strtok(commands, "\n ");

	if (!cmd)
		return;

	if (!strcmp(cmd, "add"))
	{
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		int id1 = get_user_id(user1);
		int id2 = get_user_id(user2);
		add_friend(user1, user2, graph);
		id1++;
		id2++;
	} else if (!strcmp(cmd, "remove")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		int id1 = get_user_id(user1);
		int id2 = get_user_id(user2);
		rm_fr(id1, id2, user1, user2, graph);
	} else if (!strcmp(cmd, "suggestions")) {
		char *user1 = strtok(NULL, "\n ");
		suggest_friends(graph, user1);
	} else if (!strcmp(cmd, "distance")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		int id1 = get_user_id(user1);
		int id2 = get_user_id(user2);
		bfs_dist(graph, id1, id2, user1, user2);
		id1++;
		id2++;
	} else if (!strcmp(cmd, "common")) {
		char *user1 = strtok(NULL, "\n ");
		char *user2 = strtok(NULL, "\n ");
		common_friends(graph, user1, user2);
	} else if (!strcmp(cmd, "friends")) {
		char *user1 = strtok(NULL, "\n ");
		print_friend_count(graph, user1);
	} else if (!strcmp(cmd, "popular")) {
		char *user1 = strtok(NULL, "\n ");
		find_most_popular(graph, user1);
	}

	free(commands);
}
