// =============================================================================
// AVLTree.cpp
// ~~~~~~~~~~~
// YOUR NAME
// implement the remove function of the AVL tree
// =============================================================================

#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
using namespace std; // BAD PRACTICE

const string usage_msg = "AVL Tree Driver. Version 0.2\n"
                         "Author: Your Name\n"                       
                         "Report bugs to yourid@buffalo.edu";
/**
 * -----------------------------------------------------------------------------
 * Maximum Method~
 *  Purpose is to find the right-most node of a binary search tree
 * -----------------------------------------------------------------------------
 **/
template <typename Key>
typename AVLTree<Key>::AVLNode* 
AVLTree<Key::maximum(typename AVLTree<Key>::AVLNode* root){
	if(root == NULL){return NULL;}
	while(root->right != NULL){
		root = root->right;
	}
	return root;
}
/**
 * -----------------------------------------------------------------------------
 * Predecessor Method~
 *  Purpose is to find the right-most node on the left branch, otherwise the
 *  first ancestor whose right child is an ancestor
 * -----------------------------------------------------------------------------
 **/
template <typename Key>
typename AVLTree<Key>::AVLNode* 
AVLTree<Key::predecessor(typename AVLTree<Key>::AVLNode* node){
	if(node == NULL){return NULL;}
	if(node->left != NULL){return maximum(node->left);}
	AVLNode* par = node->parent;
	while((par != NULL) && (par->left == node)){
		node = par;
		par = par->parent;
	}
	return par;
}
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
	AVLNode* node = search(root_, key);
	if(node == NULL){
		return false;
	}else{
		AVLNode
	}
}
