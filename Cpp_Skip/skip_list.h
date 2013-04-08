#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <random>

template <class T>
class Node {
 public:
  Node * left;
  Node * right;
  Node * up;
  Node * down;
  T value;
 Node(T value) : value(value) {
    left = right = up = down = NULL;
  };
  T get_value(void) { return value; };
  Node * get_right(void) { return right; };
  Node * get_left(void) { return left; };
  Node * get_up(void) { return up; };
  Node * get_down(void) { return down; };
};

template <class T>
class SkipList {
  //Insert node with value to right of node
  void list_insert(Node<T>* node, T Value); 
  void raise_level(Node<T>* node, T value);
  void insert_level();
  bool raise_check(void) { return (p_up > distribution(generator)); };
  void delete_list(Node<T>* node);
  void delete_node(Node<T>* node);
  void copy(SkipList<T>& new_, const SkipList<T>& other);
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution;
  T p_inf;
  T n_inf;

 public:
  Node<T> * head;
  Node<T> * tail;
  SkipList();
  SkipList(const SkipList<T>& list_to_copy);
  SkipList<T>& operator=(const SkipList<T>& rhs);
  ~SkipList();
  int levels;
  float p_up;
  void insert(const T value);
  bool remove(T value);
  Node<T> * find(T value);
  void reset();
  void print_level(int level);
  void print_all();
};

#include "skip_list.tpp"

#endif
