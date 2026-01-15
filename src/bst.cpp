#include "header.h"

namespace stl_ext
{
    template <typename T>
    Node<T> BST<T>::*insertRec(const T &val, Node<T> *node)
    {
        if (node == nullptr)
        {
            return new Node(val);
        }
        if (val < node->m_data)
        {
            node->set_left(std::move(insertRec(node->p_left, val)));
        }
        else if (val > node->m_data)
        {
            node->set_right(std::move(insertRec(node->p_right, val)));
        }

        return node;
    }
}