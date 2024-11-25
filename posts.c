#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "users.h"
#include "posts.h"
#include "struct.h"

// Function to create a new post tree with a given post as the root
post_tree_t *create_post_tree(post_t *post)
{
	post_tree_t *new_tree = (post_tree_t *)malloc(sizeof(post_tree_t));
	if (!new_tree) {
		printf("Error: Unable to allocate memory for new post tree\n");
		return NULL;
	}
	new_tree->root = post;
	new_tree->next = NULL;
	return new_tree;
}

// Function to create a new post by a user
//with a given title and add it to the post tree
void create_post(char *user, char *title, post_tree_t *post_tree, int *post_id)
{
	// Initialize the new post's fields
	post_tree->root->id = *post_id;
	post_tree->root->user_id = get_user_id(user);
	strcpy(post_tree->root->title, title);
	post_tree->root->parent = NULL;
	post_tree->root->children = NULL;
	post_tree->root->num_children = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
	{
		post_tree->root->like[i] = 0;
	}
	post_tree->root->total_likes = 0;
	(*post_id)++;
	printf("Created %s for %s\n", title, user);
}

// Function to add a child post to a parent post
void add_child_post(post_t *parent, post_t *child)
{
	parent->num_children++;
	parent->children = realloc(parent->children
	, parent->num_children * sizeof(post_t *));
	if (!parent->children)
	{
		printf("Error: Unable to allocate memory for new child post\n");
		return;
	}
	parent->children[parent->num_children - 1] = child;
	child->parent = parent;
}

// Function to create a repost of a post, optionally as child of another repost
void repost(char *user, char *post_id_str
, char *repost_id_str, post_tree_t *post_tree[MAX_FOREST], int *post_id)
{
	int original_post_id = atoi(post_id_str);
	if (!post_tree[original_post_id])
	{
		printf("Error: Post with id %d does not exist\n", original_post_id);
		return;
	}

	post_t *original_post = post_tree[original_post_id]->root;
	post_t *new_post = (post_t *)malloc(sizeof(post_t));

	if (!new_post)
	{
		printf("Error: Unable to allocate memory for new post\n");
		return;
	}
    // Initialize the new repost's fields
	new_post->id = (*post_id)++;
	new_post->user_id = get_user_id(user);
	new_post->title[0] = '\0';
	new_post->parent = NULL;
	new_post->children = NULL;
	new_post->num_children = 0;
	new_post->total_likes = 0;
	for (int i = 0; i < MAX_PEOPLE; i++)
	{
		new_post->like[i] = 0;
	}

	if (repost_id_str) {
		int repost_id = atoi(repost_id_str);
		if (!post_tree[repost_id]) {
			printf("Error: Post with id %d does not exist\n", repost_id);
			free(new_post);
			return;
		}
		add_child_post(post_tree[repost_id]->root, new_post);
	} else {
		add_child_post(original_post, new_post);
	}

	post_tree[new_post->id] = create_post_tree(new_post);
	printf("Created repost #%d for %s\n", new_post->id, user);
}

// Helper function to recursively print reposts of a post
void dfs_print_reposts(post_t *post, char *user, int repost_id)
{
	// Print the post
	if (repost_id == 0) { // if there is no second argument
		if (post->title[0] != '\0') {
			printf("%s - Post by %s\n", post->title, user);
			if (post->total_likes == -1)
				return;
		} else {
			printf("Repost #%d by %s\n", post->id
			, get_user_name(post->user_id));
		}
		for (int i = 0; i < post->num_children; i++)
		{
			dfs_print_reposts(post->children[i], user, repost_id);
		}
	} else { //if there is a second arg
		if (post->id == repost_id) {
			printf("Repost #%d by %s\n", post->id
			, get_user_name(post->user_id));
			for (int i = 0; i < post->num_children; i++)
				dfs_print_reposts(post->children[i], user, repost_id);
		} else {
			for (int i = 0; i < post->num_children; i++)
			{
				dfs_print_reposts(post->children[i], user, repost_id);
			}
		}
	}
}

// Function to get and print reposts of a post, optionally filtered by repost ID
void get_reposts(char *post_id_str, char *repost_id_str
, post_tree_t *post_tree[MAX_FOREST])
{
	int post_id = atoi(post_id_str);
	int repost_id;
	if (!repost_id_str)
		repost_id = 0;
	else
		repost_id = atoi(repost_id_str);
	post_t *post = post_tree[post_id]->root;
	char *user = get_user_name(post->user_id);
	dfs_print_reposts(post, user, repost_id);
}

// Function to find and print the lowest common ancestor of two posts
void lca(char *post, char *post_id_str1, char *post_id_str2
, post_tree_t *post_tree[MAX_FOREST])
{
	int post_id1 = atoi(post_id_str1);
	int post_id2 = atoi(post_id_str2);
	post_t *post1 = post_tree[post_id1]->root;
	post_t *post2 = post_tree[post_id2]->root;
	post_t *temp1 = post1;
	post_t *temp2 = post2;
	int count1 = 0;
	int count2 = 0;
	// Calculate the depth of both posts
	while (temp1->parent)
	{
		count1++;
		temp1 = temp1->parent;
	}
	while (temp2->parent)
	{
		count2++;
		temp2 = temp2->parent;
	}
	temp1 = post1;
	temp2 = post2;
	// Align the depth of both posts
	if (count1 > count2)
	{
		for (int i = 0; i < count1 - count2; i++)
		{
			temp1 = temp1->parent;
		}
	} else {
		for (int i = 0; i < count2 - count1; i++)
		{
			temp2 = temp2->parent;
		}
	}
	// Find the lowest common ancestor
	while (temp1 != temp2)
	{
		temp1 = temp1->parent;
		temp2 = temp2->parent;
	}
	if (post[0] == 'p')
		printf("The first common repost for %d and %d is %d\n"
		, post_id1, post_id2, temp1->id);
	else
		printf("The first common repost of %d and %d is %d\n"
		, post_id1, post_id2, temp1->id);
}

// Function to like or unlike a post or repost
void like(char *user, char *post_id_str, char *repost_id_str
, post_tree_t *post_tree[MAX_FOREST])
{
	int post_id = atoi(post_id_str);
	int repost_id;
	if (!repost_id_str)
		repost_id = 0;
	else
		repost_id = atoi(repost_id_str);
	post_t *post = post_tree[post_id]->root;
	if (repost_id == 0) // se cere pentru repost
	{
		if (post->like[get_user_id(user)] == 0)
		{
			post->like[get_user_id(user)] = 1;
			post->total_likes++;
			printf("User %s liked post %s\n"
			, user, post_tree[post_id]->root->title);
		} else {
			post->like[get_user_id(user)] = 0;
			post->total_likes--;
			printf("User %s unliked post %s\n"
			, user, post_tree[post_id]->root->title);
		}
	} else {
		post_t *repost = post_tree[repost_id]->root;
		if (repost->like[get_user_id(user)] == 0)
		{
			repost->like[get_user_id(user)] = 1;
			repost->total_likes++;
			printf("User %s liked repost %s\n"
			, user, post_tree[post_id]->root->title);
		} else {
			repost->like[get_user_id(user)] = 0;
			repost->total_likes--;
			printf("User %s unliked repost %s\n"
			, user, post_tree[post_id]->root->title);
		}
	}
}

// Function to get the number of likes of a post or repost
void get_likes(char *post_id_str, char *repost_id_str
, post_tree_t *post_tree[MAX_FOREST])
{
	int post_id = atoi(post_id_str);
	int repost_id;
	if (!repost_id_str)
		repost_id = 0;
	else
		repost_id = atoi(repost_id_str);
	post_t *post;
	if (repost_id == 0)
	{
		post = post_tree[post_id]->root;
		printf("Post %s has %d likes\n", post->title, post->total_likes);
	} else {
		post = post_tree[repost_id]->root;
		printf("Repost #%d has %d likes\n", post->id, post->total_likes);
	}
}

//Function to check if a repost has more likes than the original post
void ratio(char *post_id_str, post_tree_t *post_tree[MAX_FOREST])
{
	int post_id = atoi(post_id_str);
	post_t *post = post_tree[post_id]->root;
	int likes = post->total_likes;
	int likes_check = 0;
	int i = 0;
	int ratiod = 0;
	while (post && i < post->num_children)
	{
		likes_check = 0;
		for (int j = 0; j < post->num_children; j++)
		{
			likes_check = post->children[j]->total_likes;
			if (likes_check > likes)
			{
				printf("Post %d got ratio'd by repost %d\n"
				, post_id, post->children[i]->id);
				ratiod = 1;
				break;
			}
		}

		post = post->children[i];
		i++;
	}
	if (ratiod == 0)
	{
		printf("The original post is the highest rated\n");
	}
}

//Function to delete a post
void delete(char *post_id_str, char *repost_id_str
, post_tree_t *post_tree[MAX_FOREST])
{
	int post_id = atoi(post_id_str);
	post_t *dummy = post_tree[post_id]->root;
	int repost_id;
	if (!repost_id_str)
		repost_id = 0;
	else
		repost_id = atoi(repost_id_str);
	post_t *post = post_tree[post_id]->root;
	post->total_likes = -1;
	if (repost_id == 0)
	{
		post = post_tree[post_id]->root;
		printf("Deleted post %s\n",  post->title);
	} else {
		post = post_tree[repost_id]->root;
		printf("Deleted repost #%d of post %s\n", post->id, dummy->title);
	}
}

void handle_input_posts(char *input
, post_tree_t *post_tree[MAX_FOREST], int *post_id)
{
	char *commands = strdup(input);

	char *cmd = strtok(commands, " ");
	if (!cmd)
	{
		free(commands);
		return;
	}

	if (!strcmp(cmd, "create"))
	{
		char *user = strtok(NULL, " ");
		char *title = strtok(NULL, "\n");

		if (user && title)
		{
			post_t *new_post = (post_t *)malloc(sizeof(post_t));
			if (!new_post)
			{
				printf("Error: Unable to allocate memory for new post\n");
				free(commands);
				return;
			}
			post_tree[*post_id] = create_post_tree(new_post);
			create_post(user, title, post_tree[*post_id], post_id);
		}
	} else if (!strcmp(cmd, "repost")) {
		char *user = strtok(NULL, " ");
		char *post_id_str = strtok(NULL, " ");
		char *repost_id_str = strtok(NULL, " ");

		if (user && post_id_str)
		{
			repost(user, post_id_str, repost_id_str, post_tree, post_id);
		}
	} else if (!strcmp(cmd, "get-reposts")) {
		char *post_id_str = strtok(NULL, " ");
		char *repost_id_str = strtok(NULL, " ");
		get_reposts(post_id_str, repost_id_str, post_tree);
	} else if (!strcmp(cmd, "common-repost")) {
		char *post = strtok(NULL, " ");
		char *post_id_str1 = strtok(NULL, " ");
		char *post_id_str2 = strtok(NULL, " ");
		lca(post, post_id_str1, post_id_str2, post_tree);
	} else if (!strcmp(cmd, "like")) {
		char *user = strtok(NULL, " ");
		char *post_id_str = strtok(NULL, " ");
		char *repost_id_str = strtok(NULL, " ");
		like(user, post_id_str, repost_id_str, post_tree);
	} else if (!strcmp(cmd, "get-likes")) {
		char *post_id_str = strtok(NULL, " ");
		char *repost_id_str = strtok(NULL, " ");
		get_likes(post_id_str, repost_id_str, post_tree);
	} else if (!strcmp(cmd, "ratio")) {
		char *post_id_str = strtok(NULL, " ");
		ratio(post_id_str, post_tree);
	} else if (!strcmp(cmd, "delete")) {
		char *post_id_str = strtok(NULL, " ");
		char *repost_id_str = strtok(NULL, " ");
		delete(post_id_str, repost_id_str, post_tree);
	}

	free(commands);
}
