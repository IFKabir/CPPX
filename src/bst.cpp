#include "cppx.h"
#include <stdexcept>

namespace stl_ext
{
    template <typename T>
    void BST<T>::insert(const T &val)
    {
        if (!p_head)
        {
            p_head = BinaryTree<T>::make_node(val);
            return;
        }

        insertRec(p_head.get(), val);
    }

    template <typename T>
    bool BST<T>::contains(const T &val) const
    {
        Node<T> *current = p_head.get();
        while (current != nullptr)
        {
            if (val == current->get_data())
                return true;

            if (val < current->get_data())
                current = current->get_left();
            else
                current = current->get_right();
        }
        return false;
    }

    template <typename T>
    T BST<T>::get_min() const
    {
        Node<T> *root = p_head.get();
        if (!root)
            throw std::runtime_error("Cannot get_min() from an empty BST");

        while (root->get_left() != nullptr)
        {
            root = root->get_left();
        }
        return root->get_data();
    }

    template <typename T>
    T BST<T>::get_max() const
    {
        Node<T> *root = p_head.get();
        if (!root)
            throw std::runtime_error("Cannot get_max() from an empty BST");

        while (root->get_right() != nullptr)
        {
            root = root->get_right();
        }
        return root->get_data();
    }

    template <typename T>
    void BST<T>::insertRec(Node<T> *node_ptr, const T &val)
    {
        if (val <= node_ptr->get_data())
        {
            if (node_ptr->get_left() == nullptr)
            {
                node_ptr->set_left(BinaryTree<T>::make_node(val));
            }
            else
            {
                insertRec(node_ptr->get_left(), val);
            }
        }
        else if (val > node_ptr->get_data())
        {
            if (node_ptr->get_right() == nullptr)
            {
                node_ptr->set_right(BinaryTree<T>::make_node(val));
            }
            else
            {
                insertRec(node_ptr->get_right(), val);
            }
        }
    }
} // namespace stl_ext

template class stl_ext::BST<int>;
template class stl_ext::BST<double>;
template class stl_ext::BST<float>;
template class stl_ext::BST<char>;
template class stl_ext::BST<long>;