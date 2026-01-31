#include <iostream>
#include "../include/cppx.h" // This includes AVLTree, BST, Node, etc.

int main() {
    std::cout << "=== AVL Tree Test Start ===\n" << std::endl;

    // 1. Instantiate the AVL Tree
    stl_ext::AVLTree<int> tree;

    // 2. Test Right-Right Case (Left Rotation)
    // In a normal BST: 10 -> 20 -> 30 (Skewed Line)
    // In AVL: Should rotate so 20 is root, 10 is left, 30 is right.
    std::cout << "[Test 1] Inserting 10, 20, 30..." << std::endl;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);

    // Check Root
    if (tree.get_root()->get_data() == 20) {
        std::cout << "SUCCESS: Root is 20 (Tree rotated correctly)." << std::endl;
    } else {
        std::cout << "FAILURE: Root is " << tree.get_root()->get_data() << " (Expected 20)." << std::endl;
    }
    
    std::cout << "Level Order: ";
    tree.print_levelorder(); // Should print: 20 10 30
    std::cout << "\n--------------------------------\n";

    // 3. Test Left-Left Case (Right Rotation)
    // Inserting 5, 4 should cause imbalance at 10.
    std::cout << "[Test 2] Inserting 5, 4..." << std::endl;
    tree.insert(5);
    tree.insert(4);

    std::cout << "Level Order: ";
    tree.print_levelorder(); 
    // Tree should look like:
    //       20
    //      /  \
    //     5    30
    //    / \
    //   4   10
    std::cout << "\n--------------------------------\n";

    // 4. Test Removal (Rebalancing)
    // Removing 30 should make the right side light, potentially forcing rotation?
    std::cout << "[Test 3] Removing 30..." << std::endl;
    tree.remove(30);

    std::cout << "Root is now: " << tree.get_root()->get_data() << std::endl;
    std::cout << "Level Order: ";
    tree.print_levelorder();
    std::cout << "\n--------------------------------\n";

    // 5. Large Scale Insert (Verify Height)
    std::cout << "[Test 4] Inserting values 40 to 100..." << std::endl;
    for (int i = 40; i <= 100; i += 10) {
        tree.insert(i);
    }
    
    // In a normal BST, height would be huge. In AVL, it should be small (log n).
    // Note: You might need to add a public get_height() to AVLTree for this check, 
    // or just rely on the fact that it prints quickly.
    std::cout << "Final Tree Level Order: ";
    tree.print_levelorder();
    
    std::cout << "\n\n=== Test Complete ===" << std::endl;

    return 0;
}