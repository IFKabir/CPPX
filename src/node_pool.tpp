#pragma once
#include "cppx/cppx.h"

namespace stl_ext
{

template <typename T> NodePool<T>::~NodePool()
{
    destroy_all();
}

template <typename T>
NodePool<T>::NodePool(NodePool &&other) noexcept
    : m_blocks(std::move(other.m_blocks)), m_free_list(std::move(other.m_free_list))
{
    other.m_blocks.clear();
    other.m_free_list.clear();
}

template <typename T> NodePool<T> &NodePool<T>::operator=(NodePool &&other) noexcept
{
    if (this != &other)
    {
        destroy_all();
        m_blocks = std::move(other.m_blocks);
        m_free_list = std::move(other.m_free_list);
        other.m_blocks.clear();
        other.m_free_list.clear();
    }
    return *this;
}

template <typename T> template <typename... Args> Node<T> *NodePool<T>::allocate(Args &&...args)
{
    void *ptr = get_slot();
    return new (ptr) Node<T>(std::forward<Args>(args)...);
}

template <typename T> void NodePool<T>::deallocate(Node<T> *node)
{
    if (!node)
        return;
    node->~Node();
    m_free_list.push_back(static_cast<void *>(node));
}

template <typename T> void NodePool<T>::destroy_all()
{
    for (auto *blk : m_blocks)
        delete blk;
    m_blocks.clear();
    m_free_list.clear();
}

template <typename T> void *NodePool<T>::get_slot()
{
    if (!m_free_list.empty())
    {
        void *ptr = m_free_list.back();
        m_free_list.pop_back();
        return ptr;
    }
    if (m_blocks.empty() || m_blocks.back()->used >= Block::CAPACITY)
    {
        m_blocks.push_back(new Block());
    }
    auto &blk = *m_blocks.back();
    void *ptr = blk.data + blk.used * sizeof(Node<T>);
    ++blk.used;
    return ptr;
}

} // namespace stl_ext
