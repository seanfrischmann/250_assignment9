// =============================================================================
//  AVLTree.h
//  ~~~~~~~~~
//  Hung Q. Ngo
//  A simple implementation of AVL Tree
// =============================================================================
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

template <typename Key>
class AVLTree {
public:
    AVLTree() : root_(NULL) { }
    virtual ~AVLTree() { clear(root_); }

    // -----------------------------------------------------------------------
    // insert returns true if a new node was created, false if a node with the
    // same key already exists in the tree
    // -----------------------------------------------------------------------
    bool insert(Key);

    // -----------------------------------------------------------------------
    // remove returns true if a node was removed, false if no such node is
    // found in the tree
    // -----------------------------------------------------------------------
    bool remove(Key);

    // -----------------------------------------------------------------------
    // returns whether key is found in the tree or not
    // -----------------------------------------------------------------------
    bool find(Key key) { return search(root_, key) != NULL; }

    // -----------------------------------------------------------------------
    // the minimum key and maixmum key; later on it might make sense to
    // implement an iterator for the tree
    // -----------------------------------------------------------------------
    const Key& minimum();
    const Key& maximum();
    void  clear() { clear(root_); }

private:
    // The node is similar to a BSTNode; use parent pointer to simplify codes
    // A tree is simply a pointer to a AVLNode, we will assume that variables of
    // type Key are comparable using <, <=, ==, >=, and >
    // we do not allow default keys
    struct AVLNode {
        enum { LEFT_HEAVY = 1, BALANCED = 0, RIGHT_HEAVY = -1};
        int balance; // height(left) - height(right)
        Key key;
        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;

        AVLNode(const Key& k)
        : balance(BALANCED), key(k), left(NULL), right(NULL), parent(NULL) {}

        // assumes << is implemented for the Key type
        std::string to_string() const {
            std::ostringstream oss;
            oss << key << "(" << balance << ")";
            return oss.str();
        }
    };

    AVLNode* successor(AVLNode* node);
    AVLNode* predecessor(AVLNode* node);

    // -----------------------------------------------------------------------
    // return the pointer to an AVLNode under subtree rooted at node with the
    // given key. NULL is returned if not found
    // -----------------------------------------------------------------------
    AVLNode* search(AVLNode* node, Key key);

    // -----------------------------------------------------------------------
    // node points to the root of a sub-tree which just had a height increase
    // due to insertion. the balance field of the node is correct, but the 
    // balance field of the parent node may not be correct, so is the balance
    // field of the ancestors
    // + the first time at which a new node is inserted (to a NULL place),
    //   the picture looks something like this:
    //         /
    //        p
    //       / \.
    //      B   newnode
    //  + our strategy is to 
    //   - keep moving "newnode" pointer up to the root if the subtree
    //     under newnode just got a height increase
    //   - if newnode is on the right and 
    //     - p was left heavy, p will become balanced and nothing to be done
    //     - p was balanced, p is now right heavy, and got a height increase
    //     - p was right heavy, p will become unbalanced and the fix is in
    //   - if newnode is on the left, then the symmetric situation applies
    //  + problem with the above is: to do the fix we will need a node, its
    //    parent, and its grant parent too.
    //  + hence, we will move 3 pointers at a time: node, p, and gp
    //          gp
    //         /
    //        p
    //       / \.
    //      B   newnode
    //   + initially, newnode got a height increase. We fix p's balance field
    //   (a) if p becomes balanced, then we're done!
    //   (b) if p becomes right heavy, then p just got a height increase, 
    //       in this case we check whether gp violates the AVL property
    //       (without adjusting its balance field just yet)
    //       (b.1.) If gp does not violate AVL property, then newnode = p
    //              and p = gp, move up.
    //       (b.1.) If gp does violate it: fix it with the single/double
    //       rotations
    // -----------------------------------------------------------------------
    void rebalance_after_insertion(AVLNode* node);

    // -----------------------------------------------------------------------
    // right rotate around node c. *Does not* adjust the balance field.
    //                      p              p
    //                      |              |
    //              node--> c              b <-- node (after)
    //                     / \.          /  \.
    //                    b   C  -->    A    c
    //                   / \.               / \.
    //                  A   B              B   C
    // if b is NULL then do nothing. The pointers that have to be modified are
    // - the parent pointers'
    // + c->parent = b
    // + b->parent = p
    // + B->parent = c
    // - the children's pointers'
    // + p->left/right = b (instead of c)
    // + c->left = B = b->right
    // + b->right = c (instead of B)
    // - finally, if c was the root_, then change root_ to point to b
    //   and make node point to b
    // -----------------------------------------------------------------------
    void right_rotate(AVLNode*& node);

    // -----------------------------------------------------------------------
    // left rotate around node c
    //                      p             p
    //                      |             |
    //            node-->   c             b <-- node (after)
    //                     / \.          / \.
    //                    C   b  -->    c   B
    //                       / \.      / \.
    //                      A   B     C   A
    // if b is NULL then do nothing. The pointers that have to be modified are
    // *the parent pointers*
    // + c->parent = b
    // + b->parent = p
    // + A->parent = c
    // *the children's pointers*
    // + p->left/right = b (instead of c)
    // + c->right = A = b->left
    // + b->left = c (instead of A)
    // * finally, if c was the root_, then change root_ to point to b
    //   and make node point to b
    // -----------------------------------------------------------------------
    void left_rotate(AVLNode*&);

    // clean up
    void clear(AVLNode*&);

    AVLNode* root_;

    // -----------------------------------------------------------------------
    // the following are for testing purposes only, they should be removed
    // from the "official" release, iterator should be implemented
    // -----------------------------------------------------------------------
    std::vector<std::string> preorder_sequence(AVLNode*);
    std::vector<std::string> inorder_sequence(AVLNode*);


public:
    // -----------------------------------------------------------------------
    // the following are for testing purposes only, they should be removed
    // from the "official" release, iterator should be implemented
    // -----------------------------------------------------------------------
    std::vector<std::string> preorder_sequence() { 
        return preorder_sequence(root_); 
    }
    std::vector<std::string> inorder_sequence()  { 
        return inorder_sequence(root_); 
    }
};

#include "AVLTree.cpp"   // only done for template classes
#include "AVLremove.cpp" // only done for template classes

#endif
