#include "cppx.h"
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

namespace stl_ext
{
    template <typename T>
    void BinaryTree<T>::preorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;

        stack<const Node<T> *> s;
        s.push(node);

        while (!s.empty())
        {
            const Node<T> *curr = s.top();
            s.pop();

            cout << curr->get_data() << " ";

            if (curr->get_right() != nullptr)
                s.push(curr->get_right());
            if (curr->get_left() != nullptr)
                s.push(curr->get_left());
        }
    }

    template <typename T>
    void BinaryTree<T>::inorder(const Node<T> *node) const
    {
        stack<const Node<T> *> s;
        const Node<T> *curr = node;

        while (curr != nullptr || !s.empty())
        {
            while (curr != nullptr)
            {
                s.push(curr);
                curr = curr->get_left();
            }

            curr = s.top();
            s.pop();

            cout << curr->get_data() << " ";

            curr = curr->get_right();
        }
    }

    template <typename T>
    void BinaryTree<T>::postorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;

        stack<const Node<T> *> s;
        const Node<T> *curr = node;
        const Node<T> *last_visited = nullptr;

        while (!s.empty() || curr != nullptr)
        {
            if (curr != nullptr)
            {
                s.push(curr);
                curr = curr->get_left();
            }
            else
            {
                const Node<T> *peek_node = s.top();

                if (peek_node->get_right() != nullptr && last_visited != peek_node->get_right())
                {
                    curr = peek_node->get_right();
                }
                else
                {
                    cout << peek_node->get_data() << " ";
                    last_visited = peek_node;
                    s.pop();
                }
            }
        }
    }

    template <typename T>
    int BinaryTree<T>::compute_size(const Node<T> *node) const
    {
        if (node == nullptr)
            return 0;

        int count = 0;
        queue<const Node<T> *> q;
        q.push(node);

        while (!q.empty())
        {
            const Node<T> *curr = q.front();
            q.pop();
            count++;

            if (curr->get_left() != nullptr)
                q.push(curr->get_left());
            if (curr->get_right() != nullptr)
                q.push(curr->get_right());
        }

        return count;
    }

    template <typename T>
    void BinaryTree<T>::levelorder(const Node<T> *node) const
    {
        if (node == nullptr)
            return;

        queue<const Node<T> *> q;
        q.push(node);

        while (!q.empty())
        {
            const Node<T> *cur = q.front();
            q.pop();

            if (cur == nullptr)
                continue;

            cout << cur->get_data() << " ";

            if (cur->get_left() != nullptr)
                q.push(cur->get_left());
            if (cur->get_right() != nullptr)
                q.push(cur->get_right());
        }
    }

    template <typename T>
    BinaryTree<T>::BinaryTree(const BinaryTree &other)
    {
        if (other.p_head)
            p_head = make_unique<Node<T>>(*other.p_head);
        else
            p_head = nullptr;
    }

    template <typename T>
    BinaryTree<T> &BinaryTree<T>::operator=(const BinaryTree<T> &other)
    {
        if (this == &other)
            return *this;
        p_head = other.p_head ? make_unique<Node<T>>(*other.p_head) : nullptr;
        return *this;
    }

    template <typename T>
    void BinaryTree<T>::set_left(Node<T> *parent, unique_ptr<Node<T>> left_child)
    {
        if (parent == nullptr)
            return;
        parent->set_left(move(left_child));
    }

    template <typename T>
    void BinaryTree<T>::set_right(Node<T> *parent, unique_ptr<Node<T>> right_child)
    {
        if (parent == nullptr)
            return;
        parent->set_right(move(right_child));
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
        cout << endl;
    }

    template <typename T>
    void BinaryTree<T>::print_inorder() const
    {
        inorder(p_head.get());
        cout << endl;
    }

    template <typename T>
    void BinaryTree<T>::print_postorder() const
    {
        postorder(p_head.get());
        cout << endl;
    }

    template <typename T>
    void BinaryTree<T>::print_levelorder() const
    {
        levelorder(p_head.get());
        cout << endl;
    }

    template <typename T>
    bool BinaryTree<T>::is_empty() const
    {
        return p_head == nullptr;
    }

    template <typename T>
    int BinaryTree<T>::size() const
    {
        return compute_size(p_head.get());
    }

    template <typename T>
    void BinaryTree<T>::set_root(unique_ptr<Node<T>> root)
    {
        p_head = move(root);
    }

    template <typename T>
    unique_ptr<Node<T>> BinaryTree<T>::make_node(const T &val)
    {
        return make_unique<Node<T>>(val);
    }

    template <typename T>
    unique_ptr<Node<T>> BinaryTree<T>::make_node(const T &val, unique_ptr<Node<T>> left, unique_ptr<Node<T>> right)
    {
        return make_unique<Node<T>>(val, move(left), move(right));
    }

} // namespace stl_ext

template class stl_ext::BinaryTree<int>;
template class stl_ext::BinaryTree<double>;
template class stl_ext::BinaryTree<float>;
template class stl_ext::BinaryTree<char>;
template class stl_ext::BinaryTree<long>;