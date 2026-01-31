#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>

#include "cppx.h"

using namespace stl_ext;
using namespace std;

TEST(NodeTest, ConstructorAndData)
{
    auto node = make_unique<Node<int>>(42);

    EXPECT_EQ(node->get_data(), 42);
    EXPECT_EQ(node->get_left(), nullptr);
    EXPECT_EQ(node->get_right(), nullptr);
    EXPECT_EQ(node->get_height_val(), 1) << "Default height should be 1";
}

TEST(NodeTest, ConstructorWithChildren)
{
    auto left = make_unique<Node<int>>(10);
    auto right = make_unique<Node<int>>(20);

    auto root = make_unique<Node<int>>(30, move(left), move(right));

    EXPECT_EQ(root->get_data(), 30);
    ASSERT_NE(root->get_left(), nullptr);
    ASSERT_NE(root->get_right(), nullptr);

    EXPECT_EQ(root->get_left()->get_data(), 10);
    EXPECT_EQ(root->get_right()->get_data(), 20);
}

TEST(NodeTest, ConstructorWithNullChildren)
{
    auto root = make_unique<Node<int>>(100, nullptr, nullptr);
    EXPECT_EQ(root->get_left(), nullptr);
    EXPECT_EQ(root->get_right(), nullptr);
}

TEST(NodeTest, SetData)
{
    Node<int> node(10);
    EXPECT_EQ(node.get_data(), 10);

    node.set_data(50);
    EXPECT_EQ(node.get_data(), 50);
}

TEST(NodeTest, HeightAccessors)
{
    Node<int> node(100);
    EXPECT_EQ(node.get_height_val(), 1);

    node.set_height_val(5);
    EXPECT_EQ(node.get_height_val(), 5);
}

TEST(NodeTest, SetChildrenOverwrite)
{
    auto node = make_unique<Node<int>>(1);
    node->set_left(make_unique<Node<int>>(2));

    ASSERT_NE(node->get_left(), nullptr);
    EXPECT_EQ(node->get_left()->get_data(), 2);

    node->set_left(make_unique<Node<int>>(3));
    EXPECT_EQ(node->get_left()->get_data(), 3);
}

TEST(NodeTest, SetChildrenToNull)
{
    auto node = make_unique<Node<int>>(1);
    node->set_right(make_unique<Node<int>>(99));
    ASSERT_NE(node->get_right(), nullptr);

    node->set_right(nullptr);
    EXPECT_EQ(node->get_right(), nullptr);
}

TEST(NodeTest, DetachLeft)
{
    auto node = make_unique<Node<int>>(50);
    node->set_left(make_unique<Node<int>>(25));

    unique_ptr<Node<int>> detached = node->detach_left();

    EXPECT_EQ(node->get_left(), nullptr) << "Parent should no longer hold the child";
    ASSERT_NE(detached, nullptr) << "Detached pointer should be valid";
    EXPECT_EQ(detached->get_data(), 25);
}

TEST(NodeTest, DetachRight)
{
    auto node = make_unique<Node<int>>(50);
    node->set_right(make_unique<Node<int>>(75));

    unique_ptr<Node<int>> detached = node->detach_right();

    EXPECT_EQ(node->get_right(), nullptr) << "Parent should no longer hold the child";
    ASSERT_NE(detached, nullptr);
    EXPECT_EQ(detached->get_data(), 75);
}

TEST(NodeTest, DetachEmptyChildren)
{
    auto node = make_unique<Node<int>>(10);

    auto left = node->detach_left();
    auto right = node->detach_right();

    EXPECT_EQ(left, nullptr);
    EXPECT_EQ(right, nullptr);
}

TEST(NodeTest, CopyConstructorDeepCopySimple)
{
    auto root = make_unique<Node<int>>(5);
    root->set_left(make_unique<Node<int>>(3));
    root->set_height_val(2);

    Node<int> copy = *root;

    EXPECT_NE(root.get(), &copy);
    EXPECT_NE(root->get_left(), copy.get_left()) << "Children must have distinct memory addresses";

    EXPECT_EQ(copy.get_data(), 5);
    ASSERT_NE(copy.get_left(), nullptr);
    EXPECT_EQ(copy.get_left()->get_data(), 3);
    EXPECT_EQ(copy.get_height_val(), 2);
}

TEST(NodeTest, CopyConstructorDeepCopyHierarchy)
{
    auto root = make_unique<Node<int>>(1);
    root->set_left(make_unique<Node<int>>(2));
    root->get_left()->set_left(make_unique<Node<int>>(3));

    Node<int> copy = *root;

    ASSERT_NE(copy.get_left(), nullptr);
    EXPECT_EQ(copy.get_left()->get_data(), 2);
    EXPECT_NE(copy.get_left(), root->get_left());

    auto *copy_child = copy.get_left();
    auto *root_child = root->get_left();

    ASSERT_NE(copy_child->get_left(), nullptr);
    EXPECT_EQ(copy_child->get_left()->get_data(), 3);
    EXPECT_NE(copy_child->get_left(), root_child->get_left()) << "Deep copy failed at depth 2";
}

TEST(NodeTest, AssignmentOperatorDeepCopy)
{
    Node<int> original(10);
    original.set_right(make_unique<Node<int>>(20));
    original.set_height_val(10);

    Node<int> copy(999);
    copy = original;

    EXPECT_NE(&original, &copy);
    EXPECT_NE(original.get_right(), copy.get_right());

    EXPECT_EQ(copy.get_data(), 10);
    ASSERT_NE(copy.get_right(), nullptr);
    EXPECT_EQ(copy.get_right()->get_data(), 20);
    EXPECT_EQ(copy.get_height_val(), 10);
}

TEST(NodeTest, CopySelfAssignment)
{
    Node<int> node(10);
    node.set_left(make_unique<Node<int>>(5));

    node = node;

    EXPECT_EQ(node.get_data(), 10);
    ASSERT_NE(node.get_left(), nullptr);
    EXPECT_EQ(node.get_left()->get_data(), 5);
}

TEST(NodeTest, MoveConstructor)
{
    auto node = make_unique<Node<int>>(10);
    node->set_left(make_unique<Node<int>>(5));
    node->set_height_val(4);

    Node<int> moved_node = move(*node);

    EXPECT_EQ(moved_node.get_data(), 10);
    EXPECT_EQ(moved_node.get_height_val(), 4);
    ASSERT_NE(moved_node.get_left(), nullptr);
    EXPECT_EQ(moved_node.get_left()->get_data(), 5);
}

TEST(NodeTest, MoveAssignment)
{
    Node<int> source(10);
    source.set_left(make_unique<Node<int>>(5));

    Node<int> dest(99);
    dest = move(source);

    EXPECT_EQ(dest.get_data(), 10);
    ASSERT_NE(dest.get_left(), nullptr);
    EXPECT_EQ(dest.get_left()->get_data(), 5);
}

TEST(NodeTest, MoveSelfAssignment)
{
    Node<int> node(10);
    node.set_right(make_unique<Node<int>>(20));

    node = move(node);

    EXPECT_EQ(node.get_data(), 10);
    ASSERT_NE(node.get_right(), nullptr);
    EXPECT_EQ(node.get_right()->get_data(), 20);
}

TEST(NodeTest, ComplexDataType)
{
    string test_str = "Hello World";
    auto node = make_unique<Node<string>>(test_str);

    EXPECT_EQ(node->get_data(), "Hello World");

    node->set_data("Modified");
    EXPECT_EQ(node->get_data(), "Modified");

    node->set_left(make_unique<Node<string>>("Left Child"));
    EXPECT_EQ(node->get_left()->get_data(), "Left Child");
}

TEST(NodeTest, ConstCorrectness)
{
    const Node<int> node(100);

    EXPECT_EQ(node.get_data(), 100);
    EXPECT_EQ(node.get_height_val(), 1);
    EXPECT_EQ(node.get_left(), nullptr);
    EXPECT_EQ(node.get_right(), nullptr);
}