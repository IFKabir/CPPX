namespace stl_ext
{

// ─── Node implementation ───────────────────────────────────────────────────────

template <typename T> Node<T>::Node(T val) : m_data(std::move(val))
{
}

template <typename T>
Node<T>::Node(T val, Node<T> *left, Node<T> *right) : m_data(std::move(val)), p_left(left), p_right(right)
{
}

template <typename T> int Node<T>::get_height_val() const
{
    return m_height;
}

template <typename T> void Node<T>::set_height_val(int h)
{
    m_height = static_cast<std::int8_t>(h);
}

template <typename T> const T &Node<T>::get_data() const
{
    return m_data;
}

template <typename T> void Node<T>::set_data(const T &val)
{
    m_data = val;
}

template <typename T> Node<T> *Node<T>::get_left() const
{
    return p_left;
}

template <typename T> void Node<T>::set_left(Node<T> *node)
{
    p_left = node;
}

template <typename T> Node<T> *Node<T>::get_right() const
{
    return p_right;
}

template <typename T> void Node<T>::set_right(Node<T> *node)
{
    p_right = node;
}

template <typename T> Color Node<T>::get_color() const
{
    return m_color;
}

template <typename T> void Node<T>::set_color(Color c)
{
    m_color = c;
}

template <typename T> Node<T> *Node<T>::get_parent() const
{
    return p_parent;
}

template <typename T> void Node<T>::set_parent(Node<T> *parent)
{
    p_parent = parent;
}

// ─── NodePool implementation ───────────────────────────────────────────────────

template <typename T> NodePool<T>::~NodePool()
{
    destroy_all();
}

template <typename T>
NodePool<T>::NodePool(NodePool &&other) noexcept
    : m_blocks(std::move(other.m_blocks)), m_free_list(std::move(other.m_free_list))
{
    other.m_blocks.clear();
    other.m_free_list.clear();
}

template <typename T> NodePool<T> &NodePool<T>::operator=(NodePool &&other) noexcept
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

template <typename T> template <typename... Args> Node<T> *NodePool<T>::allocate(Args &&...args)
{
    void *ptr = get_slot();
    return new (ptr) Node<T>(std::forward<Args>(args)...);
}

template <typename T> void NodePool<T>::deallocate(Node<T> *node)
{
    if (!node)
        return;
    node->~Node();
    m_free_list.push_back(static_cast<void *>(node));
}

template <typename T> void NodePool<T>::destroy_all()
{
    for (auto *blk : m_blocks)
        delete blk;
    m_blocks.clear();
    m_free_list.clear();
}

template <typename T> void *NodePool<T>::get_slot()
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

} // namespace stl_ext