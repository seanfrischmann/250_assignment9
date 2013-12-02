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
    // TBD
    return false;
}
