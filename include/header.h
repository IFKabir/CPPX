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

    void preorder(Node<T> *node) const;
    void inorder(Node<T> *node) const;
    void postorder(Node<T> *node) const;
    void levelorder(Node<T> *node) const;

  public:
    explicit BinaryTree(Node<T> *root) : head(root) {};

    // Setting Values
    void setLeft(Node<T> *parent, Node<T> *leftChild) const;
    void setRight(Node<T> *parent, Node<T> *rightChild) const;

    // Getting Root
    Node<T> *getRoot() const;

    // Traversals
    void printPreorder(Node<T> *node) const;
    void printInorder(Node<T> *node) const;
    void printPostorder(Node<T> *node) const;
    void printLevelorder(Node<T> *node) const;

    // Size & Empty
    bool isEmpty(Node<T> *node) const;
    int getSize(Node<T> *node);
  };

  template <typename T>
  class BinarySearchTree : public BinaryTree<T>
  {
  public:
    BinarySearchTree() : BinaryTree<T>(nullptr) {}
    explicit BinarySearchTree(Node<T> *root) : BinaryTree<T>(root) {}

    void setLeft(Node<T> *parent, Node<T> *leftChild) const;
    void setRight(Node<T> *parent, Node<T> *rightChild) const;

    Node<T> *search(Node<T> *root, T val) const;
    void insert(Node<T> *root, T val);
    void remove(Node<T> *root, T val);
  };
}
