#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <typename T>
class Node
{
  public:
    T key;
    Node * next;
    Node * prev;
};

template <typename T>
class List 
{ 
  public:
    // date member
    Node<T> * head;
    Node<T> * tail;

    // constructor
    List(Node<T> * _head=NULL, Node<T> * _tail=NULL);
    // destrutor
    ~List();

    // supported operations 
    void push_front(T key);   // O(1)
    T top_front(); // O(1)
    void pop_front(); // O(1)
    void push_back(T key); //O(1)
    T top_back(); // O(1)
    void pop_back(); // O(1)
    vector<Node<T>*> find(T key);  // O(n) with n as the total count of the nodes in the list.
    void erase(T key);  // First find find, then erase. O(n)
    bool empty();       // O(1)
    void add_before(Node<T> *node, T key); // O(1)
    void add_after(Node<T> *node, T key);  // O(1)
};

template <typename T>
List<T>::List(Node<T> * _head, Node<T> *_tail)
{
  head = _head;
  tail = _tail;
}

template <typename T>
List<T>::~List()
{
    Node<T> * ptr_current_node = head;
    head = NULL;  // avoid that head points to released memory space

    // release all nodes from the memory by traverse the list.
    while(ptr_current_node != NULL)
    {
      Node<T> * ptr_next_node = ptr_current_node->next;
      delete ptr_current_node;
      ptr_current_node = ptr_next_node;  // update current
    }

    cout << "All nodes in the list are freed up!" << endl;
}

template <typename T>
void List<T>::push_front(T key)
{
  // O(1)
  // Allocate a new node
  Node<T> * new_node = new Node<T>;
  new_node->key = key;
  if(head == NULL)  // empty list
  {
    head = new_node;
    tail = new_node;
    new_node->prev = NULL;
    new_node->next = NULL;
  }
  else  // List is not empty.Update head, ptrs of previous head node
  {
    new_node->prev = NULL;
    new_node->next = head;
    head->prev = new_node;
    head = new_node;    
    // head points to the previous head node.Hence updated lastly
    // Otherwise, you should cache its value beforehand.
  }
}

template <typename T>
void List<T>::push_back(T key)
{
  // O(1)
  // Allocate a new node from the heap
  Node<T> * new_node = new Node<T>;
  new_node->key = key;

  if(head == NULL) // empty list, update head and tail
  {
    head = new_node;
    tail = new_node;
    new_node->prev = NULL;
    new_node->next = NULL;
  }
  else  // not empty, update tail, prev and next of the previous tail node
  {
    new_node->prev = tail;
    new_node->next = NULL;
    tail->next = new_node;
    tail = new_node;
  }
}

template <typename T>
T List<T>::top_front()
{
  // O(1)
  return head->key;
}

template <typename T>
T List<T>::top_back()
{
  // O(1)
  return tail->key;
}

template <typename T>
void List<T>::pop_front()
{
  // O(1)
  if(head == NULL)  // Empty list.
  {
    throw string("Empty List");
  }

  if(head == tail)  // one node
  {
    delete head;  // release heap memory pointed to by head
    head = NULL;
    tail = NULL;
  }
  else  // at least 2 Nodes
  {
    Node<T> * temp;
    temp = head->next;
    temp->prev = NULL;
    delete head;
    head = temp;
  }
}

template <typename T>
void List<T>::pop_back()
{
  // O(1)
  if(head == NULL) //  empty list
  {
    throw string("Empty List");
  }

  // not empty
  if(head == tail) // One node, after pop_back, become empty
  {
    delete tail;
    head = NULL;
    tail = NULL;
  }
  else  // At least two Nodes.
  {   
    Node<T> * second_to_last(tail->prev);
    delete tail;
    tail = second_to_last;
    second_to_last->next = NULL;
  } 
}

template <typename T>
vector<Node<T>*> List<T>::find(T key)
{// O(n) with n as the total count of the nodes in the list.

  vector<Node<T> *> target;  // empty vector of pointers

  if(head == NULL)  // empty list
  {
    return target;
  }

  // List not empty
  for(Node<T> * p(head); p != NULL; p = p->next)
  {
    if(p->key == key)
    {
      target.push_back(p);
    }
  }
  return target;
}

template <typename T>
void List<T>::erase(T key)
{// First find find, then erase. O(n)
  vector<Node<T>*> target = find(key);

  // Key does not appear/occur in the List
  if(target.empty())
  {
    throw string("No such key or empty list!");
  }

  // key appear 
  for(int i(0); i < target.size(); i++)
  {
    // erase the head node, i.e. pop_front
    if(target[i] == head)
    {
      pop_front();
      continue;
    }

    // erase the tail node, i.e. pop_back
    if(target[i] == tail)
    {
      pop_back();
      continue;
    }

    // Up to this point, erase the node in the middle
    target[i]->prev->next = target[i]->next;
    target[i]->next->prev = target[i]->prev;
    delete target[i];
    target[i] = NULL;
  }
}

template <typename T>
bool List<T>::empty()
{
  return head == NULL;
}

template <typename T>
void List<T>::add_before(Node<T> *node, T key)
{
  // O(1)
  Node<T> * new_node = new Node<T>;
  new_node->key = key;
  new_node->prev = node->prev;   // may be the NULL, new
  new_node->next = node;
  node->prev = new_node;

  if(new_node->prev != NULL) // new_node is not the head node
  {
    new_node->prev->next = new_node;
  }
  else  // new_node is the head node. Update the head
  {
    head = new_node;
  }
}

template <typename T>
void List<T>::add_after(Node<T> *node, T key) 
{
  // O(1)
  Node<T> * new_node = new Node<T>;
  new_node->key = key;
  new_node->prev = node;
  new_node->next = node->next; // may be NULL
  node->next = new_node;

  if(new_node != NULL)
  {
    new_node->next->prev = new_node;
  }
  else
  {
    tail = new_node;
  } 
}

int main()
{
  List<int> mylist;

  // test push_front
  mylist.push_front(10);
  mylist.push_front(11);

  cout << "push_front test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout <<  endl;

  // test push_back
  mylist.push_back(20);
  mylist.push_back(30);
  cout << "push_back test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl;

  // test find
  mylist.push_back(20);
  vector<Node<int> *> target = mylist.find(20);
  
  cout << "find test" << endl;
  for(int i(0); i < target.size(); i++)
  {
    cout << target[i]->key << endl;
  }

  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl;

  // test erase
  mylist.erase(20);
  cout << "erase test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl; 

  // add_before test
  mylist.add_before(mylist.tail->prev, 100);
  cout << "add_before test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl;

  // add_after test
  mylist.add_after(mylist.head->next, 200);
  cout << "add_after test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl;

  // pop_front  test
  mylist.pop_front();
  cout << "pop_front test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl;

  // pop_back test
  mylist.pop_back();
  cout << "pop_back test" << endl;
  for(Node<int> *p(mylist.head); p != NULL; p = p->next)
  {
    cout << p->key << "-->";
  }
  cout << endl;

  // top_back top_front test
  cout << "top_front " << mylist.top_front() << endl;
  cout << "top_back  " << mylist.top_back() << endl;

  // empty test
  cout << "empty" << mylist.empty() << endl;

  return 0;
}
