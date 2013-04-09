#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#define BOOT_TEST_MODULE Skippy
#include <boost/test/unit_test.hpp>

#include <cmath>
#include <random>

#include "skip_list.h"

#define MAX_INSERT 10000

using namespace std;

float std_dev(int N) {
  return 0.5 * sqrt( (float) N);
}

struct RandomSList {
  SkipList<int> s_list;
  RandomSList() {
    default_random_engine generator;
    uniform_int_distribution<int> rand_int(0, MAX_INSERT);
    
    for (int i=0; i<MAX_INSERT/10; i++)
      s_list.insert(rand_int(generator));
  }
};

BOOST_FIXTURE_TEST_SUITE(test_list_function, RandomSList)

BOOST_AUTO_TEST_CASE(test_linking)
{
  Node<int> * row, * node;
  row = node = s_list.head;

  while (node->down) {
    node = row;
    while (node->right) {
      BOOST_CHECK_EQUAL(node, node->right->left);
      if (node->left)
	BOOST_CHECK_EQUAL(node, node->left->right);
      if (node->up)
	BOOST_CHECK_EQUAL(node, node->up->down);
      if (row->down)
	if (node->down)
	  BOOST_CHECK_EQUAL(node, node->down->up);
	else
	  BOOST_CHECK_MESSAGE(1<0, "Node is missing down pointer!\n");
      node = node->right;
    }
    row = row->down;
  }
}

BOOST_AUTO_TEST_CASE(test_find)
{
  s_list.insert(17);

  BOOST_CHECK_EQUAL(s_list.find(17)->get_value(), 17);
  BOOST_CHECK(!(s_list.find(17)->down));
  BOOST_CHECK(!s_list.find(MAX_INSERT+1));
}

BOOST_AUTO_TEST_CASE(test_remove)
{
  s_list.insert(1003);

  BOOST_CHECK(s_list.find(1003));
  BOOST_CHECK(s_list.remove(1003));
  BOOST_CHECK(!s_list.find(1003));
  BOOST_CHECK(!s_list.remove(MAX_INSERT+1));
}

BOOST_AUTO_TEST_CASE(test_ordering) 
{
  s_list.insert(1003);
  s_list.insert(1003); // Ensure a duplicate insert was attempted

  Node<int> * row, * node;
  row = node = s_list.head;

  while (node->down) {
    node = row;
    while (node->right) {
      BOOST_CHECK_LE(node->get_value(), node->right->get_value());
      if (node->up) 
	BOOST_CHECK_EQUAL(node->get_value(), node->up->get_value());
      if (node->down)
	BOOST_CHECK_EQUAL(node->get_value(), node->down->get_value());
      node = node->right;
    }
    row = row->down;
  }
}

/* This test checks that the probability of raising a level is approximately
   one half. Warnings will be raise if observed probabilities exceed three 
   standard deviations. */

BOOST_AUTO_TEST_CASE(test_new_levels)
{
  Node<int> * row, * node;
  row = s_list.head;
  while (row->down) 
    row = row->down;
  while (row->up) {
    node = row;
    // Node counts start at -1 to avoid counting sentinels 
    int nodes = -1, upnodes = -1;
    while (node->right) {
      if (node->up) 
	upnodes++;
      nodes++;
      node = node->right;
    }
    if (nodes < 10) break;
    BOOST_WARN_MESSAGE(abs((float) s_list.p_up*nodes-upnodes) < 
		       3*std_dev(nodes),
		       "Level increase rate > 3*sigma away from expected.\n"
		       << "Total nodes = " << nodes << "; Nodes pointg up = " 
		       << upnodes << "\n");
    row = row->up;
  }
}

BOOST_AUTO_TEST_CASE(test_copy_constructor)
{
  SkipList<int> new_list = s_list;

  //Compare the structures
  Node<int> *s_node, *s_row, *n_node, *n_row; 
  s_node = s_row = s_list.head;
  n_node = n_row = new_list.head;

  while (s_node->right) {
    BOOST_REQUIRE_MESSAGE(n_node->right,
			  "Copy constructed list wrong shape!\n");
    BOOST_CHECK_NE(n_node, s_node);
    BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
    s_node = s_node->right;
    n_node = n_node->right;
  }
  BOOST_CHECK_NE(n_node, s_node);
  BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
  BOOST_CHECK_MESSAGE(!n_node->right, "Copy constructed list wrong shape!\n");
  while (s_row->down) {
    BOOST_REQUIRE_MESSAGE(n_row->down, 
			  "Copy constructed list wrong shape!\n");
    s_node = s_row = s_row->down;
    n_node = n_row = n_row->down;
    while (s_node->right) {
      BOOST_REQUIRE_MESSAGE(n_node->right,
			    "Copy constructed list wrong shape!\n");
      BOOST_CHECK_NE(n_node, s_node);
      BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
      s_node = s_node->right;
      n_node = n_node->right;
      if (!s_row->down)
        BOOST_CHECK_MESSAGE(!n_node->down, 
			    "Copy constructed list wrong shape!\n");
      
    }
    BOOST_CHECK_NE(n_node, s_node);
    BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
    BOOST_CHECK_MESSAGE(!n_node->right, "Copy constructed list wrong shape!\n");
  }
}

BOOST_AUTO_TEST_CASE(test_assignment_operator)
{
  SkipList<int> new_list;
  default_random_engine generator;
  uniform_int_distribution<int> rand_int(0, MAX_INSERT);
  for (int i=0; i<MAX_INSERT/10; i++)
    new_list.insert(rand_int(generator));

  new_list = s_list;
  //Compare the structures
  Node<int> *s_node, *s_row, *n_node, *n_row; 
  s_node = s_row = s_list.head;
  n_node = n_row = new_list.head;

  while (s_node->right) {
    BOOST_REQUIRE_MESSAGE(n_node->right,
			  "Copy constructed list wrong shape!\n");
    BOOST_CHECK_NE(n_node, s_node);
    BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
    s_node = s_node->right;
    n_node = n_node->right;
  }
  BOOST_CHECK_NE(n_node, s_node);
  BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
  BOOST_CHECK_MESSAGE(!n_node->right, "Copy constructed list wrong shape!\n");
  while (s_row->down) {
    BOOST_REQUIRE_MESSAGE(n_row->down, 
			  "Copy constructed list wrong shape!\n");
    s_node = s_row = s_row->down;
    n_node = n_row = n_row->down;
    while (s_node->right) {
      BOOST_REQUIRE_MESSAGE(n_node->right,
			    "Copy constructed list wrong shape!\n");
      BOOST_CHECK_NE(n_node, s_node);
      BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
      s_node = s_node->right;
      n_node = n_node->right;
      if (!s_row->down)
        BOOST_CHECK_MESSAGE(!n_node->down, 
			    "Copy constructed list wrong shape!\n");
      
    }
    BOOST_CHECK_NE(n_node, s_node);
    BOOST_CHECK_EQUAL(n_node->get_value(), s_node->get_value());
    BOOST_CHECK_MESSAGE(!n_node->right, "Copy constructed list wrong shape!\n");
  }
}

BOOST_AUTO_TEST_SUITE_END()
