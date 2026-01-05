#include "header.h"
#include <iostream>
#include <cassert>
#include <string>
#include <vector>

using namespace std;
using namespace stl_ext;

void print_divider(const string &title)
{
    cout << "---" << endl;
    cout << "TESTING: " << title << endl;
    cout << "---" << endl;
}

void test1()
{
    print_divider("1");

    BinaryTree<int> tree1;
    tree1.set_root(BinaryTree<int>::make_node(10));

    BinaryTree<int> tree2(std::move(tree1));
    assert(tree1.is_empty() && "Tree1 should be empty after move");
    assert(tree2.size() == 1 && "Tree2 should have inherited the node");

    BinaryTree<int> tree3;
    tree3.set_root(BinaryTree<int>::make_node(50));
    tree3 = std::move(tree2);
    assert(tree2.is_empty() && "Tree2 should be empty after move assignment");
    assert(tree3.get_root()->get_data() == 10 && "Tree3 should now hold moved data");

    cout << "✓ test 1 passed." << endl;
}

void test2()
{
    print_divider("2");

    BinaryTree<int> deep_tree;
    deep_tree.set_root(BinaryTree<int>::make_node(0));
    Node<int> *current = deep_tree.get_root();

    for (int i = 1; i < 100; ++i)
    {
        deep_tree.set_left(current, BinaryTree<int>::make_node(i));
        current = current->get_left();
    }

    assert(deep_tree.size() == 100);

    assert(current->get_data() == 99);

    BinaryTree<int> deep_copy = deep_tree;
    assert(deep_copy.size() == 100);
    assert(deep_copy.get_root() != deep_tree.get_root());

    cout << "✓ test 2 passed." << endl;
}

void test3()
{
    print_divider("3");

    BinaryTree<int> tree;
    tree.set_root(BinaryTree<int>::make_node(1));
    auto r = tree.get_root();

    tree.set_left(r, BinaryTree<int>::make_node(2));
    tree.set_left(r->get_left(), BinaryTree<int>::make_node(3));
    tree.set_right(r->get_left(), BinaryTree<int>::make_node(4));

    assert(tree.size() == 4);

    tree.set_left(r, BinaryTree<int>::make_node(10));

    assert(tree.size() == 2);
    assert(r->get_left()->get_data() == 10);
    assert(r->get_left()->get_left() == nullptr);

    cout << "✓ test 3 passed." << endl;
}

void test4()
{
    print_divider("4");

    BinaryTree<float> empty_f;

    assert(empty_f.size() == 0);
    assert(empty_f.is_empty() == true);

    BinaryTree<float> empty_copy(empty_f);
    assert(empty_copy.is_empty());

    cout << "✓ test 4 passed." << endl;
}

void test5()
{
    print_divider("5");

    auto left_child = BinaryTree<int>::make_node(20);
    auto right_child = BinaryTree<int>::make_node(30);
    auto parent = BinaryTree<int>::make_node(10, std::move(left_child), std::move(right_child));

    BinaryTree<int> tree(std::move(parent));

    assert(tree.size() == 3);
    assert(tree.get_root()->get_left()->get_data() == 20);
    assert(tree.get_root()->get_right()->get_data() == 30);

    cout << "✓ test 5 passed." << endl;
}

int main()
{
    cout << "\n====" << endl;
    cout << "STRESS TESTS STARTING..." << endl;
    cout << "===" << endl;

    test1();
    test2();
    test3();
    test4();
    test5();

    cout << "\n===" << endl;
    cout << "STRESS TESTS COMPLETED SUCCESSFULLY" << endl;
    cout << "===" << endl;

    return 0;
}