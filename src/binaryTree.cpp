#include "header.h"

namespace stl_ext
{

    template <typename T>
    void BinaryTree<T>::preorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;
        std::cout << node->get_data() << " ";
        preorder(node->get_left());
        preorder(node->get_right());
    }

    template <typename T>
    void BinaryTree<T>::inorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;
        inorder(node->get_left());
        std::cout << node->get_data() << " ";
        inorder(node->get_right());
    }

    template <typename T>
    void BinaryTree<T>::postorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;
        postorder(node->get_left());
        postorder(node->get_right());
        std::cout << node->get_data() << " ";
    }

    template <typename T>
    void BinaryTree<T>::levelorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;
        std::queue<const Node<T> *> q;
        q.push(node);
        while (!q.empty())
        {
            const Node<T> *cur = q.front();
            q.pop();
            if (cur == nullptr)
                continue;
            std::cout << cur->get_data() << " ";
            if (cur->get_left() != nullptr)
                q.push(cur->get_left());
            if (cur->get_right() != nullptr)
                q.push(cur->get_right());
        }
    }

    template <typename T>
    int BinaryTree<T>::compute_size(const Node<T> *node) const
    {
        if (node == nullptr)
            return 0;
        return 1 + compute_size(node->get_left()) + compute_size(node->get_right());
    }

    template <typename T>
    void BinaryTree<T>::set_left(Node<T> *parent, std::unique_ptr<Node<T>> left_child)
    {
        if (parent == nullptr)
            return;
        parent->set_left(std::move(left_child));
    }

    template <typename T>
    void BinaryTree<T>::set_right(Node<T> *parent, std::unique_ptr<Node<T>> right_child)
    {
        if (parent == nullptr)
            return;
        parent->set_right(std::move(right_child));
    }

    template <typename T>
    Node<T> *BinaryTree<T>::get_root() const
    {
        return p_head.get();
    }

    template <typename T>
    void BinaryTree<T>::print_preorder() const
    {
        preorder(p_head.get());
        std::cout << std::endl;
    }

    template <typename T>
    void BinaryTree<T>::print_inorder() const
    {
        inorder(p_head.get());
        std::cout << std::endl;
    }

    template <typename T>
    void BinaryTree<T>::print_postorder() const
    {
        postorder(p_head.get());
        std::cout << std::endl;
    }

    template <typename T>
    void BinaryTree<T>::print_levelorder() const
    {
        levelorder(p_head.get());
        std::cout << std::endl;
    }

    template <typename T>
    bool BinaryTree<T>::is_empty() const
    {
        if (p_head == nullptr)
            return true;
        else
            return false;
    }

    template <typename T>
    int BinaryTree<T>::size() const
    {
        return compute_size(p_head.get());
    }

    template <typename T>
    void BinaryTree<T>::set_root(std::unique_ptr<Node<T>> root)
    {
        p_head = std::move(root);
    }

    template <typename T>
    std::unique_ptr<Node<T>> BinaryTree<T>::make_node(const T &val)
    {
        return std::make_unique<Node<T>>(val);
    }

    template <typename T>
    std::unique_ptr<Node<T>> BinaryTree<T>::make_node(const T &val, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right)
    {
        return std::make_unique<Node<T>>(val, std::move(left), std::move(right));
    }

} // namespace stl_ext

template class stl_ext::BinaryTree<int>;
template class stl_ext::BinaryTree<double>;
template class stl_ext::BinaryTree<float>;
template class stl_ext::BinaryTree<char>;
template class stl_ext::BinaryTree<long>;