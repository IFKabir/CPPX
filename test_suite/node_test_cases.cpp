#include <gtest/gtest.h>
#include <memory>
#include <utility>

#include "cppx.h"

using namespace stl_ext;

TEST(NodeTest, ConstructorAndData)
{
    auto node = std::make_unique<Node<int>>(42);

    EXPECT_EQ(node->get_data(), 42);
    EXPECT_EQ(node->get_left(), nullptr);
    EXPECT_EQ(node->get_right(), nullptr);
    EXPECT_EQ(node->get_height_val(), 1) << "Default height should be 1";
}

TEST(NodeTest, ConstructorWithChildren)
{
    auto left = std::make_unique<Node<int>>(10);
    auto right = std::make_unique<Node<int>>(20);

    auto root = std::make_unique<Node<int>>(30, std::move(left), std::move(right));

    EXPECT_EQ(root->get_data(), 30);
    ASSERT_NE(root->get_left(), nullptr);
    ASSERT_NE(root->get_right(), nullptr);

    EXPECT_EQ(root->get_left()->get_data(), 10);
    EXPECT_EQ(root->get_right()->get_data(), 20);
}

TEST(NodeTest, HeightAccessors)
{
    Node<int> node(100);
    EXPECT_EQ(node.get_height_val(), 1);

    node.set_height_val(5);
    EXPECT_EQ(node.get_height_val(), 5);
}

TEST(NodeTest, CopyConstructorDeepCopy)
{
    auto root = std::make_unique<Node<int>>(5);
    root->set_left(std::make_unique<Node<int>>(3));
    root->set_height_val(2);

    Node<int> copy = *root;

    EXPECT_NE(root.get(), &copy);
    EXPECT_NE(root->get_left(), copy.get_left()) << "Children must have distinct memory addresses";

    EXPECT_EQ(copy.get_data(), 5);
    ASSERT_NE(copy.get_left(), nullptr);
    EXPECT_EQ(copy.get_left()->get_data(), 3);

    EXPECT_EQ(copy.get_height_val(), 2) << "Height must be preserved during copy";
}

TEST(NodeTest, AssignmentOperatorDeepCopy)
{
    Node<int> original(10);
    original.set_right(std::make_unique<Node<int>>(20));
    original.set_height_val(10);

    Node<int> copy(999);
    copy = original;

    EXPECT_NE(original.get_right(), copy.get_right());
    EXPECT_EQ(copy.get_data(), 10);
    EXPECT_EQ(copy.get_right()->get_data(), 20);
    EXPECT_EQ(copy.get_height_val(), 10) << "Height must be preserved during assignment";
}

TEST(NodeTest, DetachLeft)
{
    auto node = std::make_unique<Node<int>>(50);
    node->set_left(std::make_unique<Node<int>>(25));

    std::unique_ptr<Node<int>> detached = node->detach_left();

    EXPECT_EQ(node->get_left(), nullptr) << "Parent should no longer hold the child";

    ASSERT_NE(detached, nullptr) << "Detached pointer should be valid";
    EXPECT_EQ(detached->get_data(), 25);
}

TEST(NodeTest, DetachRight)
{
    auto node = std::make_unique<Node<int>>(50);
    node->set_right(std::make_unique<Node<int>>(75));

    std::unique_ptr<Node<int>> detached = node->detach_right();

    EXPECT_EQ(node->get_right(), nullptr) << "Parent should no longer hold the child";

    ASSERT_NE(detached, nullptr);
    EXPECT_EQ(detached->get_data(), 75);
}

TEST(NodeTest, SetChildrenOverwrite)
{
    auto node = std::make_unique<Node<int>>(1);
    node->set_left(std::make_unique<Node<int>>(2));

    node->set_left(std::make_unique<Node<int>>(3));

    EXPECT_EQ(node->get_left()->get_data(), 3);
}

TEST(NodeTest, MoveSemantics)
{
    auto node = std::make_unique<Node<int>>(10);
    node->set_height_val(4);

    Node<int> moved_node = std::move(*node);

    EXPECT_EQ(moved_node.get_data(), 10);
    EXPECT_EQ(moved_node.get_height_val(), 4);
}