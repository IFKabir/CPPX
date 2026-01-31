#include <algorithm>

namespace stl_ext
{

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
        std::cout << curr->get_data() << " ";
        if (curr->get_right())
            s.push(curr->get_right());
        if (curr->get_left())
            s.push(curr->get_left());
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
            curr = curr->get_left();
        }
        curr = s.top();
        s.pop();
        std::cout << curr->get_data() << " ";
        curr = curr->get_right();
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
            curr = curr->get_left();
        }
        else
        {
            const Node<T> *peek = s.top();
            if (peek->get_right() && last != peek->get_right())
                curr = peek->get_right();
            else
            {
                std::cout << peek->get_data() << " ";
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
        std::cout << cur->get_data() << " ";
        if (cur->get_left())
            q.push(cur->get_left());
        if (cur->get_right())
            q.push(cur->get_right());
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
        if (curr->get_left())
            q.push(curr->get_left());
        if (curr->get_right())
            q.push(curr->get_right());
    }
    return c;
}

template <typename T> BinaryTree<T>::BinaryTree(const BinaryTree &other)
{
    if (other.p_head)
        p_head = std::make_unique<Node<T>>(*other.p_head);
}

template <typename T> BinaryTree<T> &BinaryTree<T>::operator=(const BinaryTree<T> &other)
{
    if (this == &other)
        return *this;
    p_head = other.p_head ? std::make_unique<Node<T>>(*other.p_head) : nullptr;
    return *this;
}

template <typename T> Node<T> *BinaryTree<T>::get_root() const
{
    return p_head.get();
}

template <typename T> void BinaryTree<T>::set_root(std::unique_ptr<Node<T>> root)
{
    p_head = std::move(root);
}

template <typename T> bool BinaryTree<T>::is_empty() const
{
    return p_head == nullptr;
}

template <typename T> int BinaryTree<T>::size() const
{
    return compute_size(p_head.get());
}

template <typename T> void BinaryTree<T>::set_left(Node<T> *p, std::unique_ptr<Node<T>> l)
{
    if (p)
    {
        p->set_left(std::move(l));

        int h_left = p->get_left() ? p->get_left()->get_height_val() : 0;
        int h_right = p->get_right() ? p->get_right()->get_height_val() : 0;
        p->set_height_val(1 + std::max(h_left, h_right));
    }
}

template <typename T> void BinaryTree<T>::set_right(Node<T> *p, std::unique_ptr<Node<T>> r)
{
    if (p)
    {
        p->set_right(std::move(r));

        int h_left = p->get_left() ? p->get_left()->get_height_val() : 0;
        int h_right = p->get_right() ? p->get_right()->get_height_val() : 0;
        p->set_height_val(1 + std::max(h_left, h_right));
    }
}

template <typename T> void BinaryTree<T>::print_preorder() const
{
    preorder(p_head.get());
    std::cout << std::endl;
}
template <typename T> void BinaryTree<T>::print_inorder() const
{
    inorder(p_head.get());
    std::cout << std::endl;
}
template <typename T> void BinaryTree<T>::print_postorder() const
{
    postorder(p_head.get());
    std::cout << std::endl;
}
template <typename T> void BinaryTree<T>::print_levelorder() const
{
    levelorder(p_head.get());
    std::cout << std::endl;
}

template <typename T> std::unique_ptr<Node<T>> BinaryTree<T>::make_node(const T &val)
{
    return std::make_unique<Node<T>>(val);
}

template <typename T>
std::unique_ptr<Node<T>> BinaryTree<T>::make_node(const T &val, std::unique_ptr<Node<T>> l, std::unique_ptr<Node<T>> r)
{
    return std::make_unique<Node<T>>(val, std::move(l), std::move(r));
}

} // namespace stl_ext