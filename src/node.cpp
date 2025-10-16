#include "header.h"

namespace stl_ext
{

    template <typename T>
    const T& Node<T>::get_data() const
    {
        return m_data;
    }

    template <typename T>
    void Node<T>::set_data(const T &val)
    {
        m_data = val;
    }

    template <typename T>
    Node<T> *Node<T>::get_left() const
    {
        return p_left.get();
    }

    template <typename T>
    void Node<T>::set_left(std::unique_ptr<Node<T>> node)
    {
        p_left = std::move(node);
    }

    template <typename T>
    Node<T> *Node<T>::get_right() const
    {
        return p_right.get();
    }

    template <typename T>
    void Node<T>::set_right(std::unique_ptr<Node<T>> node)
    {
        p_right = std::move(node);
    }

} // namespace stl_ext

template class stl_ext::Node<int>;
template class stl_ext::Node<double>;
template class stl_ext::Node<float>;
template class stl_ext::Node<char>;
template class stl_ext::Node<long>;
