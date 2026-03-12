#ifndef CPPX_H
#define CPPX_H

#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <vector>

namespace stl_ext
{

enum class Color
{
    RED,
    BLACK
};

template <typename T> class BST;
template <typename T> class RBTree;

template <typename T> class Node
{

    friend class BST<T>;
    friend class RBTree<T>;

  private:
    T m_data;
    std::unique_ptr<Node<T>> p_left;
    std::unique_ptr<Node<T>> p_right;
    int m_height = 1;
    Color m_color = Color::RED;
    Node<T> *p_parent = nullptr;

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

    Color get_color() const;

    void set_color(Color c);

    Node<T> *get_parent() const;

    void set_parent(Node<T> *parent);
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
    void print_tree_helper(const Node<T> *node, const std::string &prefix, bool is_left) const;
    void dot_helper(const Node<T> *node, std::ostream &out, int &null_count) const;

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
    void print_tree() const;
    void dump_to_dot(const std::string &filename) const;

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
    std::unique_ptr<Node<T>> remove_impl(std::unique_ptr<Node<T>> node, const T &val);

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

template <typename T> class RBTree : public BST<T>
{
  private:
    void rotate_left(Node<T> *node);

    void rotate_right(Node<T> *node);

    void transplant(Node<T> *u, Node<T> *v);

    void fix_insert_violation(Node<T> *node);

    void fix_delete_violation(Node<T> *node, Node<T> *parent);

    Node<T> *tree_minimum(Node<T> *node) const;

    Color node_color(const Node<T> *node) const;

    Node<T> *release_child(Node<T> *parent, Node<T> *child);

    void attach_child(Node<T> *parent, std::unique_ptr<Node<T>> child, bool as_left);

    int compute_black_height(const Node<T> *node) const;

    bool validate_rb_helper(const Node<T> *node) const;

  public:
    using BST<T>::p_head;

    void insert(const T &val) override;

    void remove(const T &val) override;

    void clear();

    static bool is_red(const Node<T> *node);

    static bool is_black(const Node<T> *node);

    static Color get_color(const Node<T> *node);

    int get_black_height() const;

    bool validate_rb_properties() const;

    std::vector<T> to_sorted_vector() const;
};

} // namespace stl_ext

#include "../src/avl.tpp"
#include "../src/binary_tree.tpp"
#include "../src/bst.tpp"
#include "../src/node.tpp"
#include "../src/rbt.tpp"

#endif