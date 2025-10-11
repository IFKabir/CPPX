#include "header.h"

namespace stl_ext
{
    template <typename T>
    void BinarySearchTree<T>::setLeft(Node<T> *parent, Node<T> *leftChild) const
    {
        if (leftChild->data < parent->data)
        {
            parent->left = leftChild;
        }
        else
        {
            std::cout << "Value of leftChild is not less than value of parent node" << std::endl;
        }
    }

    template <typename T>
    void BinarySearchTree<T>::setRight(Node<T> *parent, Node<T> *rightChild) const
    {
        if (rightChild->data > parent->data)
        {
            parent->right = rightChild;
        }
        else
        {
            std::cout << "Value of rightChild is not greater than value of parent node" << std::endl;
        }
    }

    template <typename T>
    Node<T> *BinarySearchTree<T>::search(Node<T> *root, T val) const
    {
        if (root == nullptr || root->data == val)
            return root;
        if (root->data < val)
            return search(root->right, val);
        else
            return search(root->left, val);
    }

    template <typename T>
    Node<T> *BinarySearchTree<T>::insert(Node<T> *root, T val)
    {
        if (root == nullptr)
            return new Node<T>(val);

        if (val < root->data)
            root->left = insert(root->left, val);
        else if (val > root->data)
            root->right = insert(root->right, val);

        return root;
    }

    template <typename T>
    Node<T> *BinarySearchTree<T>::remove(Node<T> *root, T val)
    {
        if (root == nullptr)
            return root;

        if (val < root->data)
        {
            root->left = remove(root->left, val);
        }
        else if (val > root->data)
        {
            root->right = remove(root->right, val);
        }
        else
        {
            if (root->left == nullptr && root->right == nullptr)
            {
                delete root;
                return nullptr;
            }
            else if (root->left == nullptr)
            {
                Node<T> *temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr)
            {
                Node<T> *temp = root->left;
                delete root;
                return temp;
            }
            else
            {
                Node<T> *successor = root->right;
                while (successor->left != nullptr)
                {
                    successor = successor->left;
                }

                root->data = successor->data;

                root->right = remove(root->right, successor->data);
            }
        }
        return root;
    }

} // namespace stl_ext

template class stl_ext::BinarySearchTree<int>;
template class stl_ext::BinarySearchTree<float>;
template class stl_ext::BinarySearchTree<double>;
template class stl_ext::BinarySearchTree<char>;
template class stl_ext::BinarySearchTree<std::string>;
