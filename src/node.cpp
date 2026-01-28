#include "cppx.h"

namespace stl_ext
{
template <typename T> Node<T>::Node(const Node &other)
{
    if (other.p_left)
        p_left = std::make_unique<Node<T>>(*other.p_left);
    if (other.p_right)
        p_right = std::make_unique<Node<T>>(*other.p_right);
}

template <typename T> Node<T> &Node<T>::operator=(const Node<T> &other)
{
    if (this == &other)
        return *this;
    m_data = other.m_data;
    p_left = other.p_left ? std::make_unique<Node<T>>(*other.p_left) : nullptr;
    p_right = other.p_right ? std::make_unique<Node<T>>(*other.p_right) : nullptr;
    return *this;
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
    return p_left.get();
}

template <typename T> void Node<T>::set_left(std::unique_ptr<Node<T>> node)
{
    p_left = std::move(node);
}

template <typename T> Node<T> *Node<T>::get_right() const
{
    return p_right.get();
}

template <typename T> void Node<T>::set_right(std::unique_ptr<Node<T>> node)
{
    p_right = std::move(node);
}

} // namespace stl_ext

template class stl_ext::Node<int>;
template class stl_ext::Node<double>;
template class stl_ext::Node<float>;
template class stl_ext::Node<char>;
template class stl_ext::Node<long>;
