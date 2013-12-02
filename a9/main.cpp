// ============================================================================ 
// main.cpp
// ~~~~~~~~
// Hung Q. ngo
// -interface to test AVL tree functions
// ****************************************************************************
#include <iostream>
#include <map>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

#include "BTree.h"
#include "AVLTree.h"
#include "error_handling.h"
#include "term_control.h"

using namespace std;

extern const string usage_msg;
AVLTree<string> avltree;     // test this data structure

typedef void (*cmd_t)(string);

// -----------------------------------------------------------------------------
// insert a key into the avltree
// -----------------------------------------------------------------------------
void insert_key(string); // insert a new key into the AVL tree

// -----------------------------------------------------------------------------
// remove a key from avltree
// -----------------------------------------------------------------------------
void remove_key(string); // remove a key from the AVL tree

// -----------------------------------------------------------------------------
// couple of helper functions
// -----------------------------------------------------------------------------
void prompt() { cout << term_cc(BLUE) << "> " << term_cc() << flush; }

// -----------------------------------------------------------------------------
// recursively construct a tree from a preorder vector and an inorder vector
// a[a_start, ..., a_end] is the preorder vector
// b[b_start, ..., b_end] is the inorder vector
// -----------------------------------------------------------------------------
BTNode<string>* construct_tree(
        const vector<string>& a, size_t a_start, 
        const vector<string>& b, size_t b_start, size_t len);

// -----------------------------------------------------------------------------
// print the tree symmetrically, this is from assignment 7
// -----------------------------------------------------------------------------
void symmetric_print(BTNode<string>* tree);

/**
 * -----------------------------------------------------------------------------
 * main body
 * -----------------------------------------------------------------------------
 */
int main() {
    string line;
    map<string,cmd_t> cmd_map;
    cmd_map["insert"] = &insert_key;
    cmd_map["remove"] = &remove_key;

    cout << term_cc(YELLOW) << usage_msg << endl;

    while (cin) {
        prompt(); 
        getline(cin, line);
        istringstream iss(line);
        string cmd, key;
        iss >> cmd;
        if (cmd == "") continue;
        if (cmd == "exit" || cmd == "quit" || cmd == "bye") {
            exit(0);
        }
        if (cmd == "" || iss.eof()) {
            note("Syntax: insert/remove key");
            continue;
        }
        iss >> key;
        if (key == "") {
            note("Syntax: insert/remove key");
            continue;
        }

        if (cmd_map.find(cmd) != cmd_map.end()) {
            try {
                cmd_map[cmd](key);
            } catch (runtime_error &e) {
                error_return(e.what());
            }
        } else {
            error_return("Unknown command");
        }
    }
    return 0;
}

void insert_key(string key) 
{
    if (!avltree.insert(key)) {
        ostringstream oss;
        oss << "The key " << key << " already exists";
        note(oss.str());
        return;
    } else {
        vector<string> povec = avltree.preorder_sequence();
        vector<string> iovec = avltree.inorder_sequence();
        BTNode<string>* tree = construct_tree(povec, 0, iovec, 0, iovec.size());
        cout << term_cc(CYAN); 
        symmetric_print(tree);
        cout << endl << term_cc();
        clear_tree(tree);
    }
}

void remove_key(string key) 
{
    if (!avltree.remove(key)) {
        ostringstream oss;
        oss << "The key " << key << " does not exist";
        note(oss.str());
        return;
    } else {
        vector<string> povec = avltree.preorder_sequence();
        vector<string> iovec = avltree.inorder_sequence();
        BTNode<string>* tree = construct_tree(povec, 0, iovec, 0, iovec.size());
        cout << term_cc(CYAN); 
        symmetric_print(tree);
        cout << endl << term_cc();
        clear_tree(tree);
    }
}

BTNode<string>* construct_tree(
        const vector<string>& a, size_t a_start, 
        const vector<string>& b, size_t b_start, size_t len)
{
    if (len == 0) return NULL;

    BTNode<string>* node = new BTNode<string>(a[a_start]);

    size_t i;
    for (i=0; i<len; i++) {
        if (b[b_start+i].compare(a[a_start]) == 0) break;
    }
    if (i == len) 
        throw runtime_error("Invalid pair of in/pre-order sequences");

    node->left  = construct_tree(a, a_start+1, b, b_start, i);
    node->right = construct_tree(a, a_start+i+1, b, b_start+i+1, len-i-1);
    return node;
}
