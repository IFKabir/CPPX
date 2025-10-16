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
    // Constructors
    explicit Node() : p_left(nullptr), p_right(nullptr) {}
    explicit Node(const T &val) : m_data(val), p_left(nullptr), p_right(nullptr) {}
    explicit Node(const T &val, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right)
        : m_data(val), p_left(std::move(left)), p_right(std::move(right)) {}

    // Destructor
    ~Node() = default;

    // Copy Constructor
    Node(const Node &other)
        : m_data(other.m_data)
    {
      if (other.p_left)
        p_left = std::make_unique<Node<T>>(*other.p_left);
      if (other.p_right)
        p_right = std::make_unique<Node<T>>(*other.p_right);
    }

    // Copy Assignment Operator
    Node &operator=(const Node &other)
    {
      if (this == &other)
        return *this;
      m_data = other.m_data;
      p_left = other.p_left ? std::make_unique<Node<T>>(*other.p_left) : nullptr;
      p_right = other.p_right ? std::make_unique<Node<T>>(*other.p_right) : nullptr;
      return *this;
    }

    // Move Constructor
    Node(Node &&other) noexcept = default;

    // Move Assignment Operator
    Node &operator=(Node &&other) noexcept = default;

    // Accessors
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
  private:
    std::unique_ptr<Node<T>> p_head;

    void preorder(const Node<T> *node) const;
    void inorder(const Node<T> *node) const;
    void postorder(const Node<T> *node) const;
    void levelorder(const Node<T> *node) const;
    int compute_size(const Node<T> *node) const;

  public:
    explicit BinaryTree() : p_head(nullptr) {}
    explicit BinaryTree(std::unique_ptr<Node<T>> root) : p_head(std::move(root)) {}

    // Destructor
    ~BinaryTree() = default;

    // Copy Constructor
    BinaryTree(const BinaryTree &other)
    {
      if (other.p_head)
        p_head = std::make_unique<Node<T>>(*other.p_head);
      else
        p_head = nullptr;
    }

    // Copy Assignment Operator
    BinaryTree &operator=(const BinaryTree &other)
    {
      if (this == &other)
        return *this;
      p_head = other.p_head ? std::make_unique<Node<T>>(*other.p_head) : nullptr;
      return *this;
    }

    // Move Constructor
    BinaryTree(BinaryTree &&other) noexcept = default;

    // Move Assignment Operator
    BinaryTree &operator=(BinaryTree &&other) noexcept = default;

    // Tree manipulation
    void set_left(Node<T> *parent, std::unique_ptr<Node<T>> left_child);
    void set_right(Node<T> *parent, std::unique_ptr<Node<T>> right_child);
    Node<T> *get_root() const;
    void set_root(std::unique_ptr<Node<T>> root);

    // Public traversal methods
    void print_preorder() const;
    void print_inorder() const;
    void print_postorder() const;
    void print_levelorder() const;

    // Tree properties
    bool is_empty() const;
    int size() const;

    // Static factory methods
    static std::unique_ptr<Node<T>> make_node(const T &val);
    static std::unique_ptr<Node<T>> make_node(const T &val, std::unique_ptr<Node<T>> left, std::unique_ptr<Node<T>> right);
  };

} // namespace stl_ext
