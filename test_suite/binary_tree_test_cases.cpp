#include <gtest/gtest.h>

#include <utility>
#include <vector>

#include "cppx.h"

using namespace std;
using namespace stl_ext;

TEST(BinaryTreeStressTest, MoveSemantics)
{
    BinaryTree<int> tree1;
    tree1.set_root(BinaryTree<int>::make_node(10));

    BinaryTree<int> tree2(std::move(tree1));
    EXPECT_TRUE(tree1.is_empty()) << "Tree1 should be empty after move constructor";
    EXPECT_EQ(tree2.size(), 1) << "Tree2 should have inherited the node";

    BinaryTree<int> tree3;
    tree3.set_root(BinaryTree<int>::make_node(50));
    tree3 = std::move(tree2);

    EXPECT_TRUE(tree2.is_empty()) << "Tree2 should be empty after move assignment";
    ASSERT_NE(tree3.get_root(), nullptr);
    EXPECT_EQ(tree3.get_root()->get_data(), 10) << "Tree3 should now hold moved data (10) instead of 50";
}

TEST(BinaryTreeStressTest, DeepCopyAndTraversal)
{
    BinaryTree<int> deep_tree;
    deep_tree.set_root(BinaryTree<int>::make_node(0));
    Node<int> *current = deep_tree.get_root();

    for (int i = 1; i < 100; ++i)
    {
        deep_tree.set_left(current, BinaryTree<int>::make_node(i));
        current = current->get_left();
    }

    EXPECT_EQ(deep_tree.size(), 100);
    ASSERT_NE(current, nullptr);
    EXPECT_EQ(current->get_data(), 99);

    BinaryTree<int> deep_copy = deep_tree;
    EXPECT_EQ(deep_copy.size(), 100);

    EXPECT_NE(deep_copy.get_root(), deep_tree.get_root()) << "Roots should point to different memory addresses";
    EXPECT_EQ(deep_copy.get_root()->get_data(), deep_tree.get_root()->get_data());
}

TEST(BinaryTreeStressTest, OverwriteStructure)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(1));
    auto r = tree.get_root();

    tree.set_left(r, BinaryTree<int>::make_node(2));
    tree.set_left(r->get_left(), BinaryTree<int>::make_node(3));
    tree.set_right(r->get_left(), BinaryTree<int>::make_node(4));

    EXPECT_EQ(tree.size(), 4);

    tree.set_left(r, BinaryTree<int>::make_node(10));

    EXPECT_EQ(tree.size(), 2) << "Size should decrease after overwriting a subtree";
    ASSERT_NE(r->get_left(), nullptr);
    EXPECT_EQ(r->get_left()->get_data(), 10);
    EXPECT_EQ(r->get_left()->get_left(), nullptr) << "New node should have no children";
}

TEST(BinaryTreeStressTest, EmptyTree)
{
    BinaryTree<float> empty_f;

    EXPECT_EQ(empty_f.size(), 0);
    EXPECT_TRUE(empty_f.is_empty());

    BinaryTree<float> empty_copy(empty_f);
    EXPECT_TRUE(empty_copy.is_empty());
}

TEST(BinaryTreeStressTest, AdvancedNodeConstruction)
{
    auto left_child = BinaryTree<int>::make_node(20);
    auto right_child = BinaryTree<int>::make_node(30);

    auto parent = BinaryTree<int>::make_node(10, std::move(left_child), std::move(right_child));

    BinaryTree<int> tree;
    tree.set_root(std::move(parent));

    EXPECT_EQ(tree.size(), 3);
    ASSERT_NE(tree.get_root(), nullptr);
    EXPECT_EQ(tree.get_root()->get_data(), 10);

    ASSERT_NE(tree.get_root()->get_left(), nullptr);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 20);

    ASSERT_NE(tree.get_root()->get_right(), nullptr);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 30);
}