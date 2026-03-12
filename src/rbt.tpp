namespace stl_ext
{

template <typename T> bool RBTree<T>::is_red(const Node<T> *node)
{
    return node != nullptr && node->m_color == Color::RED;
}

template <typename T> bool RBTree<T>::is_black(const Node<T> *node)
{
    return node == nullptr || node->m_color == Color::BLACK;
}

template <typename T> Color RBTree<T>::get_color(const Node<T> *node)
{
    return (node == nullptr) ? Color::BLACK : node->m_color;
}

template <typename T> Color RBTree<T>::node_color(const Node<T> *node) const
{
    return (node == nullptr) ? Color::BLACK : node->m_color;
}

template <typename T> Node<T> *RBTree<T>::tree_minimum(Node<T> *node) const
{
    while (node->p_left)
        node = node->p_left;
    return node;
}

template <typename T> void RBTree<T>::rotate_left(Node<T> *x)
{
    Node<T> *y = x->p_right;
    Node<T> *p = x->p_parent;

    x->p_right = y->p_left;
    if (y->p_left)
        y->p_left->p_parent = x;

    y->p_left = x;
    x->p_parent = y;

    y->p_parent = p;
    if (!p)
        this->p_head = y;
    else if (p->p_left == x)
        p->p_left = y;
    else
        p->p_right = y;
}

template <typename T> void RBTree<T>::rotate_right(Node<T> *x)
{
    Node<T> *y = x->p_left;
    Node<T> *p = x->p_parent;

    x->p_left = y->p_right;
    if (y->p_right)
        y->p_right->p_parent = x;

    y->p_right = x;
    x->p_parent = y;

    y->p_parent = p;
    if (!p)
        this->p_head = y;
    else if (p->p_left == x)
        p->p_left = y;
    else
        p->p_right = y;
}

template <typename T> void RBTree<T>::insert(const T &val)
{
    Node<T> *z = this->m_pool.allocate(val);
    z->m_color = Color::RED;
    z->p_left = nullptr;
    z->p_right = nullptr;

    if (!this->p_head)
    {
        z->m_color = Color::BLACK;
        z->p_parent = nullptr;
        this->p_head = z;
        return;
    }

    Node<T> *curr = this->p_head;
    Node<T> *parent = nullptr;

    while (curr)
    {
        parent = curr;
        if (val < curr->m_data)
            curr = curr->p_left;
        else if (val > curr->m_data)
            curr = curr->p_right;
        else
        {
            this->m_pool.deallocate(z);
            return;
        }
    }

    z->p_parent = parent;
    if (val < parent->m_data)
        parent->p_left = z;
    else
        parent->p_right = z;

    fix_insert_violation(z);
}

template <typename T> void RBTree<T>::fix_insert_violation(Node<T> *z)
{
    while (z->p_parent && z->p_parent->m_color == Color::RED)
    {
        Node<T> *parent = z->p_parent;
        Node<T> *grandparent = parent->p_parent;

        if (!grandparent)
            break;

        if (parent == grandparent->p_left)
        {
            Node<T> *uncle = grandparent->p_right;

            if (node_color(uncle) == Color::RED)
            {
                parent->m_color = Color::BLACK;
                uncle->m_color = Color::BLACK;
                grandparent->m_color = Color::RED;
                z = grandparent;
            }
            else
            {
                if (z == parent->p_right)
                {
                    z = parent;
                    rotate_left(z);
                    parent = z->p_parent;
                    grandparent = parent->p_parent;
                }

                parent->m_color = Color::BLACK;
                grandparent->m_color = Color::RED;
                rotate_right(grandparent);
            }
        }
        else
        {
            Node<T> *uncle = grandparent->p_left;

            if (node_color(uncle) == Color::RED)
            {
                parent->m_color = Color::BLACK;
                uncle->m_color = Color::BLACK;
                grandparent->m_color = Color::RED;
                z = grandparent;
            }
            else
            {
                if (z == parent->p_left)
                {
                    z = parent;
                    rotate_right(z);
                    parent = z->p_parent;
                    grandparent = parent->p_parent;
                }

                parent->m_color = Color::BLACK;
                grandparent->m_color = Color::RED;
                rotate_left(grandparent);
            }
        }
    }

    this->p_head->m_color = Color::BLACK;
}

template <typename T> void RBTree<T>::remove(const T &val)
{
    Node<T> *z = this->p_head;
    while (z)
    {
        if (val < z->m_data)
            z = z->p_left;
        else if (val > z->m_data)
            z = z->p_right;
        else
            break;
    }
    if (!z)
        return;

    Node<T> *y = z;
    Color y_original_color = y->m_color;
    Node<T> *x = nullptr;
    Node<T> *x_parent = nullptr;

    if (!z->p_left)
    {
        x = z->p_right;
        x_parent = z->p_parent;

        if (x)
            x->p_parent = z->p_parent;

        if (!z->p_parent)
            this->p_head = x;
        else if (z->p_parent->p_left == z)
            z->p_parent->p_left = x;
        else
            z->p_parent->p_right = x;

        this->m_pool.deallocate(z);
    }
    else if (!z->p_right)
    {
        x = z->p_left;
        x_parent = z->p_parent;

        if (x)
            x->p_parent = z->p_parent;

        if (!z->p_parent)
            this->p_head = x;
        else if (z->p_parent->p_left == z)
            z->p_parent->p_left = x;
        else
            z->p_parent->p_right = x;

        this->m_pool.deallocate(z);
    }
    else
    {
        y = tree_minimum(z->p_right);
        y_original_color = y->m_color;
        x = y->p_right;

        if (y->p_parent == z)
        {
            x_parent = y;

            if (x)
                x->p_parent = y;

            y->p_left = z->p_left;
            if (z->p_left)
                z->p_left->p_parent = y;

            if (!z->p_parent)
                this->p_head = y;
            else if (z->p_parent->p_left == z)
                z->p_parent->p_left = y;
            else
                z->p_parent->p_right = y;

            y->p_parent = z->p_parent;
            y->m_color = z->m_color;
            this->m_pool.deallocate(z);
        }
        else
        {
            x_parent = y->p_parent;

            if (x)
                x->p_parent = y->p_parent;

            if (y->p_parent->p_left == y)
                y->p_parent->p_left = x;
            else
                y->p_parent->p_right = x;

            y->p_left = z->p_left;
            if (z->p_left)
                z->p_left->p_parent = y;

            y->p_right = z->p_right;
            if (z->p_right)
                z->p_right->p_parent = y;

            if (!z->p_parent)
                this->p_head = y;
            else if (z->p_parent->p_left == z)
                z->p_parent->p_left = y;
            else
                z->p_parent->p_right = y;

            y->p_parent = z->p_parent;
            y->m_color = z->m_color;
            this->m_pool.deallocate(z);
        }
    }

    if (y_original_color == Color::BLACK)
    {
        fix_delete_violation(x, x_parent);
    }
}

template <typename T> void RBTree<T>::fix_delete_violation(Node<T> *x, Node<T> *x_parent)
{
    while (x != this->p_head && node_color(x) == Color::BLACK)
    {
        if (x_parent == nullptr)
            break;

        if (x == x_parent->p_left)
        {
            Node<T> *w = x_parent->p_right;

            if (!w)
                break;

            if (node_color(w) == Color::RED)
            {
                w->m_color = Color::BLACK;
                x_parent->m_color = Color::RED;
                rotate_left(x_parent);
                w = x_parent->p_right;
                if (!w)
                    break;
            }

            if (node_color(w->p_left) == Color::BLACK && node_color(w->p_right) == Color::BLACK)
            {
                w->m_color = Color::RED;
                x = x_parent;
                x_parent = x->p_parent;
            }
            else
            {
                if (node_color(w->p_right) == Color::BLACK)
                {
                    if (w->p_left)
                        w->p_left->m_color = Color::BLACK;
                    w->m_color = Color::RED;
                    rotate_right(w);
                    w = x_parent->p_right;
                }

                w->m_color = x_parent->m_color;
                x_parent->m_color = Color::BLACK;
                if (w->p_right)
                    w->p_right->m_color = Color::BLACK;
                rotate_left(x_parent);
                x = this->p_head;
            }
        }
        else
        {
            Node<T> *w = x_parent->p_left;

            if (!w)
                break;

            if (node_color(w) == Color::RED)
            {
                w->m_color = Color::BLACK;
                x_parent->m_color = Color::RED;
                rotate_right(x_parent);
                w = x_parent->p_left;
                if (!w)
                    break;
            }

            if (node_color(w->p_left) == Color::BLACK && node_color(w->p_right) == Color::BLACK)
            {
                w->m_color = Color::RED;
                x = x_parent;
                x_parent = x->p_parent;
            }
            else
            {
                if (node_color(w->p_left) == Color::BLACK)
                {
                    if (w->p_right)
                        w->p_right->m_color = Color::BLACK;
                    w->m_color = Color::RED;
                    rotate_left(w);
                    w = x_parent->p_left;
                }

                w->m_color = x_parent->m_color;
                x_parent->m_color = Color::BLACK;
                if (w->p_left)
                    w->p_left->m_color = Color::BLACK;
                rotate_right(x_parent);
                x = this->p_head;
            }
        }
    }

    if (x)
        x->m_color = Color::BLACK;
}

template <typename T> void RBTree<T>::clear()
{
    this->destroy_subtree(this->p_head);
    this->p_head = nullptr;
}

template <typename T> int RBTree<T>::compute_black_height(const Node<T> *node) const
{
    if (!node)
        return 1;

    int left_bh = compute_black_height(node->p_left);
    int right_bh = compute_black_height(node->p_right);

    if (left_bh == -1 || right_bh == -1 || left_bh != right_bh)
        return -1;

    return left_bh + (node->m_color == Color::BLACK ? 1 : 0);
}

template <typename T> int RBTree<T>::get_black_height() const
{
    return compute_black_height(this->p_head);
}

template <typename T> bool RBTree<T>::validate_rb_helper(const Node<T> *node) const
{
    if (!node)
        return true;

    if (node->m_color == Color::RED)
    {
        if (node_color(node->p_left) == Color::RED || node_color(node->p_right) == Color::RED)
        {
            return false;
        }
    }

    if (node->p_left && node->p_left->p_parent != node)
        return false;
    if (node->p_right && node->p_right->p_parent != node)
        return false;

    return validate_rb_helper(node->p_left) && validate_rb_helper(node->p_right);
}

template <typename T> bool RBTree<T>::validate_rb_properties() const
{
    if (!this->p_head)
        return true;

    if (this->p_head->m_color != Color::BLACK)
        return false;

    if (!validate_rb_helper(this->p_head))
        return false;

    if (compute_black_height(this->p_head) == -1)
        return false;

    if (this->p_head->p_parent != nullptr)
        return false;

    return true;
}

template <typename T> std::vector<T> RBTree<T>::to_sorted_vector() const
{
    std::vector<T> result;
    std::stack<const Node<T> *> s;
    const Node<T> *curr = this->p_head;

    while (curr || !s.empty())
    {
        while (curr)
        {
            s.push(curr);
            curr = curr->p_left;
        }
        curr = s.top();
        s.pop();
        result.push_back(curr->m_data);
        curr = curr->p_right;
    }

    return result;
}

} // namespace stl_ext