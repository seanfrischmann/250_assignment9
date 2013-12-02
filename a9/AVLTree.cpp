// =============================================================================
// AVLTree.cpp
// ~~~~~~~~~~~
// Hung Q. Ngo
// description : implement some functions from the AVLTree interface
// =============================================================================

#include <iostream>
#include <vector>
#include <sstream>
#include <stdexcept>
using namespace std; // BAD PRACTICE

template <typename Key>
typename AVLTree<Key>::AVLNode* 
AVLTree<Key>::search(typename AVLTree<Key>::AVLNode* node, Key key) 
{
    while (node != NULL && node->key != key) {
        if (key < node->key) node = node->left;
        else node = node->right;
    }
    return node;
}

template <typename Key>
bool AVLTree<Key>::insert(Key key) {
    AVLNode* p   = NULL;
    AVLNode* cur = root_;
    while (cur != NULL) {
        p = cur;
        if (key < cur->key) 
            cur = cur->left;
        else if (key > cur->key)
            cur = cur->right;
        else
            return false; // key found, no insertion, this is why we don't know
                          // whether to adjust the balance field moving down
    }

    // insert new node at a leaf position
    AVLNode* node = new AVLNode(key);
    node->parent = p;
    if (p == NULL) // empty tree to start with
        root_ = node; 
    else if (node->key < p->key)
        p->left = node;
    else
        p->right = node;

    // go up and find the first node which is not balanced, then balance it
    // also adjust the balance field of all nodes up to that point
    rebalance_after_insertion(node);
    return true;
}

template <typename Key>
void AVLTree<Key>::left_rotate(AVLNode*& node) {
    if (node == NULL || node->right == NULL) return;

    AVLNode* c = node;
    AVLNode* b = c->right;
    AVLNode* p = c->parent;

    // first, adjust all 3 parent pointers of b, c, and the left child's of b
    b->parent = p;
    c->parent = b;
    if (b->left != NULL) b->left->parent = c;

    // second, adjust other downward pointers
    if (p != NULL) {    
        if (p->right == c) p->right = b;
        else p->left = b;
    }
    c->right = b->left;
    b->left  = c;

    node = b;                  // new local root
    if (root_ == c) root_ = b; // new root if necessary
}


template <typename Key>
void AVLTree<Key>::right_rotate(AVLNode*& node) {
    if (node == NULL || node->left == NULL) return;

    AVLNode* c = node;
    AVLNode* b = c->left;
    AVLNode* p = c->parent;

    // first, adjust all 3 parent pointers of b, c, and the right child's of b
    b->parent = p;
    c->parent = b;
    if (b->right != NULL) b->right->parent = c;

    // second, adjust other downward pointers
    if (p != NULL) {
        if (p->right == c) p->right = b;
        else p->left = b;
    }
    c->left  = b->right;
    b->right = c;

    node = b;                  // new local root
    if (root_ == c) root_ = b; // new root if necessary
}

template <typename Key>
void AVLTree<Key>::rebalance_after_insertion(AVLNode* node) {
    if (node == NULL) return;
    AVLNode* p = node->parent;

    while (p != NULL) {
        // first, recompute 'balance' of the parent; node got a height increase
        if (p->left == node) 
            p->balance++;
        else 
            p->balance--;

        // if there's no grandparent or if the parent is balanced then we're done
        AVLNode* gp = p->parent; // the grand parent
        if (gp == NULL || p->balance == AVLNode::BALANCED) break;

        // if we get here then the parent p just got a height increase
        // next, see if the grand parent is unbalanced
        if (node == p->left) {
            if (p == gp->left) {
                if (gp->balance == AVLNode::LEFT_HEAVY) { 
                    // this is the LL case
                    //        gp(+2)          p (0)
                    //       /   \.          /  \.
                    //      p(+1) B  -->   node  gp (0)
                    //     / \.                 / \.
                    //   node A                A   B
                    p->balance = gp->balance = AVLNode::BALANCED;
                    right_rotate(gp);
                    break;
                }
            } else { // p == gp->right
                if (gp->balance == AVLNode::RIGHT_HEAVY) { // the RL case
                    // this is the RL case
                    //        gp(-2)               node(0)
                    //       /   \.                /   \.
                    //      A    p(+1)    -->     gp(x) p(y)
                    //           / \.             /\.  / \.
                    //         node D            A  B C   D
                    //         / \.
                    //        B   C
                    //  computing the new balance is a little trickier, depending on
                    //  which of B & C is heavier
                    switch (node->balance) {
                        case AVLNode::LEFT_HEAVY:
                            p->balance  = AVLNode::RIGHT_HEAVY;
                            gp->balance = AVLNode::BALANCED;
                            break;
                        case AVLNode::BALANCED: // only happens if B & C are NULL
                            p->balance  = AVLNode::BALANCED;   // A & D are NULL too
                            gp->balance = AVLNode::BALANCED;;
                            break;
                        case AVLNode::RIGHT_HEAVY:
                            p->balance  = AVLNode::BALANCED;;
                            gp->balance = AVLNode::LEFT_HEAVY;
                            break;
                    }
                    node->balance = AVLNode::BALANCED;
                    right_rotate(p);
                    left_rotate(gp);
                    break;
                }
            }
        } else { // node == p->right
            if (p == gp->right) {
                if (gp->balance == AVLNode::RIGHT_HEAVY) {
                    // this is the RR case
                    //        gp(-2)              p(0)
                    //       /   \.              /  \.
                    //      A   p(-1)    -->  gp(0) node
                    //           / \.         / \.  / \.
                    //          B  node      A   B
                    p->balance = gp->balance = AVLNode::BALANCED;
                    left_rotate(gp);
                    break;
                } 
            } else { // p == gp->left
                if (gp->balance == AVLNode::LEFT_HEAVY) {
                    // this is the LR case
                    //        gp(+2)          node(0)
                    //       /   \.           /   \.
                    //     p(-1)  D   -->   p(x)   gp(y)
                    //     /  \.            /\.    / \.
                    //    A    node        A  B   C   D
                    //         / \.
                    //        B   C
                    //  computing the new balance is a little trickier, depending on
                    //  with of B & C is heavier
                    switch (node->balance) {
                        case AVLNode::LEFT_HEAVY:
                            p->balance  = AVLNode::BALANCED;
                            gp->balance = AVLNode::RIGHT_HEAVY;
                            break;
                        case AVLNode::BALANCED: // only happens if B & C are NULL
                            p->balance  = AVLNode::BALANCED;
                            gp->balance = AVLNode::BALANCED;;
                            break;
                        case AVLNode::RIGHT_HEAVY:
                            p->balance  = AVLNode::LEFT_HEAVY;
                            gp->balance = AVLNode::BALANCED;;
                            break;
                    }
                    node->balance = AVLNode::BALANCED;
                    left_rotate(p);
                    right_rotate(gp);
                    break;
                } 
            }
        }
        node = p; // move up the tree
        p = gp;
    } // end while (p!= NULL)
}

template <typename Key>
vector<string> AVLTree<Key>::inorder_sequence(AVLNode* node) 
{
    vector<string> v, tmp;
    if (node != NULL) {
        v = inorder_sequence(node->left); 
        v.push_back(node->to_string());
        tmp = inorder_sequence(node->right);
        v.insert(v.end(), tmp.begin(), tmp.end());
    }
    return v;
}

template <typename Key>
vector<string> AVLTree<Key>::preorder_sequence(AVLNode* node) 
{
    vector<string> v, tmp;
    if (node != NULL) {
        v.push_back(node->to_string());
        tmp = preorder_sequence(node->left); 
        v.insert(v.end(), tmp.begin(), tmp.end());
        tmp = preorder_sequence(node->right);
        v.insert(v.end(), tmp.begin(), tmp.end());
    }
    return v;
}

template <typename Key>
void AVLTree<Key>::clear(AVLNode*& node) {
    if (node != NULL) {
        clear(node->left);
        clear(node->right);
        delete node;
        node = NULL;
    }
}
