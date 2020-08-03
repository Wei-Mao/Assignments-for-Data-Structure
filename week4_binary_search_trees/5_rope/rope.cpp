#include <cstdio>
#include <string>
#include <iostream>
#include <stack>
using namespace std;

// Vertex of a splay tree
struct Vertex {
    char key;
    // number of the nodes in left and right subtrees(including itself)
    // remember to update
    // it after each operation that changes the tree.
    long long size;   // for order statistics.
    Vertex* left;
    Vertex* right;
    Vertex* parent;

    Vertex(int key, long long size, Vertex* left, Vertex* right, Vertex* parent)
            : key(key), size(size), left(left), right(right), parent(parent) {}
};

class Rope {
    std::string s;
    Vertex * root;
public:
    Rope(const std::string &s) : s(s) {
        root = NULL;
        for(int i(0); i < s.length(); i++)
        {
            Vertex * char_node = new Vertex(s[i], 1, NULL, NULL, NULL);
            root = merge(root, char_node);
        }
    }

    void process_naive( int i, int j, int k ) {
        // Replace this code with a faster implementation
        std::string t = s.substr(0, i) + s.substr(j + 1);
        s = t.substr(0, k) + s.substr(i, j - i + 1) + t.substr(k);
    }

    void update(Vertex* v)
    {
        if (v == NULL) return;

        // NULL means does not exist.
        // update the satellite data.
        v->size = 1 + (v->left != NULL ? v->left->size : 0) +
                (v->right != NULL ? v->right->size : 0);  // update  the size.

        // v's left and right children has been updated. But v's left and right children have not been updated in time.
        // Note that tree is undirected graph. Parent should update child info. Meanwhile, parent' children should update their parent info.
        if (v->left != NULL) {
            v->left->parent = v;
        }
        if (v->right != NULL) {
            v->right->parent = v;
        }
    }

    void small_rotation(Vertex* v)
    {
        // Only rotation but not find out which side is heavy. Apply this small_rotation to the right node.
        // elevate v to take the place of its parent.
        // v, v's children, v's parent, v's grandparent are affected.
        Vertex* parent = v->parent;
        if (parent == NULL) {
            // already root node, no need to rotate.
            return;
        }
        Vertex* grandparent = v->parent->parent;
        if (parent->left == v) { // parent > v, parent can be changed to be right child of parent
            Vertex* m = v->right;  // store right child of v
            v->right = parent;     // raise v one level up, parent's left child is empty
            parent->left = m;      // right child of v becomes the left child of old parent
        } else {                 // parent->right == v, parent < v, parent can act as the left child of v
            Vertex* m = v->left;   // spare space for parent
            v->left = parent;
            parent->right = m;     // v->left take the place of the v as the right child.
        }

        update(parent);          // update v and parent's satellite and children's parent info
        update(v);

        v->parent = grandparent;   // update the parent of v.
        // raised v replace the role of the v's parent.
        // before updating, you should determine whether parent is left or right child.
        if (grandparent != NULL) { // parent of v is not root.
            if (grandparent->left == parent) {
                grandparent->left = v;
            } else {
                grandparent->right = v;
            }
        }
        // Since grandparent'sum is the sum of the all elements in its' subtree and subtree nodes of grandparent remains the same(arrangement changes)
        // No need to changes the sum of grandparent.
    }

    // small rotation may be stuck in a loop.This is where the splay step comes into play.
    void big_rotation(Vertex* v) {
        if (v->parent->left == v && v->parent->parent->left == v->parent) {
            // Zig-zig left-handed variant.
            small_rotation(v->parent);
            small_rotation(v);
        } else if (v->parent->right == v && v->parent->parent->right == v->parent) {
            // Zig-zig right-handed variant.
            small_rotation(v->parent);
            small_rotation(v);
        } else {
            // Zig-zag  or Zig  step.
            small_rotation(v);
            small_rotation(v); //  If v becomes root after first small_rotation, this rotate does not work again.
        }
    }

    // Makes splay of the given vertex and makes
    // it the new root.
    void splay(Vertex *&R, Vertex *v) {
      // you have to use Vertex *& R to ensure that R always points to the root.
        if (v == NULL) return;
        // Splay until the v becomes the root.
        while (v->parent != NULL) {
            if (v->parent->parent == NULL) {
                // Zig step
                small_rotation(v);
                break;
            }
            // Zig-Zig or Zig-Zag.
            big_rotation(v);
        }
        R = v;   //  if you don't use reference to root pointer, this will make R not point to root any longer but point to another vertex v.
	// What the pointer points to can be synchronize! But the pointer itself can not be synchronize.
	// Beware of assign one pointer to another.
    }

    // Searches for the given key in the tree with the given root
    // and calls splay for the deepest visited node after that.
    // If found, returns a pointer to the node with the given key.
    // Otherwise, returns a pointer to the node with the smallest
    // bigger key (next value in the order).
    // If the key is bigger than all keys in the tree,
    // returns NULL.
    Vertex* find(Vertex *&R, int k) {
        // Similar to find the kth smallest key in a binary tree of integers.
        Vertex* v = R;

        while (v != NULL) {
            long long s = (v->left != NULL) ? v->left->size : 0;
            if (k == (s+1)) {  // found
                break;
            }
            else if (k < (s + 1))
            {
                v = v->left;
            }
            else
            {
                // k > s + 1, first (s+1) chars are in the left subtree of v
                v = v->right;
                k = k - s - 1;
            }
        }
        // v is not NULL->found; v is NULL -> not found.

        // after find makes splay to move the frequently-accessed elements up towards the top of the tree.
        splay(R, v);   // splay  the deepest visited node. splay need reference to pointer.
        return v;
    }

    void split(Vertex* &R, int key, Vertex* &left, Vertex* &right)
    {
        right = find(R, key);  // return (key)th char_node.
        splay(R, right);

        if (right == NULL) {
            // No element with key >= key in the given tree with root. Hence, can not split.
            left = R;
            return;
        }

        left = right->left;
        right->left = NULL;  // delete the left node.

        if (left != NULL) {
            left->parent = NULL;
        }

        // only right and left are affected.
        update(left);   // (key  - 1)th and ... go the left.
        update(right);  // keyth and... go to the right.
    }

    Vertex* merge(Vertex* left, Vertex* right) {
        // all elements in left subtree are smaller than those in the right subtree
        // If one of tree is empty, then return another. Assume only one of them is empty.
        if (left == NULL) return right;  // Assignment to pointer. Hence, you should use reference to pointer.
        if (right == NULL) return left;

        // Find the smallest node in the right subtree.
        // Alternatively, find largest node in the left subtree.
        Vertex* min_right = right;
        while (min_right->left != NULL) {
            min_right = min_right->left;
        }
        splay(right, min_right); // elevate min_right to

        right->left = left;
        update(right);
        return right;
    }

    void insert(Vertex *&R, int k, Vertex *& sub_string)
    {
        // Insert sub_string before the kth char of the R, i.e. after(k - 1)
        Vertex * left = NULL;
        Vertex * right = NULL;
        split(R, k, left, right); // First to (k-1)th chars go to left while kth to last go to the right.
        R = merge(merge(left, sub_string), right);
    }

    string in_order_traverse()
    {
        string print_str;
        stack<Vertex *> stack_visited;
        Vertex * current_node = root;
        while((current_node != NULL) || (!stack_visited.empty()))
        {
            if(current_node != NULL)
            {
                stack_visited.push(current_node);
                current_node = current_node->left;
            }
            else
            {
                // current_node ==  NULL
                Vertex * pushed = stack_visited.top();
                stack_visited.pop();
                print_str.push_back(pushed->key);
                current_node = pushed->right;
            }
        }
        return print_str;
    }

    void process_fast(int i, int j, int k)
    {
        // i and j are zero-based.
        // k is 1-based.
        Vertex * left = NULL;
        Vertex * middle = NULL;
        Vertex * right = NULL;
        split(root, i + 1, left, middle); // S[i,...] goes to the middle
        split(middle, j - i + 2, middle, right); // S[i, j] goes to the middle.
        root = merge(left, right);
        insert(root, k + 1, middle);
    }

    std::string result() {
        s = in_order_traverse();
        return s;
    }
};

int main() {
    std::ios_base::sync_with_stdio(0);
    std::string s;
    std::cin >> s;
    Rope rope(s);
    int actions;
    std::cin >> actions;
    for (int action_index = 0; action_index < actions; ++action_index) {
        int i, j, k;
        std::cin >> i >> j >> k;
        rope.process_fast(i, j, k);
    }
    std::cout << rope.result() << std::endl;
}
