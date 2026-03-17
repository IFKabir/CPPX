#pragma once
#include "../include/cppx.h"

namespace stl_ext
{

template <typename T> Node<T>::Node(T val) : m_data(std::move(val))
{
}

template <typename T>
Node<T>::Node(T val, Node<T> *left, Node<T> *right) : m_data(std::move(val)), p_left(left), p_right(right)
{
}

template <typename T> int Node<T>::get_height_val() const
{
    return m_height;
}

template <typename T> void Node<T>::set_height_val(int h)
{
    m_height = static_cast<std::int8_t>(h);
}

template <typename T> const T &Node<T>::get_data() const
{
    return m_data;
}

template <typename T> void Node<T>::set_data(const T &val)
{
    m_data = val;
}

template <typename T> Node<T> *Node<T>::get_left() const
{
    return p_left;
}

template <typename T> void Node<T>::set_left(Node<T> *node)
{
    p_left = node;
}

template <typename T> Node<T> *Node<T>::get_right() const
{
    return p_right;
}

template <typename T> void Node<T>::set_right(Node<T> *node)
{
    p_right = node;
}

template <typename T> Color Node<T>::get_color() const
{
    return m_color;
}

template <typename T> void Node<T>::set_color(Color c)
{
    m_color = c;
}

template <typename T> Node<T> *Node<T>::get_parent() const
{
    return p_parent;
}

template <typename T> void Node<T>::set_parent(Node<T> *parent)
{
    p_parent = parent;
}

} // namespace stl_ext