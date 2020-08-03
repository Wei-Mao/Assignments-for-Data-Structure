#include <cstdio>

// Splay tree implementation

// Vertex of a splay tree
struct Vertex {
  int key;
  // Sum of all the keys in the subtree - remember to update
  // it after each operation that changes the tree.
  long long sum;
  Vertex* left;
  Vertex* right;
  Vertex* parent;

  Vertex(int key, long long sum, Vertex* left, Vertex* right, Vertex* parent) 
  : key(key), sum(sum), left(left), right(right), parent(parent) {}
};

void update(Vertex* v) {
  if (v == NULL) return;  

  // NULL means does not exist. 
  // update the satellite data. 
  v->sum = v->key + (v->left != NULL ? v->left->sum : 0ll) + (v->right != NULL ? v->right->sum : 0ll);  // update sum

  // v's left and right children has been updated. But v's left and right children have not been updated in time.
  // Note that tree is undirected graph. Parent should update child info. Meanwhile, parent' children should update their parent info.
  if (v->left != NULL) {
    v->left->parent = v; 
  }
  if (v->right != NULL) {
    v->right->parent = v;
  }
}

void small_rotation(Vertex* v) {
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
void splay(Vertex*& root, Vertex* v) {
  if (v == NULL) return;
  while (v->parent != NULL) {
    if (v->parent->parent == NULL) {
      // Zig step
      small_rotation(v);
      break;
    }
    // Zig-Zig or Zig-Zag.
    big_rotation(v);
  }
  root = v;
}

// Searches for the given key in the tree with the given root
// and calls splay for the deepest visited node after that.
// If found, returns a pointer to the node with the given key.
// Otherwise, returns a pointer to the node with the smallest
// bigger key (next value in the order).
// If the key is bigger than all keys in the tree, 
// returns NULL.
Vertex* find(Vertex*& root, int key) {
  Vertex* v = root;     // current node
  Vertex* last = root;  // last(previous) node
  
  // keep track of the next value during searching
  Vertex* next = NULL;  // keep track of next node with key greater than or equal to key.

  while (v != NULL) {  // not rea
    if (v->key >= key && (next == NULL || v->key < next->key)) {
      // current key more closer to the next value or equal value.
      next = v;  
    }
    last = v;    // previous node.  v == NULL  let us break out of the loop. Hence you should 

    if (v->key == key) {  // found
      break;      
    }

    // > or <
    if (v->key < key) {
      v = v->right;
    } else {
      // v->key > key
      v = v->left;
    }
  }

  // after find makes splay to move the frequently-accessed elements up towards the top of the tree.
  splay(root, last);   // splay  the deepest visited node.
  return next;         // NULL if no element >= key
}

void split(Vertex* root, int key, Vertex*& left, Vertex*& right) {
  right = find(root, key);  // return node with key equal to or closest to key
  splay(root, right);

  if (right == NULL) {
    // No element with key >= key in the given tree with root. Hence, can not split.
    left = root;
    return;
  }

  left = right->left;
  right->left = NULL;  // delete the left node.

  if (left != NULL) {
    left->parent = NULL;
  }

  // only right and left are affected.
  update(left);
  update(right);
}

Vertex* merge(Vertex* left, Vertex* right) {
  // all elements in left subtree are smaller than those in the right subtree
  // If one of tree is empty, then return another. Assume only one of them is empty.
  if (left == NULL) return right;
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

// Code that uses splay tree to solve the problem

// global variable
Vertex* root = NULL;

void insert(int x) {
  // Insert with the help merge and split.
  Vertex* left = NULL;
  Vertex* right = NULL;
  Vertex* new_vertex = NULL;  
  split(root, x, left, right);  
  
  // check the presence of x in the tree
  if (right == NULL || right->key != x) {
    // no element with key >= x or key > x.
    // x does not appear in the tree.
    new_vertex = new Vertex(x, x, NULL, NULL, NULL);
  }

  // merge two times.
  root = merge(merge(left, new_vertex), right);
}

void erase(int x) {                   
  // Implement erase yourself

  Vertex* left = NULL;
  Vertex* middle = NULL; 
  Vertex* right = NULL;
  split(root, x, left, middle);  // all keys < x go into left and all keys >=x go to the middle.
  split(middle, x + 1, middle, right); // all keys <=x, namely =x go into middle, all keys > x go into right.
  root = merge(left, right);
  
}

bool find(int x) {  
  // Implement find yourself
  Vertex * result = find(root, x);
  if(result == NULL || result->key != x)
  {
    return false;
  }
  else
  {
    return true;
  }
}

long long sum(int from, int to) {
  Vertex* left = NULL;
  Vertex* middle = NULL;
  Vertex* right = NULL;
  split(root, from, left, middle);  // All keys < from go to the left. All keys >= from go to the middle. 
  split(middle, to + 1, middle, right); // All keys [from, to] go to the middle. All keys > to go the right.
  long long ans = 0;
  // Complete the implementation of sum
  if(middle != NULL) ans = middle->sum;

  root = merge(merge(left, middle), right);
  return ans;  
}

const int MODULO = 1000000001;

int main(){
  int n;
  scanf("%d", &n);
  int last_sum_result = 0;
  for (int i = 0; i < n; i++) {
    char buffer[10];
    scanf("%s", buffer);
    char type = buffer[0];
    switch (type) {
      case '+' : {
        int x;
        scanf("%d", &x);
        insert((x + last_sum_result) % MODULO);
      } break;
      case '-' : {
        int x;
        scanf("%d", &x);
        erase((x + last_sum_result) % MODULO);
      } break;            
      case '?' : {
        int x;
        scanf("%d", &x);
        printf(find((x + last_sum_result) % MODULO) ? "Found\n" : "Not found\n");
      } break;
      case 's' : {
        int l, r;
        scanf("%d %d", &l, &r);
        long long res = sum((l + last_sum_result) % MODULO, (r + last_sum_result) % MODULO);
        printf("%lld\n", res);
        last_sum_result = int(res % MODULO);
      }
    }
  }
  return 0;
}
