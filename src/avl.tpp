#pragma once
#include "cppx/cppx.h"

namespace stl_ext
{

template <typename T> int AVLTree<T>::get_height(const Node<T> *node) const
{
    return node ? node->m_height : 0;
}

template <typename T> int AVLTree<T>::get_balance_factor(const Node<T> *node) const
{
    return node ? get_height(node->p_left) - get_height(node->p_right) : 0;
}

template <typename T> void AVLTree<T>::update_height(Node<T> *node)
{
    if (!node)
        return;
    int left_h = get_height(node->p_left);
    int right_h = get_height(node->p_right);
    node->m_height = static_cast<std::int8_t>(1 + std::max(left_h, right_h));
}

template <typename T> Node<T> *AVLTree<T>::rotate_left(Node<T> *x)
{
    Node<T> *y = x->p_right;
    x->p_right = y->p_left;
    y->p_left = x;
    update_height(x);
    update_height(y);
    return y;
}

template <typename T> Node<T> *AVLTree<T>::rotate_right(Node<T> *x)
{
    Node<T> *y = x->p_left;
    x->p_left = y->p_right;
    y->p_right = x;
    update_height(x);
    update_height(y);
    return y;
}

template <typename T> Node<T> *AVLTree<T>::rebalance(Node<T> *node)
{
    update_height(node);
    int balance = get_balance_factor(node);

    if (balance > 1)
    {
        if (get_balance_factor(node->p_left) < 0)
            node->p_left = rotate_left(node->p_left);
        return rotate_right(node);
    }

    if (balance < -1)
    {
        if (get_balance_factor(node->p_right) > 0)
            node->p_right = rotate_right(node->p_right);
        return rotate_left(node);
    }

    return node;
}

template <typename T> void AVLTree<T>::insert(const T &val)
{
    if (!this->p_head)
    {
        this->p_head = this->make_node(val);
        return;
    }

    std::vector<Node<T> **> path;
    Node<T> **link = &this->p_head;

    while (*link)
    {
        path.push_back(link);
        if (val < (*link)->m_data)
            link = &(*link)->p_left;
        else if (val > (*link)->m_data)
            link = &(*link)->p_right;
        else
            return;
    }

    *link = this->make_node(val);

    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        *(*it) = rebalance(*(*it));
    }
}

template <typename T> void AVLTree<T>::remove(const T &val)
{
    struct Frame
    {
        Node<T> **link;
    };

    std::vector<Node<T> **> path;
    Node<T> **link = &this->p_head;

    while (*link)
    {
        if (val < (*link)->m_data)
        {
            path.push_back(link);
            link = &(*link)->p_left;
        }
        else if (val > (*link)->m_data)
        {
            path.push_back(link);
            link = &(*link)->p_right;
        }
        else
        {
            Node<T> *node = *link;
            if (!node->p_left)
            {
                *link = node->p_right;
                this->m_pool.deallocate(node);
            }
            else if (!node->p_right)
            {
                *link = node->p_left;
                this->m_pool.deallocate(node);
            }
            else
            {
                Node<T> **succ_link = &node->p_right;
                std::vector<Node<T> **> succ_path;
                while ((*succ_link)->p_left)
                {
                    succ_path.push_back(succ_link);
                    succ_link = &(*succ_link)->p_left;
                }

                Node<T> *succ = *succ_link;
                node->m_data = succ->m_data;
                *succ_link = succ->p_right;
                this->m_pool.deallocate(succ);

                for (auto it = succ_path.rbegin(); it != succ_path.rend(); ++it)
                {
                    *(*it) = rebalance(*(*it));
                }
                *link = rebalance(node);
            }

            for (auto it = path.rbegin(); it != path.rend(); ++it)
            {
                *(*it) = rebalance(*(*it));
            }
            return;
        }
    }
}

} // namespace stl_ext