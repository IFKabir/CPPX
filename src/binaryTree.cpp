#include "header.h"
namespace stl_ext
{

    Node::Node(int &val) : data(val), left(nullptr), right(nullptr) {}

    void BinaryTree::preorder(Node *node)
    {
        if (node == nullptr)
            return;
        std::cout << node->data << " ";
        preorder(node->left);
        preorder(node->right);
    }
    void BinaryTree::postorder(Node *node)
    {
        if (node == nullptr)
            return;
        preorder(node->left);
        preorder(node->right);
        std::cout << node->data << " ";
    }
    void BinaryTree::inorder(Node *node)
    {
        if (node == nullptr)
            return;
        preorder(node->left);
        std::cout << node->data << " ";
        preorder(node->right);
    }
    void BinaryTree::levelorder(Node *node)
    {
        std::queue<Node *> q;
        q.push(node);
        while (!q.empty())
        {
            std::cout << q.front()->data << " ";
            if (q.front()->left != nullptr)
                q.push(q.front()->left);
            if (q.front()->right != nullptr)
                q.push(q.front()->right);
            q.pop();
        }
    }

    BinaryTree::BinaryTree(Node *node) : head(node) {}
    void BinaryTree::setLeft(Node *parent, Node *leftChild)
    {
        if (parent == nullptr)
            return;
        parent->left = leftChild;
    }
    void BinaryTree::setRight(Node *parent, Node *leftChild)
    {
        if (parent == nullptr)
            return;
        parent->right = leftChild;
    }
    Node *BinaryTree::getRoot() const
    {
        return head;
    }
    void BinaryTree::printPreorder(Node *node)
    {
        preorder(node);
        std::cout << std::endl;
    }
    void BinaryTree::printPostorder(Node *node)
    {
        postorder(node);
        std::cout << std::endl;
    }
    void BinaryTree::printInorder(Node *node)
    {
        inorder(node);
        std::cout << std::endl;
    }
    void BinaryTree::printLevelorder(Node *node)
    {
        levelorder(node);
        std::cout << std::endl;
    }
}
