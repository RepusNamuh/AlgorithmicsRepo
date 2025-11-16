#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node node_t;
struct node {
    int data;
    node_t *left;
    node_t *right;
};

typedef struct {
    node_t* root;   
    int (*cmp)(void*, void*);
} tree_t;

node_t* create_node(int data) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    if (!new_node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

int compare_ints(void* a, void *b) {
    int *i1 = a;
    int *i2 = b;
    if (*i1 >= *i2) return 1;
    else return -1;
}

node_t *insert_bst(node_t *root, int data, int (*cmp)(void*, void*)) {
    if (root == NULL) {
        return create_node(data);
    }
    if (cmp(&data, &root->data) < 0) {
        root->left = insert_bst(root->left, data, cmp);
    } else {
        root->right = insert_bst(root->right, data, cmp);
    }
    return root;
}

tree_t *generate_bst(int *values, int size, int (*cmp)(void*, void*)) {
    tree_t *bst = (tree_t*)malloc(sizeof(tree_t));
    bst->root = NULL;
    bst->cmp = cmp;

    for (int i = 0; i < size; i++) {
        bst->root = insert_bst(bst->root, values[i], bst->cmp);
    }
    return bst;
}
node_t *search_bst(node_t *root, int data, int (*cmp)(void*, void*)) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (cmp(&data, &root->data) < 0) {
        return search_bst(root->left, data, cmp);
    } else {
        return search_bst(root->right, data, cmp);
    }
}

node_t* find_sucessor(node_t *node) {
    node_t *current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}
node_t* delete_node(node_t* root, int data, int (*cmp)(void*, void*)) {
    if (root == NULL) {
        return root;
    }
    if (cmp(&data, &root->data) < 0) {
        root->left = delete_node(root->left, data, cmp);
    }
    else if (cmp(&data, &root->data) > 0) {
        root->right = delete_node(root->right, data, cmp);
    }
    // Found the node with node->data == data to be deleted
    else {
        // Node with only one child
        if (root->left == NULL || root->right == NULL) {
            node_t *temp = root->left ? root->left : root->right;
            free(root);
            return temp;
        }
        else {
            node_t* successor = find_sucessor(root->right);
            root->data = successor->data;
            root->right = delete_node(root->right, successor->data, cmp);

        }
    }
}
int main() {
    srand(time(NULL));
    int values[20];
    for (int i = 0; i < 20; i++) {
        values[i] = rand() % 100 + 1;
    }
    tree_t *bst = generate_bst(values, 20, compare_ints);
    int endflag = 0;
    while (!endflag) {
        printf("Enter a number to search/delete (0 to exit)");
    }
    return 0;
}

