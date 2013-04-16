#include <iostream>
#include <limits>
#include <random>
#include <time.h>
#include <cmath>
#include "skip_list.h"

using namespace std;
using namespace skip;

template <class T>
SkipList<T>::SkipList() : 
  distribution(uniform_int_distribution<unsigned int>
	       (0, numeric_limits<unsigned int>::max())),
  p_inf(numeric_limits<T>::has_infinity ? numeric_limits<T>::infinity() :
	numeric_limits<T>::max()),	
  n_inf(numeric_limits<T>::has_infinity ? -numeric_limits<T>::infinity() :
	numeric_limits<T>::min()),  head(new Node<T>(n_inf)), 
  tail(new Node<T>(p_inf)), levels(1), p_up(0.5), n_elem(0) { 
  head->right = tail; tail->left = head; 
  set_seed();
}

template <class T> void
SkipList<T>::set_seed(unsigned int seed=0) {
  if (!seed) {
    try {
      std::random_device rd;
      seed = rd();
    } catch (...) {
      seed = time(NULL);
    }
  }
  generator.seed(seed);
}

template <class T>
SkipList<T>::SkipList(const SkipList<T>& other) : p_inf(other.p_inf),
						  n_inf(other.n_inf),
						  levels(other.levels), 
						  p_up(other.p_up), n_elem(0) {
  head = new Node<T>(n_inf);
  tail = new Node<T>(p_inf);
  head->right = tail;
  tail->left = head;
  copy(*this, other);
  set_seed();
}

template <class T> SkipList<T>&
SkipList<T>::operator=( const SkipList<T>& rhs ) {
  if (this != &rhs) {
    SkipList<T> tmp(rhs);
    reset();
    swap(head, tmp.head);
    swap(tail, tmp.tail);
    swap(levels, tmp.levels);
    swap(p_up, tmp.p_up);
    swap(generator, tmp.generator);
    swap(distribution, tmp.distribution);
  }
  set_seed();
  return *this;
}

template <class T> void
SkipList<T>::copy(SkipList<T>& new_, const SkipList<T>& other) {
  Node<T>* n_start, *n_end, * n_node, * o_row, * o_node;
  // Copy top row making sure to miss sentinels
  n_node = head;
  for (o_node=other.head->right; o_node->right; o_node=o_node->right) {
    n_node->right = new Node<T>(o_node->get_value());
    n_node->right->left = n_node;
    n_node = n_node->right;
  }
  n_node->right = new_.tail;
  new_.tail->left = n_node;

  // Copy everything else
  o_row = other.head;
  n_end = new_.tail;
  n_start = new_.head;
  while (o_row->down) {
    o_row = o_node = o_row->down;
    n_start->down = new Node<T>(o_row->get_value());
    n_start->down->up = n_start;
    n_start = n_start->down;
    n_node = n_start;
    while (o_node->right) {
      o_node = o_node->right;
      n_node->right = new Node<T>(o_node->get_value());
      n_node->right->left = n_node;
      n_node = n_node->right;
    }
    n_node->up = n_end;
    n_end->down = n_node;
    n_end = n_node;
  }
}

template <class T> void
SkipList<T>::list_insert(Node<T>* node, T value) {
  Node<T> * new_node = new Node<T>(value);
  new_node->left = node;
  new_node->right = node->right;
  new_node->right->left = new_node;
  node->right = new_node;
}

template <class T> void
SkipList<T>::raise_level(Node<T>* node, T value) {
  Node<T> * upnode = node->left;
  while (!(upnode->up) && upnode->left)
    upnode = upnode->left;
  if (upnode->up)
    upnode = upnode->up;
  else {
    insert_level();
    upnode = head;
  }
  list_insert(upnode, value);
  upnode->right->down = node;
  node->up = upnode->right;
  if (raise_check()) raise_level(upnode->right, value);
}

template <class T> void
SkipList<T>::insert_level() {
  head->up = new Node<T>(n_inf);
  head->up->down = head;
  head = head->up;
  tail->up = new Node<T>(p_inf);
  tail->up->down = tail;
  tail = tail->up;
  head->right = tail;
  tail->left = head;
  levels++;
}

template <class T> bool
SkipList<T>::raise_check(void) {
  bool out = rand_check & 1;
  rand_check >>= 1;
  return out;
}

template <class T> void
SkipList<T>::insert(const T value) {
  if (value == n_inf || value == p_inf)
    return;
  rand_check = distribution(generator); // Initialize bit sequence for raises
  Node<T> * node = head;
  while (value >= node->get_value()) {
    if (value < node->right->get_value() && node->down) 
      node = node->down;
    else
      node = node->right;
  }
  n_elem++;
  list_insert(node->left, value);
  if (raise_check()) 
    raise_level(node->left, value);
}

template <class T> void
SkipList<T>::print_level(int level) {
  Node<T>* node = head;
  while (node->down) 
    node = node->down;
  for (int i=0; i<level; i++) 
    node = node->up;
  cout << "Printing level: " << level << endl << "  ";
  node = node->right;
  while (node->right) {
    cout << node->get_value() << "  ";
    node = node->right;
  }
  cout << endl;
}

template <class T> void
SkipList<T>::print_all() {
  for (int i=0; i<levels; i++) print_level(i);
}

template <class T> void
SkipList<T>::delete_list(Node<T> * node) {
  while (node->right) {
    node = node->right;
    delete node->left;
  }
  delete node;
}

template <class T> Node<T> *
SkipList<T>::find(T value) {
  Node<T> * node = head;
  while (value >= node->get_value()) {
    if (value == node->get_value()) {
      while (node->down)
	node = node->down;
      return node;
    } else if (value < node->right->get_value() && node->down) 
      node = node->down;
    else
      node = node->right;
  }
  return NULL;
}

template <class T> void
SkipList<T>::delete_node(Node<T>* node) {
  node->left->right = node->right;
  node->right->left = node->left;
  delete node;
}

template <class T> bool
SkipList<T>::remove(T value) {
  Node<T> * node = find(value);
  if (!node)
    return 0;
  while (node->up) {
    node = node->up;
    delete_node(node->down);
  }
  delete_node(node);
  n_elem--;
  return 1;
}

template <class T> void
SkipList<T>::reset() {
  Node<T> *node;
  node = head->right;        // Delete everything in between head and tail
  while (node->right) {
    node = node->right;
    delete node->left;
  }
  if (head->down) {          // Delete list below head
    node = head->down;
    while (node->down) {
      node = node->down;
      delete_list(node->up);
    }
    delete_list(node);
  }
  head->right = tail; // Reset head and tail state
  tail->left = head;
  head->down = tail->down = NULL;
  n_elem = 0;
}

template <class T> 
SkipList<T>::~SkipList() {
  reset(); 
  delete head; 
  delete tail;
}
