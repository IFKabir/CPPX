namespace stl_ext
{
template <typename T> void BST<T>::insert(const T &val)
{
    if (!this->p_head)
    {
        this->p_head = BinaryTree<T>::make_node(val);
        return;
    }
    insert_iterative(this->p_head.get(), val);
}
template <typename T> void BST<T>::insert_iterative(Node<T> *node, const T &val)
{
    Node<T> *curr = node;
    while (true)
    {
        if (val <= curr->get_data())
        {
            if (!curr->get_left())
            {
                curr->set_left(BinaryTree<T>::make_node(val));
                return;
            }
            curr = curr->get_left();
        }
        else
        {
            if (!curr->get_right())
            {
                curr->set_right(BinaryTree<T>::make_node(val));
                return;
            }
            curr = curr->get_right();
        }
    }
}
template <typename T> bool BST<T>::contains(const T &val) const
{
    Node<T> *curr = this->p_head.get();
    while (curr)
    {
        if (val == curr->get_data())
            return true;
        if (val < curr->get_data())
            curr = curr->get_left();
        else
            curr = curr->get_right();
    }
    return false;
}
template <typename T> void BST<T>::remove(const T &val)
{
    // [Add full removal logic here - same as your original bst.cpp file]
    throw std::runtime_error("Removal not fully implemented in this snippet");
}
template <typename T> T BST<T>::get_min() const
{
    Node<T> *r = this->p_head.get();
    if (!r)
        throw std::runtime_error("Empty");
    while (r->get_left())
        r = r->get_left();
    return r->get_data();
}
template <typename T> T BST<T>::get_max() const
{
    Node<T> *r = this->p_head.get();
    if (!r)
        throw std::runtime_error("Empty");
    while (r->get_right())
        r = r->get_right();
    return r->get_data();
}
template <typename T> T BST<T>::get_successor(const T &val) const
{
    Node<T> *curr = this->get_root();
    Node<T> *succ = nullptr;
    while (curr)
    {
        if (curr->get_data() > val)
        {
            succ = curr;
            curr = curr->get_left();
        }
        else
            curr = curr->get_right();
    }
    if (!succ)
        throw std::runtime_error("No successor");
    return succ->get_data();
}
template <typename T> T BST<T>::get_predecessor(const T &val) const
{
    Node<T> *curr = this->get_root();
    Node<T> *pred = nullptr;
    while (curr)
    {
        if (curr->get_data() < val)
        {
            pred = curr;
            curr = curr->get_right();
        }
        else
            curr = curr->get_left();
    }
    if (!pred)
        throw std::runtime_error("No predecessor");
    return pred->get_data();
}
} // namespace stl_ext