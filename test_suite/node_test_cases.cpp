#include <gtest/gtest.h>
#include <string>
#include <utility>

#include "cppx/cppx.h"

using namespace stl_ext;
using namespace std;

TEST(NodeTest, ConstructorAndData)
{
    Node<int> node(42);

    EXPECT_EQ(node.get_data(), 42);
    EXPECT_EQ(node.get_left(), nullptr);
    EXPECT_EQ(node.get_right(), nullptr);
    EXPECT_EQ(node.get_height_val(), 1) << "Default height should be 1";
}

TEST(NodeTest, ConstructorWithChildren)
{
    Node<int> left(10);
    Node<int> right(20);

    Node<int> root(30, &left, &right);

    EXPECT_EQ(root.get_data(), 30);
    ASSERT_NE(root.get_left(), nullptr);
    ASSERT_NE(root.get_right(), nullptr);

    EXPECT_EQ(root.get_left()->get_data(), 10);
    EXPECT_EQ(root.get_right()->get_data(), 20);
}

TEST(NodeTest, ConstructorWithNullChildren)
{
    Node<int> root(100, nullptr, nullptr);
    EXPECT_EQ(root.get_left(), nullptr);
    EXPECT_EQ(root.get_right(), nullptr);
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
    Node<int> node(1);
    Node<int> child2(2);
    Node<int> child3(3);

    node.set_left(&child2);
    ASSERT_NE(node.get_left(), nullptr);
    EXPECT_EQ(node.get_left()->get_data(), 2);

    node.set_left(&child3);
    EXPECT_EQ(node.get_left()->get_data(), 3);
}

TEST(NodeTest, SetChildrenToNull)
{
    Node<int> node(1);
    Node<int> child(99);
    node.set_right(&child);
    ASSERT_NE(node.get_right(), nullptr);

    node.set_right(nullptr);
    EXPECT_EQ(node.get_right(), nullptr);
}

TEST(NodeTest, ComplexDataType)
{
    string test_str = "Hello World";
    Node<string> node(test_str);

    EXPECT_EQ(node.get_data(), "Hello World");

    node.set_data("Modified");
    EXPECT_EQ(node.get_data(), "Modified");

    Node<string> child("Left Child");
    node.set_left(&child);
    EXPECT_EQ(node.get_left()->get_data(), "Left Child");
}

TEST(NodeTest, ConstCorrectness)
{
    const Node<int> node(100);

    EXPECT_EQ(node.get_data(), 100);
    EXPECT_EQ(node.get_height_val(), 1);
    EXPECT_EQ(node.get_left(), nullptr);
    EXPECT_EQ(node.get_right(), nullptr);
}

TEST(NodeTest, ColorAccessors)
{
    Node<int> node(42);
    EXPECT_EQ(node.get_color(), Color::RED);

    node.set_color(Color::BLACK);
    EXPECT_EQ(node.get_color(), Color::BLACK);
}

TEST(NodeTest, ParentAccessors)
{
    Node<int> parent(10);
    Node<int> child(20);

    child.set_parent(&parent);
    EXPECT_EQ(child.get_parent(), &parent);

    child.set_parent(nullptr);
    EXPECT_EQ(child.get_parent(), nullptr);
}