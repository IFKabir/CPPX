#include <cstdio>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "cppx/cppx.h"

using namespace std;
using namespace stl_ext;

static std::string capture_print_tree(const BinaryTree<int> &tree)
{
    std::ostringstream oss;
    std::streambuf *old = std::cout.rdbuf(oss.rdbuf());
    tree.print_tree();
    std::cout.rdbuf(old);
    return oss.str();
}

static std::string read_file_contents(const std::string &path)
{
    std::ifstream ifs(path);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    return oss.str();
}

TEST(VisualizationTest, PrintTreeEmptyTree)
{
    BinaryTree<int> tree;
    std::string out = capture_print_tree(tree);
    EXPECT_NE(out.find("(empty tree)"), std::string::npos);
}

TEST(VisualizationTest, PrintTreeSingleNode)
{
    BinaryTree<int> tree;
    tree.set_root(tree.make_node(42));

    std::string out = capture_print_tree(tree);
    EXPECT_NE(out.find("42"), std::string::npos);
}

TEST(VisualizationTest, PrintTreeMultipleNodes)
{
    BinaryTree<int> tree;
    tree.set_root(tree.make_node(10));
    tree.set_left(tree.get_root(), tree.make_node(5));
    tree.set_right(tree.get_root(), tree.make_node(15));

    std::string out = capture_print_tree(tree);

    EXPECT_NE(out.find("10"), std::string::npos);
    EXPECT_NE(out.find("5"), std::string::npos);
    EXPECT_NE(out.find("15"), std::string::npos);

    EXPECT_NE(out.find("\u2514"), std::string::npos);
    EXPECT_NE(out.find("\u250c"), std::string::npos);
}

TEST(VisualizationTest, PrintTreeAVLAfterRotation)
{
    AVLTree<int> tree;
    tree.insert(30);
    tree.insert(20);
    tree.insert(10);

    std::string out = capture_print_tree(tree);

    EXPECT_NE(out.find("20"), std::string::npos);
    EXPECT_NE(out.find("10"), std::string::npos);
    EXPECT_NE(out.find("30"), std::string::npos);
}

TEST(VisualizationTest, DumpToDotEmptyTree)
{
    BinaryTree<int> tree;
    std::string path = "/tmp/cppx_test_empty.dot";

    EXPECT_NO_THROW(tree.dump_to_dot(path));

    std::string content = read_file_contents(path);
    EXPECT_NE(content.find("digraph BST"), std::string::npos);
    EXPECT_NE(content.find("}"), std::string::npos);

    std::remove(path.c_str());
}

TEST(VisualizationTest, DumpToDotSingleNode)
{
    BinaryTree<int> tree;
    tree.set_root(tree.make_node(42));
    std::string path = "/tmp/cppx_test_single.dot";

    tree.dump_to_dot(path);

    std::string content = read_file_contents(path);
    EXPECT_NE(content.find("\"42\""), std::string::npos);

    std::remove(path.c_str());
}

TEST(VisualizationTest, DumpToDotMultipleNodes)
{
    AVLTree<int> tree;
    tree.insert(20);
    tree.insert(10);
    tree.insert(30);
    std::string path = "/tmp/cppx_test_multi.dot";

    tree.dump_to_dot(path);

    std::string content = read_file_contents(path);
    EXPECT_NE(content.find("\"20\" -> \"10\""), std::string::npos);
    EXPECT_NE(content.find("\"20\" -> \"30\""), std::string::npos);

    std::remove(path.c_str());
}

TEST(VisualizationTest, DumpToDotInvalidPath)
{
    BinaryTree<int> tree;
    tree.set_root(tree.make_node(1));

    EXPECT_THROW(tree.dump_to_dot("/nonexistent_dir/file.dot"), std::runtime_error);
}
