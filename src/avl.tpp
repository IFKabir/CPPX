#include <algorithm>

namespace stl_ext
{

template <typename T> int AVLTree<T>::get_height(const Node<T> *node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    return node->get_height_val();
}

template <typename T> int AVLTree<T>::get_balance_factor(const Node<T> *node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    return get_height(node->get_left()) - get_height(node->get_right());
}

template <typename T> void AVLTree<T>::update_height(Node<T> *node)
{
    if (node == nullptr)
    {
        return;
    }

    int left_h = get_height(node->get_left());
    int right_h = get_height(node->get_right());

    node->set_height_val(1 + std::max(left_h, right_h));
}

template <typename T> std::unique_ptr<Node<T>> AVLTree<T>::rotate_left(std::unique_ptr<Node<T>> node)
{
    std::unique_ptr<Node<T>> new_root = node->detach_right();

    node->set_right(new_root->detach_left());

    new_root->set_left(std::move(node));

    update_height(new_root->get_left());
    update_height(new_root.get());

    return new_root;
}

template <typename T> std::unique_ptr<Node<T>> AVLTree<T>::rotate_right(std::unique_ptr<Node<T>> node)
{
    std::unique_ptr<Node<T>> new_root = node->detach_left();

    node->set_left(new_root->detach_right());

    new_root->set_right(std::move(node));

    update_height(new_root->get_right());
    update_height(new_root.get());

    return new_root;
}

template <typename T> std::unique_ptr<Node<T>> AVLTree<T>::rebalance(std::unique_ptr<Node<T>> node)
{
    update_height(node.get());

    int balance = get_balance_factor(node.get());

    if (balance > 1)
    {
        if (get_balance_factor(node->get_left()) < 0)
        {
            node->set_left(rotate_left(node->detach_left()));
        }
        return rotate_right(std::move(node));
    }

    if (balance < -1)
    {
        if (get_balance_factor(node->get_right()) > 0)
        {
            node->set_right(rotate_right(node->detach_right()));
        }
        return rotate_left(std::move(node));
    }

    return node;
}

template <typename T> void AVLTree<T>::insert(const T &val)
{
    this->p_head = insert_helper(std::move(this->p_head), val);
}

template <typename T> std::unique_ptr<Node<T>> AVLTree<T>::insert_helper(std::unique_ptr<Node<T>> node, const T &val)
{
    if (node == nullptr)
    {
        return BinaryTree<T>::make_node(val);
    }

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
        return node;
    }

    return rebalance(std::move(node));
}

template <typename T> void AVLTree<T>::remove(const T &val)
{
    this->p_head = remove_helper(std::move(this->p_head), val);
}

template <typename T> std::unique_ptr<Node<T>> AVLTree<T>::remove_helper(std::unique_ptr<Node<T>> node, const T &val)
{
    if (node == nullptr)
    {
        return nullptr;
    }

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
        if (node->get_left() == nullptr)
        {
            return node->detach_right();
        }
        else if (node->get_right() == nullptr)
        {
            return node->detach_left();
        }
        Node<T> *temp = node->get_right();
        while (temp->get_left() != nullptr)
        {
            temp = temp->get_left();
        }

        node->set_data(temp->get_data());

        node->set_right(remove_helper(node->detach_right(), temp->get_data()));
    }

    return rebalance(std::move(node));
}

} // namespace stl_ext