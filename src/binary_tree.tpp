#pragma once
#include "cppx/cppx.h"

namespace stl_ext
{

template <typename T> void BinaryTree<T>::destroy_subtree(Node<T> *node)
{
    if (!node)
        return;
    std::stack<Node<T> *> s;
    s.push(node);
    while (!s.empty())
    {
        Node<T> *curr = s.top();
        s.pop();
        if (curr->p_left)
            s.push(curr->p_left);
        if (curr->p_right)
            s.push(curr->p_right);
        m_pool.deallocate(curr);
    }
}

template <typename T> Node<T> *BinaryTree<T>::clone_subtree(const Node<T> *node)
{
    if (!node)
        return nullptr;

    struct StackEntry
    {
        const Node<T> *src;
        Node<T> *dest;
        int stage;
    };

    Node<T> *new_root = m_pool.allocate(node->m_data);
    new_root->m_height = node->m_height;
    new_root->m_color = node->m_color;

    std::stack<StackEntry> s;
    s.push({node, new_root, 0});

    while (!s.empty())
    {
        auto &top = s.top();
        if (top.stage == 0)
        {
            top.stage = 1;
            if (top.src->p_left)
            {
                Node<T> *child = m_pool.allocate(top.src->p_left->m_data);
                child->m_height = top.src->p_left->m_height;
                child->m_color = top.src->p_left->m_color;
                child->p_parent = top.dest;
                top.dest->p_left = child;
                s.push({top.src->p_left, child, 0});
            }
        }
        else if (top.stage == 1)
        {
            top.stage = 2;
            if (top.src->p_right)
            {
                Node<T> *child = m_pool.allocate(top.src->p_right->m_data);
                child->m_height = top.src->p_right->m_height;
                child->m_color = top.src->p_right->m_color;
                child->p_parent = top.dest;
                top.dest->p_right = child;
                s.push({top.src->p_right, child, 0});
            }
        }
        else
        {
            s.pop();
        }
    }

    return new_root;
}

template <typename T> BinaryTree<T>::BinaryTree(const BinaryTree &other) : p_head(nullptr)
{
    p_head = clone_subtree(other.p_head);
}

template <typename T> BinaryTree<T> &BinaryTree<T>::operator=(const BinaryTree<T> &other)
{
    if (this == &other)
        return *this;
    destroy_subtree(p_head);
    p_head = nullptr;
    m_pool.destroy_all();
    p_head = clone_subtree(other.p_head);
    return *this;
}

template <typename T>
BinaryTree<T>::BinaryTree(BinaryTree &&other) noexcept : p_head(other.p_head), m_pool(std::move(other.m_pool))
{
    other.p_head = nullptr;
}

template <typename T> BinaryTree<T> &BinaryTree<T>::operator=(BinaryTree &&other) noexcept
{
    if (this == &other)
        return *this;
    destroy_subtree(p_head);
    p_head = other.p_head;
    m_pool = std::move(other.m_pool);
    other.p_head = nullptr;
    return *this;
}

template <typename T> BinaryTree<T>::~BinaryTree()
{
    p_head = nullptr;
}

template <typename T> void BinaryTree<T>::preorder(const Node<T> *node) const
{
    if (!node)
        return;
    std::stack<const Node<T> *> s;
    s.push(node);
    while (!s.empty())
    {
        const Node<T> *curr = s.top();
        s.pop();
        std::cout << curr->m_data << " ";
        if (curr->p_right)
            s.push(curr->p_right);
        if (curr->p_left)
            s.push(curr->p_left);
    }
}

template <typename T> void BinaryTree<T>::inorder(const Node<T> *node) const
{
    std::stack<const Node<T> *> s;
    const Node<T> *curr = node;
    while (curr || !s.empty())
    {
        while (curr)
        {
            s.push(curr);
            curr = curr->p_left;
        }
        curr = s.top();
        s.pop();
        std::cout << curr->m_data << " ";
        curr = curr->p_right;
    }
}

template <typename T> void BinaryTree<T>::postorder(const Node<T> *node) const
{
    if (!node)
        return;
    std::stack<const Node<T> *> s;
    const Node<T> *curr = node;
    const Node<T> *last = nullptr;
    while (!s.empty() || curr)
    {
        if (curr)
        {
            s.push(curr);
            curr = curr->p_left;
        }
        else
        {
            const Node<T> *peek = s.top();
            if (peek->p_right && last != peek->p_right)
                curr = peek->p_right;
            else
            {
                std::cout << peek->m_data << " ";
                last = peek;
                s.pop();
            }
        }
    }
}

template <typename T> void BinaryTree<T>::levelorder(const Node<T> *node) const
{
    if (!node)
        return;
    std::queue<const Node<T> *> q;
    q.push(node);
    while (!q.empty())
    {
        const Node<T> *cur = q.front();
        q.pop();
        std::cout << cur->m_data << " ";
        if (cur->p_left)
            q.push(cur->p_left);
        if (cur->p_right)
            q.push(cur->p_right);
    }
}

template <typename T> int BinaryTree<T>::compute_size(const Node<T> *node) const
{
    if (!node)
        return 0;
    int c = 0;
    std::queue<const Node<T> *> q;
    q.push(node);
    while (!q.empty())
    {
        const Node<T> *curr = q.front();
        q.pop();
        c++;
        if (curr->p_left)
            q.push(curr->p_left);
        if (curr->p_right)
            q.push(curr->p_right);
    }
    return c;
}

template <typename T> Node<T> *BinaryTree<T>::get_root() const
{
    return p_head;
}

template <typename T> void BinaryTree<T>::set_root(Node<T> *root)
{
    if (p_head && root != p_head)
    {
        destroy_subtree(p_head);
    }
    p_head = root;
}

template <typename T> bool BinaryTree<T>::is_empty() const
{
    return p_head == nullptr;
}

template <typename T> int BinaryTree<T>::size() const
{
    return compute_size(p_head);
}

template <typename T> void BinaryTree<T>::set_left(Node<T> *p, Node<T> *l)
{
    if (p)
    {
        if (p->p_left)
            destroy_subtree(p->p_left);
        p->p_left = l;

        int h_left = p->p_left ? p->p_left->m_height : 0;
        int h_right = p->p_right ? p->p_right->m_height : 0;
        p->m_height = static_cast<std::int8_t>(1 + std::max(h_left, h_right));
    }
}

template <typename T> void BinaryTree<T>::set_right(Node<T> *p, Node<T> *r)
{
    if (p)
    {
        if (p->p_right)
            destroy_subtree(p->p_right);
        p->p_right = r;

        int h_left = p->p_left ? p->p_left->m_height : 0;
        int h_right = p->p_right ? p->p_right->m_height : 0;
        p->m_height = static_cast<std::int8_t>(1 + std::max(h_left, h_right));
    }
}

template <typename T> void BinaryTree<T>::print_preorder() const
{
    preorder(p_head);
    std::cout << std::endl;
}
template <typename T> void BinaryTree<T>::print_inorder() const
{
    inorder(p_head);
    std::cout << std::endl;
}
template <typename T> void BinaryTree<T>::print_postorder() const
{
    postorder(p_head);
    std::cout << std::endl;
}
template <typename T> void BinaryTree<T>::print_levelorder() const
{
    levelorder(p_head);
    std::cout << std::endl;
}

template <typename T> Node<T> *BinaryTree<T>::make_node(const T &val)
{
    return m_pool.allocate(val);
}

template <typename T> Node<T> *BinaryTree<T>::make_node(const T &val, Node<T> *l, Node<T> *r)
{
    return m_pool.allocate(val, l, r);
}

template <typename T>
void BinaryTree<T>::print_tree_helper(const Node<T> *node, const std::string &prefix, bool is_left) const
{
    if (!node)
        return;

    print_tree_helper(node->p_right, prefix + (is_left ? "\u2502   " : "    "), false);

    std::cout << prefix << (is_left ? "\u2514\u2500\u2500 " : "\u250c\u2500\u2500 ") << node->m_data << "\n";

    print_tree_helper(node->p_left, prefix + (is_left ? "    " : "\u2502   "), true);
}

template <typename T> void BinaryTree<T>::print_tree() const
{
    if (!p_head)
    {
        std::cout << "(empty tree)" << std::endl;
        return;
    }

    print_tree_helper(p_head->p_right, "    ", false);

    std::cout << p_head->m_data << "\n";

    print_tree_helper(p_head->p_left, "    ", true);

    std::cout << std::flush;
}

template <typename T> void BinaryTree<T>::dot_helper(const Node<T> *node, std::ostream &out, int &null_count) const
{
    if (!node)
        return;

    if (node->p_left)
    {
        out << "    \"" << node->m_data << "\" -> \"" << node->p_left->m_data << "\";\n";
        dot_helper(node->p_left, out, null_count);
    }
    else
    {
        out << "    null" << null_count << " [shape=point];\n";
        out << "    \"" << node->m_data << "\" -> null" << null_count << ";\n";
        ++null_count;
    }

    if (node->p_right)
    {
        out << "    \"" << node->m_data << "\" -> \"" << node->p_right->m_data << "\";\n";
        dot_helper(node->p_right, out, null_count);
    }
    else
    {
        out << "    null" << null_count << " [shape=point];\n";
        out << "    \"" << node->m_data << "\" -> null" << null_count << ";\n";
        ++null_count;
    }
}

template <typename T> void BinaryTree<T>::dump_to_dot(const std::string &filename) const
{
    std::ofstream out(filename);
    if (!out.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    out << "digraph BST {\n";
    out << "    node [shape=circle];\n";

    if (p_head)
    {
        int null_count = 0;
        dot_helper(p_head, out, null_count);
    }

    out << "}\n";
    out.close();
}

} // namespace stl_ext