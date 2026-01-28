#include "cppx.h"
#include <stdexcept>

namespace stl_ext
{
    template <typename T>
    void BST<T>::insert(const T &val)
    {
        if (!p_head)
        {
            p_head = BinaryTree<T>::make_node(val);
            return;
        }

        insertIterative(p_head.get(), val);
    }

    template <typename T>
    bool BST<T>::contains(const T &val) const
    {
        Node<T> *current = p_head.get();
        while (current != nullptr)
        {
            if (val == current->get_data())
                return true;

            if (val < current->get_data())
                current = current->get_left();
            else
                current = current->get_right();
        }
        return false;
    }

    template <typename T>
    void BST<T>::remove(const T &val)
    {
        Node<T> *current = p_head.get();
        Node<T> *previous = nullptr;

        while (current && current->get_data() != val)
        {
            previous = current;
            if (val < current->get_data())
                current = current->get_left();
            else
                current = current->get_right();
        }

        if (!current)
        {
            throw std::runtime_error("Value not found in BST");
        }

        if (!current->get_left() || !current->get_right())
        {
            Node<T> *child = current->get_left() ? current->get_left() : current->get_right();
            if (!previous)
            {
                if (child)
                {
                    if (current == p_head.get()->get_left())
                        p_head = std::unique_ptr<Node<T>>(current->get_right() ? current->get_right() : current->get_left());
                    else
                        p_head.reset();
                }
                else
                    p_head.reset();
            }
            else if (current == previous->get_left())
            {
                if (child)
                    previous->set_left(std::unique_ptr<Node<T>>(child));
                else
                    previous->set_left(nullptr);
            }
            else
            {
                if (child)
                    previous->set_right(std::unique_ptr<Node<T>>(child));
                else
                    previous->set_right(nullptr);
            }
        }
        else
        {
            Node<T> *succ_parent = current;
            Node<T> *succ = current->get_right();
            while (succ->get_left())
            {
                succ_parent = succ;
                succ = succ->get_left();
            }
            current->set_data(succ->get_data());
            if (succ_parent->get_left() == succ)
            {
                Node<T> *temp = succ->get_right();
                if (temp)
                    succ_parent->set_left(std::unique_ptr<Node<T>>(temp));
                else
                    succ_parent->set_left(nullptr);
            }
            else
            {
                Node<T> *temp = succ->get_right();
                if (temp)
                    succ_parent->set_right(std::unique_ptr<Node<T>>(temp));
                else
                    succ_parent->set_right(nullptr);
            }
        }
    }

    template <typename T>
    T BST<T>::get_min() const
    {
        Node<T> *root = p_head.get();
        if (!root)
            throw std::runtime_error("Cannot get_min() from an empty BST");

        while (root->get_left() != nullptr)
        {
            root = root->get_left();
        }
        return root->get_data();
    }

    template <typename T>
    T BST<T>::get_max() const
    {
        Node<T> *root = p_head.get();
        if (!root)
            throw std::runtime_error("Cannot get_max() from an empty BST");

        while (root->get_right() != nullptr)
        {
            root = root->get_right();
        }
        return root->get_data();
    }

    template <typename T>
    T BST<T>::get_successor(const T &val) const
    {
        Node<T> *current = this->get_root();
        Node<T> *successor = nullptr;

        while (current != nullptr)
        {
            if (current->get_data() > val)
            {
                successor = current;
                current = current->get_left();
            }
            else
            {
                current = current->get_right();
            }
        }

        if (successor == nullptr)
            throw std::runtime_error("No successor found (value is maximum or tree is empty)");

        return successor->get_data();
    }

    template <typename T>
    T BST<T>::get_predecessor(const T &val) const
    {
        Node<T> *current = this->get_root();
        Node<T> *predecessor = nullptr;

        while (current != nullptr)
        {
            if (current->get_data() < val)
            {
                predecessor = current;
                current = current->get_right();
            }
            else
            {
                current = current->get_left();
            }
        }

        if (predecessor == nullptr)
            throw std::runtime_error("No predecessor found (value is minimum or tree is empty)");

        return predecessor->get_data();
    }

    template <typename T>
    void BST<T>::insert_iterative(Node<T> *node_ptr, const T &val)
    {
        Node<T> *current = node_ptr;

        while (true)
        {
            if (val <= current->get_data())
            {
                if (current->get_left() == nullptr)
                {
                    current->set_left(BinaryTree<T>::make_node(val));
                    return;
                }
                current = current->get_left();
            }
            else
            {
                if (current->get_right() == nullptr)
                {
                    current->set_right(BinaryTree<T>::make_node(val));
                    return;
                }
                current = current->get_right();
            }
        }
    }
} // namespace stl_ext

template class stl_ext::BST<int>;
template class stl_ext::BST<double>;
template class stl_ext::BST<float>;
template class stl_ext::BST<char>;
template class stl_ext::BST<long>;