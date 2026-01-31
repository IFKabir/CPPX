#include <gtest/gtest.h>
#include <string>
#include <utility>
#include <vector>

#include "cppx.h"

using namespace std;
using namespace stl_ext;

TEST(BinaryTreeTest, DefaultConstructor)
{
    BinaryTree<int> tree;
    EXPECT_TRUE(tree.is_empty());
    EXPECT_EQ(tree.size(), 0);
    EXPECT_EQ(tree.get_root(), nullptr);
}

TEST(BinaryTreeTest, SetRoot)
{
    BinaryTree<int> tree;

    tree.set_root(BinaryTree<int>::make_node(10));
    EXPECT_FALSE(tree.is_empty());
    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(tree.get_root()->get_data(), 10);

    tree.set_root(BinaryTree<int>::make_node(99));
    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(tree.get_root()->get_data(), 99);

    tree.set_root(nullptr);
    EXPECT_TRUE(tree.is_empty());
    EXPECT_EQ(tree.size(), 0);
}

TEST(BinaryTreeTest, MakeNodeFactory)
{
    auto node = BinaryTree<int>::make_node(5);
    EXPECT_EQ(node->get_data(), 5);
    EXPECT_EQ(node->get_left(), nullptr);

    auto left = BinaryTree<int>::make_node(1);
    auto right = BinaryTree<int>::make_node(2);
    auto root = BinaryTree<int>::make_node(3, move(left), move(right));

    EXPECT_EQ(root->get_data(), 3);
    ASSERT_NE(root->get_left(), nullptr);
    ASSERT_NE(root->get_right(), nullptr);
    EXPECT_EQ(root->get_left()->get_data(), 1);
}

TEST(BinaryTreeTest, BuildRightSkewed)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(1));

    Node<int> *curr = tree.get_root();
    for (int i = 2; i <= 5; ++i)
    {
        tree.set_right(curr, BinaryTree<int>::make_node(i));
        curr = curr->get_right();
    }

    EXPECT_EQ(tree.size(), 5);
    EXPECT_EQ(tree.get_root()->get_right()->get_data(), 2);
}

TEST(BinaryTreeTest, BuildLeftSkewed)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(1));

    Node<int> *curr = tree.get_root();
    for (int i = 2; i <= 5; ++i)
    {
        tree.set_left(curr, BinaryTree<int>::make_node(i));
        curr = curr->get_left();
    }

    EXPECT_EQ(tree.size(), 5);
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), 2);
}

TEST(BinaryTreeTest, SetChildrenLogic)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(10));
    auto root = tree.get_root();

    tree.set_left(root, BinaryTree<int>::make_node(5));
    ASSERT_NE(root->get_left(), nullptr);
    EXPECT_EQ(root->get_left()->get_data(), 5);
    EXPECT_EQ(tree.size(), 2);

    tree.set_right(root, BinaryTree<int>::make_node(15));
    ASSERT_NE(root->get_right(), nullptr);
    EXPECT_EQ(root->get_right()->get_data(), 15);
    EXPECT_EQ(tree.size(), 3);
}

TEST(BinaryTreeTest, PruneSubtree)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(10));
    auto root = tree.get_root();

    tree.set_left(root, BinaryTree<int>::make_node(20));
    tree.set_left(root->get_left(), BinaryTree<int>::make_node(30));

    EXPECT_EQ(tree.size(), 3);

    tree.set_left(root, nullptr);

    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(root->get_left(), nullptr);
}

TEST(BinaryTreeTest, OverwriteSubtreeUpdateSize)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(1));
    auto root = tree.get_root();

    auto left_child = BinaryTree<int>::make_node(2);
    left_child->set_left(BinaryTree<int>::make_node(3));
    left_child->set_right(BinaryTree<int>::make_node(4));

    tree.set_left(root, move(left_child));
    EXPECT_EQ(tree.size(), 4);

    tree.set_left(root, BinaryTree<int>::make_node(99));

    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(root->get_left()->get_data(), 99);
    EXPECT_EQ(root->get_left()->get_left(), nullptr);
}

TEST(BinaryTreeTest, CopyConstructorDeepCopy)
{
    BinaryTree<int> original;
    original.set_root(BinaryTree<int>::make_node(1));
    original.set_left(original.get_root(), BinaryTree<int>::make_node(2));
    original.set_right(original.get_root(), BinaryTree<int>::make_node(3));

    BinaryTree<int> copy = original;

    EXPECT_EQ(copy.size(), 3);
    EXPECT_NE(copy.get_root(), original.get_root());
    EXPECT_NE(copy.get_root()->get_left(), original.get_root()->get_left());

    EXPECT_EQ(copy.get_root()->get_data(), 1);
    EXPECT_EQ(copy.get_root()->get_left()->get_data(), 2);
}

TEST(BinaryTreeTest, CopyAssignmentOperator)
{
    BinaryTree<int> original;
    original.set_root(BinaryTree<int>::make_node(100));

    BinaryTree<int> copy;
    copy.set_root(BinaryTree<int>::make_node(5));

    copy = original;

    EXPECT_EQ(copy.size(), 1);
    EXPECT_EQ(copy.get_root()->get_data(), 100);
    EXPECT_NE(copy.get_root(), original.get_root());
}

TEST(BinaryTreeTest, CopySelfAssignment)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(10));

    tree = tree;

    EXPECT_EQ(tree.size(), 1);
    EXPECT_EQ(tree.get_root()->get_data(), 10);
}

TEST(BinaryTreeTest, MoveConstructor)
{
    BinaryTree<int> original;
    original.set_root(BinaryTree<int>::make_node(10));

    BinaryTree<int> moved_to(move(original));

    EXPECT_TRUE(original.is_empty());
    EXPECT_EQ(original.get_root(), nullptr);

    EXPECT_EQ(moved_to.size(), 1);
    EXPECT_EQ(moved_to.get_root()->get_data(), 10);
}

TEST(BinaryTreeTest, MoveAssignment)
{
    BinaryTree<int> original;
    original.set_root(BinaryTree<int>::make_node(42));

    BinaryTree<int> target;
    target.set_root(BinaryTree<int>::make_node(1));

    target = move(original);

    EXPECT_TRUE(original.is_empty());
    EXPECT_EQ(target.size(), 1);
    EXPECT_EQ(target.get_root()->get_data(), 42);
}

TEST(BinaryTreeTest, MoveSelfAssignment)
{
    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(10));

    tree = move(tree);

    EXPECT_FALSE(tree.is_empty());
    if (!tree.is_empty())
    {
        EXPECT_EQ(tree.get_root()->get_data(), 10);
    }
}

TEST(BinaryTreeTest, StringTypeSupport)
{
    BinaryTree<std::string> tree;
    tree.set_root(BinaryTree<std::string>::make_node("Root"));
    tree.set_left(tree.get_root(), BinaryTree<std::string>::make_node("Left"));

    EXPECT_EQ(tree.size(), 2);
    EXPECT_EQ(tree.get_root()->get_data(), "Root");
    EXPECT_EQ(tree.get_root()->get_left()->get_data(), "Left");

    BinaryTree<std::string> copy = tree;
    EXPECT_EQ(copy.get_root()->get_data(), "Root");
    EXPECT_NE(copy.get_root(), tree.get_root());
}

TEST(BinaryTreeTest, TraversalSmokeTest)
{
    BinaryTree<int> tree;

    EXPECT_NO_THROW(tree.print_preorder());
    EXPECT_NO_THROW(tree.print_inorder());

    tree.set_root(BinaryTree<int>::make_node(1));
    tree.set_left(tree.get_root(), BinaryTree<int>::make_node(2));

    EXPECT_NO_THROW(tree.print_postorder());
    EXPECT_NO_THROW(tree.print_levelorder());
}