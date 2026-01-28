#include <gtest/gtest.h>

#include <algorithm> // for std::is_sorted
#include <stdexcept> // for exceptions
#include <vector>

#include "cppx.h"

using namespace std;
using namespace stl_ext;

// Helper to capture inorder traversal into a vector
template <typename T> void capture_inorder(const Node<T> *node, vector<T> &result)
{
    if (node == nullptr)
        return;
    capture_inorder(node->get_left(), result);
    result.push_back(node->get_data());
    capture_inorder(node->get_right(), result);
}

// 1. Basic Insertion & Search
TEST(BSTTest, InsertAndSearch)
{
    BST<int> bst;

    // Insert values in a non-sorted order
    bst.insert(50);
    bst.insert(30);
    bst.insert(20);
    bst.insert(40);
    bst.insert(70);
    bst.insert(60);
    bst.insert(80);

    EXPECT_EQ(bst.size(), 7);

    // Search for existing values
    EXPECT_TRUE(bst.contains(50));
    EXPECT_TRUE(bst.contains(20)); // Leaf
    EXPECT_TRUE(bst.contains(80)); // Leaf

    // Search for non-existing values
    EXPECT_FALSE(bst.contains(90));
    EXPECT_FALSE(bst.contains(10));
}

// 2. verify BST Property (Inorder traversal must be sorted)
TEST(BSTTest, OrderingProperty)
{
    BST<int> bst;
    vector<int> values = {50, 25, 75, 10, 30, 60, 85};

    for (int v : values)
    {
        bst.insert(v);
    }

    // Capture inorder traversal
    vector<int> stored_values;
    capture_inorder(bst.get_root(), stored_values);

    // Check if the vector is sorted
    EXPECT_TRUE(std::is_sorted(stored_values.begin(), stored_values.end()))
        << "Inorder traversal of a BST must be sorted ascendingly.";

    // Specific check
    vector<int> expected = {10, 25, 30, 50, 60, 75, 85};
    EXPECT_EQ(stored_values, expected);
}

// 3. Test Successor Logic
TEST(BSTTest, GetSuccessor)
{
    /* Constructed Tree:
            20
           /  \
         10    30
        /  \   /  \
       5   15 25  35
    */
    BST<int> bst;
    vector<int> inputs = {20, 10, 30, 5, 15, 25, 35};
    for (int i : inputs)
        bst.insert(i);

    // Case A: Node has a right subtree -> Successor is min of right subtree
    // Successor of 20 is 25
    EXPECT_EQ(bst.get_successor(20), 25);

    // Successor of 10 is 15
    EXPECT_EQ(bst.get_successor(10), 15);

    // Case B: No right subtree -> Successor is one of the ancestors
    // Successor of 15 is 20
    EXPECT_EQ(bst.get_successor(15), 20);

    // Successor of 25 is 30
    EXPECT_EQ(bst.get_successor(25), 30);

    // Case C: No Successor (Maximum value)
    EXPECT_THROW(bst.get_successor(35), std::runtime_error);

    // Case D: Value not in tree (Optional, depending on implementation this might
    // throw or behave undefined) If your logic relies on finding the node first,
    // you might need to handle this.
}

// 4. Test Predecessor Logic
TEST(BSTTest, GetPredecessor)
{
    /* Constructed Tree:
            20
           /  \
         10    30
        /  \   /  \
       5   15 25  35
    */
    BST<int> bst;
    vector<int> inputs = {20, 10, 30, 5, 15, 25, 35};
    for (int i : inputs)
        bst.insert(i);

    // Case A: Node has left subtree -> Predecessor is max of left subtree
    // Predecessor of 20 is 15
    EXPECT_EQ(bst.get_predecessor(20), 15);

    // Predecessor of 30 is 25
    EXPECT_EQ(bst.get_predecessor(30), 25);

    // Case B: No left subtree -> Predecessor is ancestor
    // Predecessor of 25 is 20
    EXPECT_EQ(bst.get_predecessor(25), 20);

    // Predecessor of 15 is 10
    EXPECT_EQ(bst.get_predecessor(15), 10);

    // Case C: No Predecessor (Minimum value)
    EXPECT_THROW(bst.get_predecessor(5), std::runtime_error);
}

// 5. Test Iterative Insert Duplicate Handling
TEST(BSTTest, DuplicateInsertion)
{
    BST<int> bst;
    bst.insert(10);
    bst.insert(10);

    // Depending on logic, size might stay 1 (if strict set) or go to 2 (multiset)
    // Assuming standard BST (no duplicates):
    // If your insert logic allows duplicates, change expected size to 2.
    // Based on previous code: "if (val <= node_ptr->get_data())" implies
    // duplicates go left. If duplicates are allowed and go left:
    EXPECT_EQ(bst.size(), 2);
    EXPECT_EQ(bst.get_root()->get_left()->get_data(), 10);
}