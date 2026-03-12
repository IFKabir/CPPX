#include <algorithm>
#include <cmath>
#include <gtest/gtest.h>
#include <random>
#include <string>
#include <vector>

#include "../include/cppx.h"

using namespace std;
using namespace stl_ext;

TEST(RBTreeTest, BasicInsertionAndSearch)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(5);
    tree.insert(15);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.contains(10));
    EXPECT_TRUE(tree.contains(5));
    EXPECT_TRUE(tree.contains(15));
    EXPECT_FALSE(tree.contains(20));
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, RootIsBlack)
{
    RBTree<int> tree;
    tree.insert(42);

    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_color(), Color::BLACK);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, LeftRotationCase)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.validate_rb_properties());
    EXPECT_TRUE(tree.contains(10));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(30));
}

TEST(RBTreeTest, RightRotationCase)
{
    RBTree<int> tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, LeftRightCase)
{
    RBTree<int> tree;
    tree.insert(30);
    tree.insert(10);
    tree.insert(20);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, RightLeftCase)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(30);
    tree.insert(20);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, ColorFlipping)
{
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    tree.insert(5);

    EXPECT_EQ(tree.size(), 4);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, DuplicateInsertion)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(10);

    EXPECT_EQ(tree.size(), 1);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, SequentialInsertionStressTest)
{
    RBTree<int> tree;
    int n = 1000;

    for (int i = 0; i < n; ++i)
    {
        tree.insert(i);
    }

    EXPECT_EQ(tree.size(), n);
    EXPECT_TRUE(tree.validate_rb_properties());

    int bh = tree.get_black_height();
    EXPECT_GT(bh, 0);

    for (int i = 0; i < n; ++i)
    {
        EXPECT_TRUE(tree.contains(i));
    }
}

TEST(RBTreeTest, ReverseSequentialInsertion)
{
    RBTree<int> tree;
    int n = 500;

    for (int i = n; i > 0; --i)
    {
        tree.insert(i);
    }

    EXPECT_EQ(tree.size(), n);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, RandomInsertionStressTest)
{
    RBTree<int> tree;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(1, 50000);
    int n = 1000;
    std::set<int> reference;

    for (int i = 0; i < n; ++i)
    {
        int val = dist(rng);
        tree.insert(val);
        reference.insert(val);
    }

    EXPECT_EQ(tree.size(), static_cast<int>(reference.size()));
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, ToSortedVector)
{
    RBTree<int> tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);

    auto sorted = tree.to_sorted_vector();
    ASSERT_EQ(sorted.size(), 5u);
    EXPECT_EQ(sorted[0], 20);
    EXPECT_EQ(sorted[1], 30);
    EXPECT_EQ(sorted[2], 40);
    EXPECT_EQ(sorted[3], 50);
    EXPECT_EQ(sorted[4], 70);
}

TEST(RBTreeTest, StaticColorHelpers)
{

    EXPECT_TRUE(RBTree<int>::is_black(nullptr));
    EXPECT_FALSE(RBTree<int>::is_red(nullptr));
    EXPECT_EQ(RBTree<int>::get_color(nullptr), Color::BLACK);

    RBTree<int> tree;
    tree.insert(10);
    EXPECT_TRUE(RBTree<int>::is_black(tree.get_root()));
}

TEST(RBTreeTest, RemoveLeafNode)
{
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);

    tree.remove(10);

    EXPECT_EQ(tree.size(), 2);
    EXPECT_FALSE(tree.contains(10));
    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(30));
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, RemoveNodeWithOneChild)
{
    RBTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    tree.insert(25);

    tree.remove(30);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_FALSE(tree.contains(30));
    EXPECT_TRUE(tree.contains(25));
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, RemoveNodeWithTwoChildren)
{
    RBTree<int> tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    tree.remove(50);

    EXPECT_EQ(tree.size(), 6);
    EXPECT_FALSE(tree.contains(50));
    EXPECT_TRUE(tree.validate_rb_properties());

    EXPECT_TRUE(tree.contains(20));
    EXPECT_TRUE(tree.contains(30));
    EXPECT_TRUE(tree.contains(40));
    EXPECT_TRUE(tree.contains(60));
    EXPECT_TRUE(tree.contains(70));
    EXPECT_TRUE(tree.contains(80));
}

TEST(RBTreeTest, RemoveRoot)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.remove(10);

    EXPECT_TRUE(tree.is_empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, RemoveFromEmptyTree)
{
    RBTree<int> tree;
    EXPECT_NO_THROW(tree.remove(10));
    EXPECT_EQ(tree.size(), 0);
}

TEST(RBTreeTest, RemoveNonexistentValue)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.remove(99);

    EXPECT_EQ(tree.size(), 3);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, DeleteAllNodesSequentially)
{
    RBTree<int> tree;
    vector<int> values = {10, 20, 30, 40, 50, 60, 70};

    for (int v : values)
        tree.insert(v);

    for (int v : values)
    {
        tree.remove(v);
        EXPECT_TRUE(tree.validate_rb_properties());
    }

    EXPECT_TRUE(tree.is_empty());
}

TEST(RBTreeTest, RandomDeletionStressTest)
{
    RBTree<int> tree;
    std::mt19937 rng(42);
    int n = 500;

    vector<int> values;
    for (int i = 0; i < n; ++i)
        values.push_back(i);

    std::shuffle(values.begin(), values.end(), rng);
    for (int v : values)
        tree.insert(v);

    EXPECT_EQ(tree.size(), n);
    EXPECT_TRUE(tree.validate_rb_properties());

    std::shuffle(values.begin(), values.end(), rng);
    for (int i = 0; i < n / 2; ++i)
    {
        tree.remove(values[i]);
        EXPECT_TRUE(tree.validate_rb_properties());
    }

    EXPECT_EQ(tree.size(), n - n / 2);

    for (int i = n / 2; i < n; ++i)
    {
        EXPECT_TRUE(tree.contains(values[i]));
    }
}

TEST(RBTreeTest, BlackHeightConsistency)
{
    RBTree<int> tree;
    for (int i = 1; i <= 100; ++i)
    {
        tree.insert(i);
        EXPECT_GT(tree.get_black_height(), 0);
    }
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, ClearTree)
{
    RBTree<int> tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    tree.clear();

    EXPECT_TRUE(tree.is_empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_TRUE(tree.validate_rb_properties());
}

TEST(RBTreeTest, GetMinMax)
{
    RBTree<int> tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(80);

    EXPECT_EQ(tree.get_min(), 20);
    EXPECT_EQ(tree.get_max(), 80);
}

TEST(RBTreeTest, GetSuccessorPredecessor)
{
    RBTree<int> tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(70);
    tree.insert(20);
    tree.insert(40);
    tree.insert(60);
    tree.insert(80);

    EXPECT_EQ(tree.get_successor(30), 40);
    EXPECT_EQ(tree.get_successor(50), 60);
    EXPECT_EQ(tree.get_predecessor(70), 60);
    EXPECT_EQ(tree.get_predecessor(50), 40);
}

TEST(RBTreeTest, MoveSemantics)
{
    RBTree<int> t1;
    t1.insert(10);
    t1.insert(20);
    t1.insert(30);

    RBTree<int> t2(std::move(t1));

    EXPECT_TRUE(t1.is_empty());
    EXPECT_EQ(t2.size(), 3);
    EXPECT_TRUE(t2.contains(10));
    EXPECT_TRUE(t2.contains(20));
    EXPECT_TRUE(t2.contains(30));
}

TEST(RBTreeTest, StringTypeSupport)
{
    RBTree<std::string> tree;
    tree.insert("cherry");
    tree.insert("apple");
    tree.insert("banana");
    tree.insert("date");

    EXPECT_EQ(tree.size(), 4);
    EXPECT_TRUE(tree.validate_rb_properties());
    EXPECT_TRUE(tree.contains("apple"));
    EXPECT_TRUE(tree.contains("banana"));

    tree.remove("cherry");
    EXPECT_EQ(tree.size(), 3);
    EXPECT_FALSE(tree.contains("cherry"));
    EXPECT_TRUE(tree.validate_rb_properties());
}
