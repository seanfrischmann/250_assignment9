// =============================================================================
// AVLTree.cpp
// ~~~~~~~~~~~
// Sean Frischmann
// implement the remove function of the AVL tree
// =============================================================================

#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
#include "AVLTree.h"
using namespace std; // BAD PRACTICE

const string usage_msg = "AVL Tree Driver. Version 0.2\n"
                         "Author: Your Name\n"                       
                         "Report bugs to yourid@buffalo.edu";
/**
 * -----------------------------------------------------------------------------
 * Assignment 9: 
 * - implement this function
 * - to remove a node that has one child: splice it out
 * - if it has two children, put its predecessor in its place then splice
 *   the predecessor. This is not the same strategy as in the lecture slides
 *   which uses the successor. The reason is that you can run the applet
 *   at http://webdiis.unizar.es/asignaturas/EDA/AVLTree/avltree.html
 *   to verify whether your implementation is correct, and that applet uses
 *   the predecessor strategy.
 * - adjust the balance fields correctly, fix balances and rotate
 *   up to the root
 * - all the rotate functions have already been implemented
 * - I STRONGLY suggest that you read and understand the insert function that I
 *   implemented in the file AVLTree.cpp
 * - you can add (private) methods to AVLTree class in AVLTree.h to help out
 * - so the two files your can modify are AVLremove.cpp and AVLTree.h
 *
 * RETURN: 
 * - true if a key was removed
 * - false if the key does not exist
 * -----------------------------------------------------------------------------
 */
template <typename Key>
bool AVLTree<Key>::remove(Key key) {
	AVLNode* node_to_delete = search(root_, key);
	if(node_to_delete == NULL){
		return false;
	}
	AVLNode* node_value;
	if((node_to_delete->left == NULL) || (node_to_delete->right == NULL)){
		if(node_to_delete->left == NULL){
			node_value = node_to_delete->right;
		}else{
			node_value = node_to_delete->left;
		}
	}else{
		node_value = node_to_delete->left;
		while(node_value->right != NULL){
			node_value = node_value->right;
		}
		node_to_delete->key = node_value->key;
		node_to_delete = node_value;
		node_value = node_value->right;
	}
	AVLNode* node_par = node_to_delete->parent;
	if(node_par == NULL){
		root_ = node_value;
	}else{
		if(node_par->right == node_to_delete){
			node_par->right = node_value;
		}else{
			node_par->left = node_value;
		}
	}
	delete node_to_delete;
	if(node_value != NULL){
		node_value->parent = node_par;
	}
	if(node_par == NULL){
		return true;
	}
	rebalance_after_insertion(root_);
	return true;
}
