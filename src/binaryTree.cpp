#include "header.h"

namespace stl_ext
{
    // constructors are defined in header file

    template <typename T>
    void BinaryTree<T>::preorder(Node<T> *node)
    {
        if (node == nullptr)
            return;
        std::cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }

    template <typename T>
    void BinaryTree<T>::inorder(Node<T> *node)
    {
        if (node == nullptr)
            return;
        inorder(node->left);
        std::cout << node->data << " ";
        inorder(node->right);
    }

    template <typename T>
    void BinaryTree<T>::postorder(Node<T> *node)
    {
        if (node == nullptr)
            return;
        postorder(node->left);
        postorder(node->right);
        std::cout << node->data << " ";
    }

    template <typename T>
    void BinaryTree<T>::levelorder(Node<T> *node)
    {
        if (node == nullptr)
            return;
        std::queue<Node<T> *> q;
        q.push(node);
        while (!q.empty())
        {
            Node<T> *cur = q.front();
            q.pop();
            if (cur == nullptr)
                continue;
            std::cout << cur->data << " ";
            if (cur->left != nullptr)
                q.push(cur->left);
            if (cur->right != nullptr)
                q.push(cur->right);
        }
    }

    template <typename T>
    void BinaryTree<T>::setLeft(Node<T> *parent, Node<T> *leftChild) const
    {
        if (parent == nullptr)
            return;
        parent->left = leftChild;
        parent->size += leftChild->size;
    }

    template <typename T>
    void BinaryTree<T>::setRight(Node<T> *parent, Node<T> *rightChild) const
    {
        if (parent == nullptr)
            return;
        parent->right = rightChild;
        parent->size += rightChild->size;
    }

    template <typename T>
    Node<T> *BinaryTree<T>::getRoot() const
    {
        return head;
    }

    template <typename T>
    void BinaryTree<T>::printPreorder(Node<T> *node) const
    {
        preorder(node);
        std::cout << std::endl;
    }

    template <typename T>
    void BinaryTree<T>::printInorder(Node<T> *node) const
    {
        inorder(node);
        std::cout << std::endl;
    }

    template <typename T>
    void BinaryTree<T>::printPostorder(Node<T> *node) const
    {
        postorder(node);
        std::cout << std::endl;
    }

    template <typename T>
    void BinaryTree<T>::printLevelorder(Node<T> *node) const
    {
        levelorder(node);
        std::cout << std::endl;
    }

    template <typename T>
    bool BinaryTree<T>::isEmpty(Node<T> *node) const
    {
        if (node == nullptr)
            return true;
        else
            return false;
    }

    

} // namespace stl_ext

template class stl_ext::BinaryTree<int>;
template class stl_ext::BinaryTree<float>;
template class stl_ext::BinaryTree<double>;
template class stl_ext::BinaryTree<char>;
template class stl_ext::BinaryTree<std::string>;