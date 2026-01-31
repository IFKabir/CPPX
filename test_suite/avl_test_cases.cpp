#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <random>
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
}

TEST(AVLTreeTest, RotationLLCase)
{
    AVLTree<int> tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20) << "Root should be 20 after LL rotation";
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
}

TEST(AVLTreeTest, RotationRRCase)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20) << "Root should be 20 after RR rotation";
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
}

TEST(AVLTreeTest, RotationLRCase)
{
    AVLTree<int> tree;
    tree.insert(30);
    tree.insert(10);
    tree.insert(20);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20) << "Root should be 20 after LR rotation";
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
}

TEST(AVLTreeTest, RotationRLCase)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(30);
    tree.insert(20);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 20) << "Root should be 20 after RL rotation";
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 10);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
    EXPECT_EQ(get_tree_height(tree), 2);
}

TEST(AVLTreeTest, DuplicateInsertion)
{
    AVLTree<int> tree;
    tree.insert(10);
    tree.insert(10);

    EXPECT_EQ(tree.size(), 1) << "Duplicates should not increase size";
    EXPECT_EQ(tree.get_root()->get_data(), 10);
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
    EXPECT_LE(h, 15) << "Height should be logarithmic (O(log N)), not linear";

    int root_val = tree.get_root()->get_data();
    EXPECT_GT(root_val, 100);
    EXPECT_LT(root_val, 900);
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
}

TEST(AVLTreeTest, RemovalWithRebalancing)
{
    AVLTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(40);

    tree.remove(10);
    EXPECT_EQ(tree.get_root()->get_data(), 30) << "Tree should rebalance (rotate left) after deletion";
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 20);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 40);
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
}