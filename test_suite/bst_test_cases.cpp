#include <algorithm>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
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

    EXPECT_TRUE(std::is_sorted(stored_values.begin(), stored_values.end()));

    vector<int> expected = {10, 25, 30, 50, 60, 75, 85};
    EXPECT_EQ(stored_values, expected);
}

TEST(BSTTest, MinMax)
{
    BST<int> bst;
    vector<int> inputs = {20, 10, 30, 5, 25, 35};
    for (int i : inputs)
        bst.insert(i);

    EXPECT_EQ(bst.get_min(), 5);
    EXPECT_EQ(bst.get_max(), 35);
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
    EXPECT_THROW(bst.get_successor(100), std::runtime_error);
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
    EXPECT_THROW(bst.get_predecessor(100), std::runtime_error);
}

TEST(BSTTest, RemoveLeaf)
{
    BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);

    bst.remove(30);
    EXPECT_FALSE(bst.contains(30));
    EXPECT_EQ(bst.size(), 2);
    EXPECT_EQ(bst.get_root()->get_left(), nullptr);
}

TEST(BSTTest, RemoveNodeWithOneChild)
{
    BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(20);

    bst.remove(30);
    EXPECT_FALSE(bst.contains(30));
    EXPECT_TRUE(bst.contains(20));
    EXPECT_EQ(bst.size(), 2);
    EXPECT_EQ(bst.get_root()->get_left()->get_data(), 20);
}

TEST(BSTTest, RemoveNodeWithTwoChildren)
{
    BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(20);
    bst.insert(40);

    bst.remove(30);

    EXPECT_FALSE(bst.contains(30));
    EXPECT_TRUE(bst.contains(20));
    EXPECT_TRUE(bst.contains(40));
    EXPECT_EQ(bst.size(), 3);

    vector<int> stored_values;
    capture_inorder(bst.get_root(), stored_values);
    EXPECT_TRUE(std::is_sorted(stored_values.begin(), stored_values.end()));
}

TEST(BSTTest, RemoveRoot)
{
    BST<int> bst;
    bst.insert(50);
    bst.insert(30);
    bst.insert(70);

    bst.remove(50);

    EXPECT_FALSE(bst.contains(50));
    EXPECT_EQ(bst.size(), 2);

    if (bst.get_root()->get_data() == 30)
    {
        EXPECT_EQ(bst.get_root()->get_right()->get_data(), 70);
    }
    else
    {
        EXPECT_EQ(bst.get_root()->get_data(), 70);
        EXPECT_EQ(bst.get_root()->get_left()->get_data(), 30);
    }
}

TEST(BSTTest, DuplicateInsertion)
{
    BST<int> bst;
    bst.insert(10);
    bst.insert(10);
    EXPECT_EQ(bst.size(), 2);
    EXPECT_EQ(bst.get_root()->get_left()->get_data(), 10);

    vector<int> stored_values;
    capture_inorder(bst.get_root(), stored_values);
    EXPECT_EQ(stored_values.size(), 2);
    EXPECT_EQ(stored_values[0], 10);
    EXPECT_EQ(stored_values[1], 10);
}

TEST(BSTTest, EmptyTreeOperations)
{
    BST<int> bst;
    EXPECT_EQ(bst.size(), 0);
    EXPECT_FALSE(bst.contains(10));

    EXPECT_NO_THROW(bst.remove(10));

    EXPECT_THROW(bst.get_min(), std::runtime_error);
    EXPECT_THROW(bst.get_max(), std::runtime_error);
}

TEST(BSTTest, SingleElementOperations)
{
    BST<int> bst;
    bst.insert(100);

    EXPECT_EQ(bst.get_min(), 100);
    EXPECT_EQ(bst.get_max(), 100);

    bst.remove(100);
    EXPECT_TRUE(bst.is_empty());
}

TEST(BSTTest, StringTypeSupport)
{
    BST<std::string> bst;
    bst.insert("banana");
    bst.insert("apple");
    bst.insert("cherry");

    EXPECT_EQ(bst.get_min(), "apple");
    EXPECT_EQ(bst.get_max(), "cherry");

    EXPECT_TRUE(bst.contains("banana"));

    bst.remove("banana");
    EXPECT_FALSE(bst.contains("banana"));
    EXPECT_TRUE(bst.contains("apple"));
    EXPECT_TRUE(bst.contains("cherry"));
}