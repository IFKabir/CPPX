#pragma once
#include <iostream>
#include <queue>

namespace stl_ext
{
  template <typename T>
  class Node
  {
  public:
    T data;
    Node<T> *left;
    Node<T> *right;

    Node(const T &val) : data(val), left(nullptr), right(nullptr) {};
  };

  template <typename T>
  class BinaryTree
  {
  private:
    Node<T> *head;

    void preorder(Node<T> *node);
    void inorder(Node<T> *node);
    void postorder(Node<T> *node);
    void levelorder(Node<T> *node);

  public:
    explicit BinaryTree(Node<T> *root) : head(root) {};

    void setLeft(Node<T> *parent, Node<T> *leftChild);
    void setRight(Node<T> *parent, Node<T> *rightChild);
    Node<T> *getRoot() const;
    void printPreorder(Node<T> *node);
    void printInorder(Node<T> *node);
    void printPostorder(Node<T> *node);
    void printLevelorder(Node<T> *node);
  };
}
