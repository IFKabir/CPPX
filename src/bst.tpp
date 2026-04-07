#pragma once
#include "cppx/cppx.h"

namespace stl_ext
{
template <typename T> void BST<T>::insert(const T &val)
{
    if (!this->p_head)
    {
        this->p_head = this->make_node(val);
        return;
    }
    insert_iterative(this->p_head, val);
}

template <typename T> void BST<T>::insert_iterative(Node<T> *node, const T &val)
{
    Node<T> *curr = node;

    while (true)
    {
        if (val <= curr->m_data)
        {
            if (!curr->p_left)
            {
                curr->p_left = this->make_node(val);
                break;
            }
            curr = curr->p_left;
        }
        else
        {
            if (!curr->p_right)
            {
                curr->p_right = this->make_node(val);
                break;
            }
            curr = curr->p_right;
        }
    }
}

template <typename T> void BST<T>::remove_impl(const T &val)
{
    struct Frame
    {
        Node<T> **link;
        Node<T> *node;
    };

    Node<T> **link = &this->p_head;
    Node<T> *node = this->p_head;

    while (node)
    {
        if (val < node->m_data)
        {
            link = &node->p_left;
            node = node->p_left;
        }
        else if (val > node->m_data)
        {
            link = &node->p_right;
            node = node->p_right;
        }
        else
        {
            if (!node->p_left)
            {
                *link = node->p_right;
                this->m_pool.deallocate(node);
                return;
            }
            if (!node->p_right)
            {
                *link = node->p_left;
                this->m_pool.deallocate(node);
                return;
            }

            Node<T> *succ = node->p_right;
            Node<T> **succ_link = &node->p_right;
            while (succ->p_left)
            {
                succ_link = &succ->p_left;
                succ = succ->p_left;
            }

            node->m_data = succ->m_data;
            *succ_link = succ->p_right;
            this->m_pool.deallocate(succ);
            return;
        }
    }
}

template <typename T> bool BST<T>::contains(const T &val) const
{
    const Node<T> *curr = this->p_head;
    while (curr)
    {
        if (val == curr->m_data)
            return true;
        curr = (val < curr->m_data) ? curr->p_left : curr->p_right;
    }
    return false;
}

template <typename T> void BST<T>::remove(const T &val)
{
    remove_impl(val);
}

template <typename T> T BST<T>::get_min() const
{
    Node<T> *r = this->p_head;
    if (!r)
        throw std::runtime_error("Empty");
    while (r->p_left)
        r = r->p_left;
    return r->m_data;
}

template <typename T> T BST<T>::get_max() const
{
    Node<T> *r = this->p_head;
    if (!r)
        throw std::runtime_error("Empty");
    while (r->p_right)
        r = r->p_right;
    return r->m_data;
}

template <typename T> T BST<T>::get_successor(const T &val) const
{
    const Node<T> *curr = this->p_head;
    const Node<T> *ancestor_succ = nullptr;

    while (curr)
    {
        if (val < curr->m_data)
        {
            ancestor_succ = curr;
            curr = curr->p_left;
        }
        else if (val > curr->m_data)
        {
            curr = curr->p_right;
        }
        else
        {
            if (curr->p_right)
            {
                const Node<T> *temp = curr->p_right;
                while (temp->p_left)
                    temp = temp->p_left;
                return temp->m_data;
            }

            if (ancestor_succ)
                return ancestor_succ->m_data;

            throw std::runtime_error("No successor");
        }
    }

    throw std::runtime_error("Value not found");
}

template <typename T> T BST<T>::get_predecessor(const T &val) const
{
    const Node<T> *curr = this->p_head;
    const Node<T> *ancestor_pred = nullptr;

    while (curr)
    {
        if (val < curr->m_data)
        {
            curr = curr->p_left;
        }
        else if (val > curr->m_data)
        {
            ancestor_pred = curr;
            curr = curr->p_right;
        }
        else
        {
            if (curr->p_left)
            {
                const Node<T> *temp = curr->p_left;
                while (temp->p_right)
                    temp = temp->p_right;
                return temp->m_data;
            }

            if (ancestor_pred)
                return ancestor_pred->m_data;

            throw std::runtime_error("No predecessor");
        }
    }
    throw std::runtime_error("Value not found");
}

} // namespace stl_ext