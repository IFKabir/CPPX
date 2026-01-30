#include <gtest/gtest.h>

#include <algorithm>
#include <stdexcept>
#include <vector>

#include "cppx.h"

using namespace std;
using namespace stl_ext;

template <typename T> void capture_inorder(const Node<T> *node, vector<T> &result)
{
    if (node == nullptr)
        return;
    capture_inorder(node->get_left(), result);
    result.push_back(node->get_data());
    capture_inorder(node->get_right(), result);
}

TEST(BSTTest, InsertAndSearch)
{
    BST<int> bst;

    bst.insert(50);
    bst.insert(30);
    bst.insert(20);
    bst.insert(40);
    bst.insert(70);
    bst.insert(60);
    bst.insert(80);

    EXPECT_EQ(bst.size(), 7);

    EXPECT_TRUE(bst.contains(50));
    EXPECT_TRUE(bst.contains(20));
    EXPECT_TRUE(bst.contains(80));

    EXPECT_FALSE(bst.contains(90));
    EXPECT_FALSE(bst.contains(10));
}

TEST(BSTTest, OrderingProperty)
{
    BST<int> bst;
    vector<int> values = {50, 25, 75, 10, 30, 60, 85};

    for (int v : values)
    {
        bst.insert(v);
    }

    vector<int> stored_values;
    capture_inorder(bst.get_root(), stored_values);

    EXPECT_TRUE(std::is_sorted(stored_values.begin(), stored_values.end()))
        << "Inorder traversal of a BST must be sorted ascendingly.";

    vector<int> expected = {10, 25, 30, 50, 60, 75, 85};
    EXPECT_EQ(stored_values, expected);
}

TEST(BSTTest, GetSuccessor)
{
    BST<int> bst;
    vector<int> inputs = {20, 10, 30, 5, 15, 25, 35};
    for (int i : inputs)
        bst.insert(i);

    EXPECT_EQ(bst.get_successor(20), 25);

    EXPECT_EQ(bst.get_successor(10), 15);

    EXPECT_EQ(bst.get_successor(15), 20);

    EXPECT_EQ(bst.get_successor(25), 30);

    EXPECT_THROW(bst.get_successor(35), std::runtime_error);
}

TEST(BSTTest, GetPredecessor)
{
    BST<int> bst;
    vector<int> inputs = {20, 10, 30, 5, 15, 25, 35};
    for (int i : inputs)
        bst.insert(i);

    EXPECT_EQ(bst.get_predecessor(20), 15);

    EXPECT_EQ(bst.get_predecessor(30), 25);

    EXPECT_EQ(bst.get_predecessor(25), 20);

    EXPECT_EQ(bst.get_predecessor(15), 10);

    EXPECT_THROW(bst.get_predecessor(5), std::runtime_error);
}

TEST(BSTTest, DuplicateInsertion)
{
    BST<int> bst;
    bst.insert(10);
    bst.insert(10);
    EXPECT_EQ(bst.size(), 2);
    EXPECT_EQ(bst.get_root()->get_left()->get_data(), 10);
}