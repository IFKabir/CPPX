#include <algorithm> // Required for std::max

namespace stl_ext {

template <typename T>
int AVLTree<T>::get_height(const Node<T> *node) const
{
    // If the node doesn't exist, height is 0 (or -1 depending on convention)
    if (node == nullptr)
    {
        return 0;
    }

    
    return node->get_height_val();
}



template <typename T>
int AVLTree<T>::get_balance_factor(const Node<T> *node) const
{
    // If the node is null, it is perfectly balanced
    if (node == nullptr)
    {
        return 0;
    }

    // Calculate difference: Height of Left Subtree - Height of Right Subtree
    return get_height(node->get_left()) - get_height(node->get_right());
}

template <typename T>
void AVLTree<T>::update_height(Node<T> *node)
{
    // Safety check
    if (node == nullptr)
    {
        return;
    }

    // Get the height of left and right children (get_height handles nulls automatically)
    int left_h = get_height(node->get_left());
    int right_h = get_height(node->get_right());

    // Update current node's height: 1 + maximum of the two children
    node->set_height_val(1 + std::max(left_h, right_h));
}

template <typename T>
std::unique_ptr<Node<T>> AVLTree<T>::rotate_left(std::unique_ptr<Node<T>> node)
{
    // 1. The right child becomes the new root of this subtree
    std::unique_ptr<Node<T>> new_root = node->detach_right();

    // 2. The new root's left child becomes the old root's right child
    //    (We perform the transfer: node->right = new_root->left)
    node->set_right(new_root->detach_left());

    // 3. The old root becomes the new root's left child
    //    (We perform the transfer: new_root->left = node)
    new_root->set_left(std::move(node));

    // 4. Update heights (Order matters: child first, then parent)
    //    Note: We use .get() to pass the raw pointer to our helper
    update_height(new_root->get_left()); // Update old root (now the left child)
    update_height(new_root.get());       // Update new root

    // 5. Return the new root of this subtree to the caller
    return new_root;
}

template <typename T>
std::unique_ptr<Node<T>> AVLTree<T>::rotate_right(std::unique_ptr<Node<T>> node)
{
    // 1. The left child becomes the new root of this subtree
    std::unique_ptr<Node<T>> new_root = node->detach_left();

    // 2. The new root's right child becomes the old root's left child
    //    (We perform the transfer: node->left = new_root->right)
    node->set_left(new_root->detach_right());

    // 3. The old root becomes the new root's right child
    //    (We perform the transfer: new_root->right = node)
    new_root->set_right(std::move(node));

    // 4. Update heights (Order matters: child first, then parent)
    //    Note: We access the old root via get_right() since it is now the right child
    update_height(new_root->get_right()); 
    update_height(new_root.get());       

    // 5. Return the new root of this subtree to the caller
    return new_root;
}

template <typename T>
std::unique_ptr<Node<T>> AVLTree<T>::rebalance(std::unique_ptr<Node<T>> node)
{
    // 1. Update the height of the current node first (in case children changed)
    update_height(node.get());

    // 2. Check the balance factor
    int balance = get_balance_factor(node.get());

    // Case 1: Left Heavy (Balance > 1)
    if (balance > 1)
    {
        // Check if it's Left-Right (LR) Case:
        // If the left child has a negative balance (right heavy), we need a double rotation.
        if (get_balance_factor(node->get_left()) < 0)
        {
            // Perform Left Rotation on the left child
            node->set_left(rotate_left(node->detach_left()));
        }
        // Perform Right Rotation on the current node
        return rotate_right(std::move(node));
    }

    // Case 2: Right Heavy (Balance < -1)
    if (balance < -1)
    {
        // Check if it's Right-Left (RL) Case:
        // If the right child has a positive balance (left heavy), we need a double rotation.
        if (get_balance_factor(node->get_right()) > 0)
        {
            // Perform Right Rotation on the right child
            node->set_right(rotate_right(node->detach_right()));
        }
        // Perform Left Rotation on the current node
        return rotate_left(std::move(node));
    }

    // If balanced, return the node as is
    return node;
}

template <typename T>
void AVLTree<T>::insert(const T &val)
{
    // Call the recursive helper starting at the root (p_head)
    // We move p_head in and take the (potentially new) root back out
    this->p_head = insert_helper(std::move(this->p_head), val);
}

template <typename T>
std::unique_ptr<Node<T>> AVLTree<T>::insert_helper(std::unique_ptr<Node<T>> node, const T &val)
{
    // Base Case: Found the insertion spot, create new node
    if (node == nullptr)
    {
        return BinaryTree<T>::make_node(val);
    }

    // Recursive Step: Go Left or Right
    if (val < node->get_data())
    {
        node->set_left(insert_helper(node->detach_left(), val));
    }
    else if (val > node->get_data())
    {
        node->set_right(insert_helper(node->detach_right(), val));
    }
    else
    {
        // Duplicate value: do nothing (or update count if needed), just return node
        return node; 
    }

    // CRITICAL: Rebalance this node on the way back up the recursion stack
    return rebalance(std::move(node));
}



template <typename T>
void AVLTree<T>::remove(const T &val)
{
    // Call the recursive helper starting at the root
    this->p_head = remove_helper(std::move(this->p_head), val);
}

template <typename T>
std::unique_ptr<Node<T>> AVLTree<T>::remove_helper(std::unique_ptr<Node<T>> node, const T &val)
{
    // Base Case: Item not found
    if (node == nullptr)
    {
        return nullptr;
    }

    // 1. SEARCH for the node
    if (val < node->get_data())
    {
        node->set_left(remove_helper(node->detach_left(), val));
    }
    else if (val > node->get_data())
    {
        node->set_right(remove_helper(node->detach_right(), val));
    }
    else
    {
        // 2. FOUND IT: Handle removal cases
        
        // Case A: No children or One child
        if (node->get_left() == nullptr)
        {
            return node->detach_right(); // Return right child (or null)
        }
        else if (node->get_right() == nullptr)
        {
            return node->detach_left(); // Return left child
        }

        // Case B: Two children
        // Strategy: Find successor (min value in right subtree), swap data, delete successor
        
        // Find min node in right subtree
        Node<T> *temp = node->get_right();
        while (temp->get_left() != nullptr)
        {
            temp = temp->get_left();
        }

        // Replace current data with successor's data
        node->set_data(temp->get_data());

        // Delete the successor node from the right subtree
        node->set_right(remove_helper(node->detach_right(), temp->get_data()));
    }

    // 3. REBALANCE on the way back up
    return rebalance(std::move(node));
}


} // namespace stl_ext