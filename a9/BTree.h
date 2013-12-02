/**
 ******************************************************************************
 * file name : BTree.h
 * author    : Hung Q. Ngo
 * description: Binary Tree node structures and traversal routines
 ******************************************************************************
 */
#ifndef BTREE_H_
#define BTREE_H_

#include <iostream>
#include <deque>

/*
 * -----------------------------------------------------------------------------
 * A tree is simply a pointer to a BTNode
 * -----------------------------------------------------------------------------
 */
template <typename Item>
struct BTNode {
    Item payload;
    BTNode* left;
    BTNode* right;
    BTNode(const Item& item = Item(), 
                 BTNode* l = NULL, 
                 BTNode* r = NULL)
        : payload(item), left(l), right(r) {}
};

/*
 * -----------------------------------------------------------------------------
 * level-order traverse & print nodes
 * -----------------------------------------------------------------------------
 */
template <typename T>
void levelorder_print(BTNode<T>* root) {
    if (root != NULL) {
        std::deque<BTNode<T>*> node_q;
        node_q.push_front(root);
        while (!node_q.empty()) {
            BTNode<T>* cur = node_q[node_q.size()-1];
            node_q.pop_back();
            if (cur->left != NULL) node_q.push_front(cur->left);
            if (cur->right != NULL) node_q.push_front(cur->right);
            std::cout << cur->payload << " ";
        }
        std::cout << std::endl;
    }
}

/*
 * -----------------------------------------------------------------------------
 * recursive inorder traverse & print nodes
 * -----------------------------------------------------------------------------
 */
template <typename T>
void inorder_print(BTNode<T>* root) {
    if (root != NULL) {
        inorder_print(root->left);
        std::cout << root->payload << " ";
        inorder_print(root->right);
    }
}

/*
 * -----------------------------------------------------------------------------
 * recursive postorder traverse & print nodes
 * -----------------------------------------------------------------------------
 */
template <typename T>
void postorder_print(BTNode<T>* root) {
    if (root != NULL) {
        postorder_print(root->left);
        postorder_print(root->right);
        std::cout << root->payload << " ";
    }
}

/*
 * -----------------------------------------------------------------------------
 * free the memory used by all nodes in the tree started from root,
 * set root to NULL too
 * -----------------------------------------------------------------------------
 */
template <typename T>
void clear_tree(BTNode<T>*& root) {
    if (root != NULL) {
        clear_tree(root->left);
        clear_tree(root->right);
        delete root;
        root = NULL;
    }
}

/*
 * -----------------------------------------------------------------------------
 * recursive preorder traverse & print nodes
 * -----------------------------------------------------------------------------
 */
template <typename T>
void preorder_print(BTNode<T>* root) {
    if (root != NULL) {
        std::cout << root->payload << " ";
        preorder_print(root->left);
        preorder_print(root->right);
    }
}

#endif
