#include "header.h"

namespace stl_ext
{
    template <typename T>
    void BST<T>::insert(const T &val)
    {
        if (!this->p_head)
        {
            this->p_head = BinaryTree<T>::make_node(val);
            return;
        }

        insertRec(this->p_head.get(), val);
    }

    template <typename T>
    bool BST<T>::contains(const T &val) const
    {
        bool flag = false;
        check_contains(this->p_head.get(), val, flag);
        return flag;
    }

    template <typename T>
    void BST<T>::insertRec(Node<T> *node_ptr, const T &val)
    {
        if (val < node_ptr->get_data())
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

    template <typename T>
    void BST<T>::check_contains(const Node<T> *node_ptr, const T &val, bool &flag) const
    {
        if (!node_ptr || flag)
            return;

        if (node_ptr->get_data() == val)
        {
            flag = true;
            return;
        }
        if (val < node_ptr->get_data())
            check_contains(node_ptr->get_left(), val, flag);
        else
            check_contains(node_ptr->get_right(), val, flag);
    }

} // namespace stl_ext

template class stl_ext::BST<int>;
template class stl_ext::BST<double>;
template class stl_ext::BST<float>;
template class stl_ext::BST<char>;
template class stl_ext::BST<long>;