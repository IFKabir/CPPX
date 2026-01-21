#include "header.h"

namespace stl_ext
{
    template <typename T>
    void BST<T>::insert(const T &val)
    {
        insertRec(this->p_head, val);
    }

    template <typename T>
    void BST<T>::insertRec(std::unique_ptr<Node<T>> &node_ptr, const T &val)
    {
        if (!node_ptr)
        {
            node_ptr = std::make_unique<Node<T>>(val);
            return;
        }

        if (val < node_ptr->get_data())
        {
            insertRec(node_ptr->p_left, val);
        }
        else if (val > node_ptr->get_data())
        {
            insertRec(node_ptr->p_right, val);
        }
    }
}