#ifndef CPPX_H
#define CPPX_H

#include <cstddef>
#include <cstdint>
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

enum class Color : std::uint8_t
{
    RED,
    BLACK
};

template <typename T> class BST;
template <typename T> class AVLTree;
template <typename T> class RBTree;
template <typename T> class BinaryTree;

template <typename T> class Node
{
    friend class BST<T>;
    friend class AVLTree<T>;
    friend class RBTree<T>;
    friend class BinaryTree<T>;

  private:
    T m_data;
    Node<T> *p_left = nullptr;
    Node<T> *p_right = nullptr;
    Node<T> *p_parent = nullptr;
    std::int8_t m_height = 1;
    Color m_color = Color::RED;

  public:
    explicit Node(T val) : m_data(std::move(val))
    {
    }

    Node(T val, Node<T> *left, Node<T> *right) : m_data(std::move(val)), p_left(left), p_right(right)
    {
    }

    int get_height_val() const
    {
        return m_height;
    }

    void set_height_val(int h)
    {
        m_height = static_cast<std::int8_t>(h);
    }

    const T &get_data() const
    {
        return m_data;
    }

    void set_data(const T &val)
    {
        m_data = val;
    }

    Node<T> *get_left() const
    {
        return p_left;
    }

    void set_left(Node<T> *node)
    {
        p_left = node;
    }

    Node<T> *get_right() const
    {
        return p_right;
    }

    void set_right(Node<T> *node)
    {
        p_right = node;
    }

    Color get_color() const
    {
        return m_color;
    }

    void set_color(Color c)
    {
        m_color = c;
    }

    Node<T> *get_parent() const
    {
        return p_parent;
    }

    void set_parent(Node<T> *parent)
    {
        p_parent = parent;
    }
};

template <typename T> class NodePool
{
    struct Block
    {
        static constexpr std::size_t CAPACITY = 4096;
        alignas(alignof(T) > alignof(void *) ? alignof(T)
                                             : sizeof(void *)) unsigned char data[CAPACITY * sizeof(Node<T>)];
        std::size_t used = 0;
    };

    std::vector<Block *> m_blocks;
    std::vector<void *> m_free_list;

  public:
    NodePool() = default;

    ~NodePool()
    {
        destroy_all();
    }

    NodePool(const NodePool &) = delete;
    NodePool &operator=(const NodePool &) = delete;

    NodePool(NodePool &&other) noexcept : m_blocks(std::move(other.m_blocks)), m_free_list(std::move(other.m_free_list))
    {
        other.m_blocks.clear();
        other.m_free_list.clear();
    }

    NodePool &operator=(NodePool &&other) noexcept
    {
        if (this != &other)
        {
            destroy_all();
            m_blocks = std::move(other.m_blocks);
            m_free_list = std::move(other.m_free_list);
            other.m_blocks.clear();
            other.m_free_list.clear();
        }
        return *this;
    }

    template <typename... Args> Node<T> *allocate(Args &&...args)
    {
        void *ptr = get_slot();
        return new (ptr) Node<T>(std::forward<Args>(args)...);
    }

    void deallocate(Node<T> *node)
    {
        if (!node)
            return;
        node->~Node();
        m_free_list.push_back(static_cast<void *>(node));
    }

    void destroy_all()
    {
        for (auto *blk : m_blocks)
            delete blk;
        m_blocks.clear();
        m_free_list.clear();
    }

  private:
    void *get_slot()
    {
        if (!m_free_list.empty())
        {
            void *ptr = m_free_list.back();
            m_free_list.pop_back();
            return ptr;
        }
        if (m_blocks.empty() || m_blocks.back()->used >= Block::CAPACITY)
        {
            m_blocks.push_back(new Block());
        }
        auto &blk = *m_blocks.back();
        void *ptr = blk.data + blk.used * sizeof(Node<T>);
        ++blk.used;
        return ptr;
    }
};

template <typename T> class BinaryTree
{
  protected:
    Node<T> *p_head = nullptr;
    NodePool<T> m_pool;

    void preorder(const Node<T> *node) const;
    void inorder(const Node<T> *node) const;
    void postorder(const Node<T> *node) const;
    void levelorder(const Node<T> *node) const;
    int compute_size(const Node<T> *node) const;
    void print_tree_helper(const Node<T> *node, const std::string &prefix, bool is_left) const;
    void dot_helper(const Node<T> *node, std::ostream &out, int &null_count) const;
    void destroy_subtree(Node<T> *node);
    Node<T> *clone_subtree(const Node<T> *node);

  public:
    BinaryTree() = default;

    BinaryTree(const BinaryTree &other);
    BinaryTree &operator=(const BinaryTree &other);

    BinaryTree(BinaryTree &&other) noexcept;
    BinaryTree &operator=(BinaryTree &&other) noexcept;

    virtual ~BinaryTree();

    Node<T> *get_root() const;
    void set_root(Node<T> *root);
    bool is_empty() const;
    int size() const;
    void print_preorder() const;
    void print_inorder() const;
    void print_postorder() const;
    void print_levelorder() const;
    void print_tree() const;
    void dump_to_dot(const std::string &filename) const;

    Node<T> *make_node(const T &val);
    Node<T> *make_node(const T &val, Node<T> *left, Node<T> *right);
    void set_left(Node<T> *parent, Node<T> *left_child);
    void set_right(Node<T> *parent, Node<T> *right_child);
};

template <typename T> class BST : public BinaryTree<T>
{
  private:
    void insert_iterative(Node<T> *node, const T &val);
    void remove_impl(const T &val);

  public:
    using BinaryTree<T>::p_head;
    using BinaryTree<T>::m_pool;
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

    Node<T> *rotate_left(Node<T> *node);
    Node<T> *rotate_right(Node<T> *node);
    Node<T> *rebalance(Node<T> *node);

  public:
    void insert(const T &val) override;
    void remove(const T &val) override;
};

template <typename T> class RBTree : public BST<T>
{
  private:
    void rotate_left(Node<T> *node);
    void rotate_right(Node<T> *node);
    void fix_insert_violation(Node<T> *node);
    void fix_delete_violation(Node<T> *node, Node<T> *parent);
    Node<T> *tree_minimum(Node<T> *node) const;
    Color node_color(const Node<T> *node) const;
    int compute_black_height(const Node<T> *node) const;
    bool validate_rb_helper(const Node<T> *node) const;

  public:
    using BST<T>::p_head;
    using BST<T>::m_pool;

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
#include "../src/rbt.tpp"

#endif