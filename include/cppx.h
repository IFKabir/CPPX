#ifndef CPPX_H
#define CPPX_H

#include <iostream>
#include <memory>
#include <queue>
#include <stack>
#include <stdexcept>
#include <vector>

namespace stl_ext
{

template <typename T> class Node
{
  private:
    T m_data;
    std::unique_ptr<Node<T>> p_left;
    std::unique_ptr<Node<T>> p_right;
    int m_height = 1;

  public:
    explicit Node(T val) : m_data(val), p_left(nullptr), p_right(nullptr), m_height(1)
    {
    }
    Node(T val, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right)
        : m_data(val), p_left(std::move(left)), p_right(std::move(right)), m_height(1)
    {
    }

    Node(const Node &other);
    Node &operator=(const Node &other);
    Node(Node &&other) noexcept = default;
    Node &operator=(Node &&other) noexcept = default;
    int get_height_val() const;
    void set_height_val(int h);
    std::unique_ptr<Node<T>> detach_left();
    std::unique_ptr<Node<T>> detach_right();
    const T &get_data() const;
    void set_data(const T &val);
    Node<T> *get_left() const;
    void set_left(std::unique_ptr<Node<T>> node);
    Node<T> *get_right() const;
    void set_right(std::unique_ptr<Node<T>> node);
};

template <typename T> class BinaryTree
{
  protected:
    std::unique_ptr<Node<T>> p_head;
    void preorder(const Node<T> *node) const;
    void inorder(const Node<T> *node) const;
    void postorder(const Node<T> *node) const;
    void levelorder(const Node<T> *node) const;
    int compute_size(const Node<T> *node) const;

  public:
    BinaryTree() : p_head(nullptr)
    {
    }
    BinaryTree(const BinaryTree &other);
    BinaryTree &operator=(const BinaryTree &other);
    BinaryTree(BinaryTree &&other) noexcept = default;
    BinaryTree &operator=(BinaryTree &&other) noexcept = default;
    virtual ~BinaryTree() = default;

    Node<T> *get_root() const;
    void set_root(std::unique_ptr<Node<T>> root);
    bool is_empty() const;
    int size() const;
    void print_preorder() const;
    void print_inorder() const;
    void print_postorder() const;
    void print_levelorder() const;

    static std::unique_ptr<Node<T>> make_node(const T &val);
    static std::unique_ptr<Node<T>> make_node(const T &val, std::unique_ptr<Node<T>> left,
                                              std::unique_ptr<Node<T>> right);
    void set_left(Node<T> *parent, std::unique_ptr<Node<T>> left_child);
    void set_right(Node<T> *parent, std::unique_ptr<Node<T>> right_child);
};

template <typename T> class BST : public BinaryTree<T>
{
  private:
    void insert_iterative(Node<T> *node, const T &val);

  public:
    using BinaryTree<T>::p_head;
    virtual void insert(const T &val);
    bool contains(const T &val) const;
    virtual void remove(const T &val);
    T get_min() const;
    T get_max() const;
    T get_successor(const T &val) const;
    T get_predecessor(const T &val) const;
};

template <typename T> class AVLTree : public BST<T>
{

  private:
    int get_height(const Node<T> *node) const;
    int get_balance_factor(const Node<T> *node) const;
    void update_height(Node<T> *node);

    std::unique_ptr<Node<T>> rotate_left(std::unique_ptr<Node<T>> node);
    std::unique_ptr<Node<T>> rotate_right(std::unique_ptr<Node<T>> node);
    std::unique_ptr<Node<T>> rebalance(std::unique_ptr<Node<T>> node);

    std::unique_ptr<Node<T>> insert_helper(std::unique_ptr<Node<T>> node, const T &val);
    std::unique_ptr<Node<T>> remove_helper(std::unique_ptr<Node<T>> node, const T &val);

  public:
    void insert(const T &val) override;
    void remove(const T &val) override;
};

} // namespace stl_ext

#include "avl.tpp"
#include "binary_tree.tpp"
#include "bst.tpp"
#include "node.tpp"

#endif