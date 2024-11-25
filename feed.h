#ifndef FEED_H
#define FEED_H
#include "struct.h"
/**
 * Function that handles the calling of every command from task 3
 *
 * Please add any necessary parameters to the functions
 */
void
handle_input_feed(char *input
, post_tree_t *post_tree[MAX_FOREST], matrix_graph_t *graph, int *post_id);

#endif // FEED_H
