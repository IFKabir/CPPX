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

} // namespace stl_ext

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