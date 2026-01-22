#pragma once

#include <iostream>
#include <memory>
#include <queue>

namespace stl_ext
{

  template <typename T>
  class Node
  {
  private:
    T m_data;
    std::unique_ptr<Node<T>> p_left;
    std::unique_ptr<Node<T>> p_right;

  public:
    explicit Node() : p_left(nullptr), p_right(nullptr) {}
    explicit Node(const T &val) : m_data(val), p_left(nullptr), p_right(nullptr) {}
    explicit Node(const T &val, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right)
        : m_data(val), p_left(std::move(left)), p_right(std::move(right)) {}

    ~Node() = default;

    Node(const Node &other);

    Node &operator=(const Node &other);

    Node(Node &&other) noexcept = default;

    Node &operator=(Node &&other) noexcept = default;

    const T &get_data() const;
    void set_data(const T &val);
    Node *get_left() const;
    void set_left(std::unique_ptr<Node<T>> node);
    Node *get_right() const;
    void set_right(std::unique_ptr<Node<T>> node);
  };

  template <typename T>
  class BinaryTree
  {
  protected:
    std::unique_ptr<Node<T>> p_head;

  private:
    void preorder(const Node<T> *node) const;
    void inorder(const Node<T> *node) const;
    void postorder(const Node<T> *node) const;
    void levelorder(const Node<T> *node) const;
    int compute_size(const Node<T> *node) const;

  public:
    explicit BinaryTree() : p_head(nullptr) {}
    explicit BinaryTree(std::unique_ptr<Node<T>> root) : p_head(std::move(root)) {}

    virtual ~BinaryTree() = default;

    BinaryTree(const BinaryTree &other);

    BinaryTree &operator=(const BinaryTree &other);

    BinaryTree(BinaryTree &&other) noexcept = default;

    BinaryTree &operator=(BinaryTree &&other) noexcept = default;

    void set_left(Node<T> *parent, std::unique_ptr<Node<T>> left_child);
    void set_right(Node<T> *parent, std::unique_ptr<Node<T>> right_child);
    Node<T> *get_root() const;
    void set_root(std::unique_ptr<Node<T>> root);

    void print_preorder() const;
    void print_inorder() const;
    void print_postorder() const;
    void print_levelorder() const;

    bool is_empty() const;
    int size() const;

    static std::unique_ptr<Node<T>> make_node(const T &val);
    static std::unique_ptr<Node<T>> make_node(const T &val, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right);
  };

  template <typename T>
  class BST : public BinaryTree<T>
  {
  public:
    void insert(const T &val);
    bool contains(const T &val) const;
    void remove(const T &val);
    T get_min() const;
    T get_max() const;
    T get_successor(const T &val) const;
    T get_predecessor(const T &val) const;

  private:
    void insertRec(Node<T> *node, const T &val);
    void check_contains(const Node<T> *node, const T &val, bool &flag) const;
  };

} // namespace stl_ext
