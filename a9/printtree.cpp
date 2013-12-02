/**
 * *****************************************************************************
 * file name : print.cpp
 * author    : Hung Q. Ngo
 * description : 
 * - implement the two tree printing routines
 * - this is a relatively bad implementation, sorry I didn't have time to clean
 *   up my codes
 * *****************************************************************************
 */
#include <iostream>
#include <iomanip>
#include <algorithm> // for max()
#include <map>
#include <vector>
#include <deque>
#include "BTree.h"
#include "term_control.h"
using namespace std;

struct Text {
    string text; // the text to be printed, e. "abc__"
    size_t pos;  // its position on the line
    Text(string t="", size_t p=0) : text(t), pos(p) {}
};

struct Coordinate {
    size_t width;  // width of a subtree box
    size_t offset; // offset to where the parent points
};


/**
 * -----------------------------------------------------------------------------
 *  given a vector of strings & their positions on a line, print them out
 *  at the correct positions; for example, if the input is
 *  [("abc", 0), ("xyz", 6), ("defg", 10)] then the routine prints
 *  abc   xyz defg
 *  ^     ^   ^
 *  0     6   10
 *  Note that
 *  vec[i].pos + vec[i].text.length() is the cursor position of the last
 *  character of the i'th string
 * -----------------------------------------------------------------------------
 */
void print_line(const vector<Text>& vec) {
    size_t cur_pos = 0; // where we are on the line
    size_t new_pos;
    for (size_t i=0; i<vec.size(); i++) {
        new_pos = vec[i].pos + vec[i].text.length();
        cout << setfill(' ') << right << setw(new_pos - cur_pos) << vec[i].text;
        cur_pos = new_pos;
    }
    cout << endl;
}

/**
 * -----------------------------------------------------------------------------
 *  the width is 
 *   max(payload, width(left-subtree)) + 1 + width(right-subtree)
 *  the 1  in the middle is to separate the left from the right by one space
 *  if the pointer is NULL, then the width is 1
 *  here's how you can visualize the routine:
 *
 *  payload___
 *  |         \
 *  ---------- ----------
 *  | left   | | right  |
 *  | width  | | width  |
 *  ---------- ----------
 *
 *  or, in case the payload is really large
 *
 *  reallylargepayload
 *  |                 \
 *  ----------         ----------
 *  | left   |         | right  |
 *  | width  |         | width  |
 *  ----------         ----------
 *
 *  that's why we take max(payload, left-width) + 1 + right-width
 * -----------------------------------------------------------------------------
 */
void compute_widths(BTNode<string>* root, 
        map<BTNode<string>*, size_t>& width_map)
{
    if (root == NULL) { width_map[NULL] = 1; return; }
    compute_widths(root->left, width_map);
    compute_widths(root->right, width_map);
    width_map[root] = max(root->payload.length(), width_map[root->left])
                      + 1 + width_map[root->right];
}

/**
 * -----------------------------------------------------------------------------
 * print a tree horizontally; the algorithm goes like this:
 * - for each node in the tree we compute the "width" of its subtree
 * - then do a level order traversal of the tree
 * - we keep 2 counter variables c & n
 *   c counts # of nodes in the current level still in the Q
 *   n counts # of nodes in the next level already pushed in the Q
 *   when c reaches 0, we know the current level is done and we move on to the
 *   next level; at the same time, we can print the current level line which we
 *   built up while extracting nodes from the current level out from the Q
 * - now, to build the "current level line" to be printed, consider an example
 *   where nodes at the current level are
 *   abc     xyz     uv  def
 * - for each node, we compute the "indentation" for that node. The indentation
 *   is computed using the width_map which we computed earlier
 * - the parent node "tells" its child what the indentation of the child is
 *   the left child has indentation exactly equal to the parent
 *   the right child has indentation equal to 
 *    the parent's indentation + max(left-width, payload) + 1 
 *
 *  payload___
 *  |         \  <--- these are called "connectives", stored in conn_vec
 *  ---------- ----------
 *  | left   | | right  |
 *  | width  | | width  |
 *  ---------- ----------
 * - once we know the indentation of each node, building up the line to be
 *   printed is not hard
 * -----------------------------------------------------------------------------
 */
void horizontal_print(BTNode<string>* root) {
    if (root == NULL) return;
    map<BTNode<string>*, size_t> width_map;
    compute_widths(root, width_map);

    deque<BTNode<string>*> node_q; node_q.push_front(root);
    deque<size_t> indent_q;        indent_q.push_front(0);
    size_t c = 1;  // # of nodes on the current level in queue
    size_t n = 0;  // # of nodes on the next level in queue
    string filler; // the _____ part
    size_t filler_size;
    vector<Text> node_vec;
    vector<Text> conn_vec;
    while (!node_q.empty()) {
        BTNode<string>* cur = node_q.back(); // head of Q
        size_t indent = indent_q.back();     // head of indent Q
        node_q.pop_back();
        indent_q.pop_back();
        --c;

        if (cur == NULL) {
            node_vec.push_back(Text("x", indent));
        } else if (cur->left == NULL && cur->right == NULL) {
            node_vec.push_back(Text(cur->payload, indent));
        } else {
            node_q.push_front(cur->left);
            indent_q.push_front(indent);
            node_q.push_front(cur->right);
            indent_q.push_front(indent + max(cur->payload.length(), 
                                width_map[cur->left]) + 1);
            n += 2;
            filler_size = max(cur->payload.length(), width_map[cur->left])
                          - cur->payload.length(); 
            filler = cur->payload + string(filler_size, '_');

            node_vec.push_back(Text(filler, indent));
            conn_vec.push_back(Text("|", indent));
            conn_vec.push_back(Text("\\", indent+filler.length()));
        }

        if (c == 0) { // reached the end of a level
            print_line(node_vec); node_vec.clear();
            print_line(conn_vec); conn_vec.clear();
            c = n;
            n = 0;
        }
    } // end while (!node_q.empty()))
}

/**
 * -----------------------------------------------------------------------------
 *  This routine computes both the width and the "midpoint" of the box enclosing
 *  each subtree rooted at 'root'
 *  Base cases:
 *  - the width is 1 if the node is NULL, offset = 1
 *  - if both the left and the right children are NULL, then
 *    width = payload.length()
 *    offset = (payload.length()+1)/2
 *
 *  Inductive cases:
 *
 *  Case 1: payload + 2 + lo + r-ro + 1 <= l+r+1
 *
 *  ----------
 *            \
 *        __payload___
 *       /            \  <--- these are called "connectives"
 *  ----------    ----------
 *  | left   |    | right  |
 *  | width  |    | width  |
 *  ----------    ----------
 *  =====1     1  ======----     
 *   lo           ro    r-ro
 *
 *  the width is l+r+1
 *  new offset is lo + (l+r+1-(r-ro+1)-lo)/2 + 1
 *              = lo + (l+ro-lo)/2 + 1
 *
 *  Case 2: payload + lo + r-ro + 1 >= l+r
 *
 *  ----------
 *            \
 *        payloadlong
 *       /           \  <--- these are called "connectives"
 *  ----------   ----------
 *  | left   |   | right  |
 *  | width  |   | width  |
 *  ----------   ----------
 *  =====1    x  ======----     
 *   lo          ro    r-ro
 *
 *  here, we want l+r+x = payload + 2 + lo + r - ro + 1
 *  and hence x = payload+lo-ro+3-l
 *
 *  new offset is lo + (l+r+x-(r-ro+1)-lo)/2 + 1
 *              = lo + (l+ro-lo+x-1)/2 + 1
 *
 *  In both cases,
 *  x = max(payload+lo+r-ro+2,l+r) -l-r+1
 *  the width is l + x + r.
 *  and new offset is lo + (l+ro-lo+x-1)/2 + 1
 * -----------------------------------------------------------------------------
 */
void compute_coordinates(BTNode<string>* root, 
        map<BTNode<string>*, Coordinate>& coordinate_map)
{
    Coordinate ret, lc, rc;
    size_t x;
    if (root == NULL) { 
        ret.width  = 1;
        ret.offset = 1;
    } else if (root->left == NULL && root->right == NULL) {
        ret.width = root->payload.length();
        ret.offset = 1 + ret.width/2;
    } else {
        compute_coordinates(root->left, coordinate_map);
        compute_coordinates(root->right, coordinate_map);
        lc = coordinate_map[root->left];
        rc = coordinate_map[root->right];
        x = max(root->payload.length()+lc.offset+rc.width-
                rc.offset+2,lc.width+rc.width) - lc.width-rc.width+1;
        ret.width = lc.width+rc.width+x;
        ret.offset = lc.offset + 1 + (lc.width+rc.offset+x-1-lc.offset)/2;
    }
    coordinate_map[root] = ret;
}

/**
 * -----------------------------------------------------------------------------
 * print a tree symmetrically; the algorithm goes like this:
 * - for each node in the tree we compute the "width" of its subtree
 * - then do a level order traversal of the tree
 * - we keep 2 counter variables c & n
 *   c counts # of nodes in the current level still in the Q
 *   n counts # of nodes in the next level already pushed in the Q
 *   when c reaches 0, we know the current level is done and we move on to the
 *   next level; at the same time, we can print the current level line which we
 *   built up while extracting nodes from the current level out from the Q
 * - now, to build the "current level line" to be printed, consider an example
 *   where nodes at the current level are
 *   abc     xyz     uv  def
 * - for each node, we compute the "indentation" for that node's subtree. 
 *   The indentation is computed using the width_map which we computed earlier
 * - the parent node "tells" its children what the indentation of the children
 *   are. Here the indentation is the indentation of the smallest 'box' that 
 *   encloses the subtree rooted at the child.
 *   the left child has indentation exactly equal to the parent's
 *   the right child has indentation equal to a quantity that depends on how
 *   large the left box is and how large the parent's payload is
 * -----------------------------------------------------------------------------
 */
void symmetric_print(BTNode<string>* root) {
    if (root == NULL) return;
    map<BTNode<string>*, Coordinate> coordinate_map;
    compute_coordinates(root, coordinate_map);

    deque<BTNode<string>*> node_q; node_q.push_front(root);
    deque<size_t> indent_q;        indent_q.push_front(0);
    size_t c = 1;  // # of nodes on the current level in queue
    size_t n = 0;  // # of nodes on the next level in queue
    string filler; // the _____ part
    Coordinate lc, rc;
    size_t filler_size, x;
    vector<Text> node_vec;
    vector<Text> conn_vec;
    while (!node_q.empty()) {
        BTNode<string>* cur = node_q.back(); // head of Q
        size_t indent = indent_q.back();     // head of indent Q
        node_q.pop_back();
        indent_q.pop_back();
        --c;

        if (cur == NULL) {
            node_vec.push_back(Text("x", indent));
        } else if (cur->left == NULL && cur->right == NULL) {
            node_vec.push_back(Text(cur->payload, indent));
        } else {
            lc = coordinate_map[cur->left];
            rc = coordinate_map[cur->right];
            x = max(cur->payload.length()+lc.offset+rc.width-
                    rc.offset+2,lc.width+rc.width) - lc.width-rc.width+1;

            node_q.push_front(cur->left); 
            indent_q.push_front(indent);
            node_q.push_front(cur->right);
            indent_q.push_front(indent + lc.width + x);

            n += 2;
            filler_size = x+lc.width+rc.width-2-lc.offset-(rc.width-rc.offset+1)
                          -cur->payload.length();
            filler = string(filler_size/2, '_') + cur->payload + 
                     string(filler_size - filler_size/2, '_');

            node_vec.push_back(Text(filler, indent+lc.offset+1));
            conn_vec.push_back(Text("/", indent+lc.offset));
            conn_vec.push_back(Text("\\", indent+lc.width+x+rc.offset-2));
        }

        if (c == 0) { // reached the end of a level
            print_line(node_vec); node_vec.clear();
            print_line(conn_vec); conn_vec.clear();
            c = n;
            n = 0;
        }
    } // end while (!node_q.empty()))
}

/**
 * -----------------------------------------------------------------------------
 * print vertical bars '|' at the positions whose offsets are specified by 
 * the depth vector wvec
 * for example, if wvec = [1, 3, 5, 3] then we print
 * |  |    |  |
 * 0  3    8 11 
 * where 0 is the 1st position
 * 2 = 0+2 is the next position
 * 6 = 2+4 is the next position
 * 8 = 6+2 is the next position
 * -----------------------------------------------------------------------------
 */
void print_bars(const vector<size_t>& wvec, bool underscore=true) {
    for (size_t i=0; i<wvec.size(); i++)
        cout << right << setw(wvec[i]) << "|";
    if (underscore) cout << "__";
}

/**
 * -----------------------------------------------------------------------------
 * recursive procedure for printing the tree vertically
 * the algorithm works as follows. Consider the following tree as an example
 * root
 * |__34
 * |  |__c
 * |  |                 <--- transitional line
 * |  |__longer
 * |     |__bbbbbbb
 * |     |              <--- transitional line
 * |     |__aaaaaaaaa
 * |                    <--- transitional line
 * |__thisisverylong
 *    |__short
 *    |
 *    |__x
 * we are given the root, say with payload "34" above. We assume that the 
 * connecting lines |__ up to this root has already been printed.
 * further more, we are given a vector of offsets of all the vertical bars prior
 * to 34. In this case, 'wvec' has only one element: wvec = [1]
 * - To print the next line, we print all the vertical bars indicated by wvec
 * plus one more bar | and the two underscores __
 *   we call recursively stunning_vertical so it will take care of itself
 * - then, we print the t transitional line between two children
 * - finally we recursively print the right child; the only tricky part is that
 * on the right child we have to shift the right most vertical bar two positions
 * to the right
 * -----------------------------------------------------------------------------
 */
void stunning_vertical(BTNode<string>* node, vector<size_t>& wvec) 
{
    // print the node's payload
    cout << term_cc(CYAN);
    if (node == NULL) { 
        cout << "x" << endl << term_cc(); 
        return; 
    } else {
        cout << node->payload << endl << term_cc();
    }

    if (node->right == NULL && node->left == NULL) return;

    // print | ... |__ on the next line
    print_bars(wvec);

    // one extra | 3 positions ahead for the right child, let it do its work
    wvec.push_back(3);
    stunning_vertical(node->right, wvec);

    // transitional line between two children
    wvec.pop_back(); 
    print_bars(wvec, false); cout << endl;

    // finally do the left child
    print_bars(wvec);
    wvec[wvec.size()-1] += 3;
    stunning_vertical(node->left, wvec);
}


/**
 * -----------------------------------------------------------------------------
 * print a tree vertically
 * -----------------------------------------------------------------------------
 */
void vertical_print(BTNode<string>* root) 
{
    vector<size_t> wvec; 
    wvec.push_back(1);
    stunning_vertical(root, wvec);
}
