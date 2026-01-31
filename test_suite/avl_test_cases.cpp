#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>

#include "cppx.h"

using namespace std;
using namespace stl_ext;

template <typename T> int get_tree_height(const AVLTree<T> &tree)
{
    if (tree.is_empty())
        return 0;
    return tree.get_root()->get_height_val();
}

template <typename T> bool validate_avl_property(const Node<T> *node)
{
    if (!node)
        return true;

    int left_h = node->get_left() ? node->get_left()->get_height_val() : 0;
    int right_h = node->get_right() ? node->get_right()->get_height_val() : 0;

    if (std::abs(left_h - right_h) > 1)
        return false;

    int expected_h = 1 + std::max(left_h, right_h);
    if (node->get_height_val() != expected_h)
        return false;

    return validate_avl_property(node->get_left()) && validate_avl_property(node->get_right());
}

TEST(AVLTreeTest, BasicInsertionAndSearch)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contains(10));
    EXPECT_TRUE(tree.contains(5));
    EXPECT_TRUE(tree.contains(15));
    EXPECT_FALSE(tree.contains(20));
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RotationLLCase)
{
    AVLTree<int> tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RotationRRCase)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RotationLRCase)
{
    AVLTree<int> tree;
    tree.insert(30);
    tree.insert(10);
    tree.insert(20);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RotationRLCase)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(30);
    tree.insert(20);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, DuplicateInsertion)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(10);

    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(tree.get_root()->get_data(), 10);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, SequentialInsertionStressTest)
{
    AVLTree<int> tree;
    int n = 1000;

    for (int i = 0; i < n; ++i)
    {
        tree.insert(i);
    }

    EXPECT_EQ(tree.size(), n);

    int h = get_tree_height(tree);
    EXPECT_LE(h, 15);

    int root_val = tree.get_root()->get_data();
    EXPECT_GT(root_val, 100);
    EXPECT_LT(root_val, 900);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RandomInsertionStressTest)
{
    AVLTree<int> tree;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 10000);
    int n = 500;

    for (int i = 0; i < n; ++i)
    {
        tree.insert(dist(rng));
    }

    EXPECT_TRUE(validate_avl_property(tree.get_root()));
    EXPECT_LE(get_tree_height(tree), 2 * log2(n + 1));
}

TEST(AVLTreeTest, RemovalLeafNode)
{
    AVLTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    tree.remove(10);

    EXPECT_EQ(tree.size(), 2);
    EXPECT_FALSE(tree.contains(10));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(30));

    EXPECT_EQ(tree.get_root()->get_left(), nullptr);
    EXPECT_NE(tree.get_root()->get_right(), nullptr);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RemovalWithRebalancing)
{
    AVLTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(40);

    tree.remove(10);
    EXPECT_EQ(tree.get_root()->get_data(), 30);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 20);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 40);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RemovalTwoChildren)
{
    AVLTree<int> tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    tree.remove(30);

    EXPECT_EQ(tree.size(), 6);
    EXPECT_FALSE(tree.contains(30));

    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(40));

    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 40);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}

TEST(AVLTreeTest, RemoveRootCausingRotation)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.remove(20);

    EXPECT_EQ(tree.size(), 2);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));

    if (tree.get_root()->get_data() == 10)
        EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    else
    {
        EXPECT_EQ(tree.get_root()->get_data(), 30);
        EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    }
}

TEST(AVLTreeTest, DeleteAllNodes)
{
    AVLTree<int> tree;
    vector<int> values = {10, 20, 30, 40, 50};
    for (int v : values)
        tree.insert(v);

    for (int v : values)
    {
        tree.remove(v);
        EXPECT_TRUE(validate_avl_property(tree.get_root()));
    }

    EXPECT_TRUE(tree.is_empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(AVLTreeTest, EmptyTreeOperations)
{
    AVLTree<int> tree;
    EXPECT_NO_THROW(tree.remove(10));
    EXPECT_EQ(tree.size(), 0);
    EXPECT_FALSE(tree.contains(10));
    EXPECT_EQ(get_tree_height(tree), 0);
}

TEST(AVLTreeTest, MoveSemanticsAVL)
{
    AVLTree<int> tree1;
    tree1.insert(10);
    tree1.insert(20);

    AVLTree<int> tree2(std::move(tree1));

    EXPECT_TRUE(tree1.is_empty());
    EXPECT_EQ(tree2.size(), 2);
    EXPECT_TRUE(tree2.contains(10));
    EXPECT_TRUE(tree2.contains(20));

    EXPECT_GT(get_tree_height(tree2), 0);
    EXPECT_TRUE(validate_avl_property(tree2.get_root()));
}

TEST(AVLTreeTest, CopyConstructorDeepCopy)
{
    AVLTree<int> tree1;
    tree1.insert(10);
    tree1.insert(5);
    tree1.insert(15);

    AVLTree<int> tree2 = tree1;

    EXPECT_EQ(tree2.size(), 3);
    EXPECT_NE(tree1.get_root(), tree2.get_root());
    EXPECT_TRUE(validate_avl_property(tree2.get_root()));

    tree2.insert(20);
    EXPECT_EQ(tree2.size(), 4);
    EXPECT_EQ(tree1.size(), 3);
}

TEST(AVLTreeTest, StringTypeSupport)
{
    AVLTree<std::string> tree;
    tree.insert("banana");
    tree.insert("apple");
    tree.insert("cherry");
    tree.insert("date");

    EXPECT_EQ(tree.size(), 4);
    EXPECT_EQ(tree.get_root()->get_data(), "banana");
    EXPECT_TRUE(validate_avl_property(tree.get_root()));

    tree.remove("banana");
    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(validate_avl_property(tree.get_root()));
}