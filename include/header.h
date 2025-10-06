#pragma once
#include <iostream>
#include <queue>

namespace stl_ext
{
  class Node
  {
  public:
    int data;
    Node *left;
    Node *right;

    explicit Node(int &val);
  };

  class BinaryTree
  {
  private:
    Node *head;

    void preorder(Node *node);
    void inorder(Node *node);
    void postorder(Node *node);
    void levelorder(Node *node);

  public:
    BinaryTree();
    BinaryTree(Node *root);

    void setLeft(Node *parent, Node *leftChild);
    void setRight(Node *parent, Node *rightChild);
    Node *getRoot() const;
    void printPreorder(Node *node);
    void printInorder(Node *node);
    void printPostorder(Node *node);
    void printLevelorder(Node *node);
  };
}
