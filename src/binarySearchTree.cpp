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
            std::cout << "Value of leftChild is not less then value of parent node" << std::endl;
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
            std::cout << "Value of rightChild is not more then value of parent node" << std::endl;
        }
    }
} // namespace stl_ext

template class stl_ext::BinarySearchTree<int>;
template class stl_ext::BinarySearchTree<float>;
template class stl_ext::BinarySearchTree<double>;
template class stl_ext::BinarySearchTree<char>;
template class stl_ext::BinarySearchTree<std::string>;