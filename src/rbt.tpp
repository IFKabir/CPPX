#include <algorithm>
#include <stack>

namespace stl_ext
{

template <typename T> bool RBTree<T>::is_red(const Node<T> *node)
{
    return node != nullptr && node->get_color() == Color::RED;
}

template <typename T> bool RBTree<T>::is_black(const Node<T> *node)
{
    return node == nullptr || node->get_color() == Color::BLACK;
}

template <typename T> Color RBTree<T>::get_color(const Node<T> *node)
{
    return (node == nullptr) ? Color::BLACK : node->get_color();
}

template <typename T> Color RBTree<T>::node_color(const Node<T> *node) const
{
    return (node == nullptr) ? Color::BLACK : node->get_color();
}

template <typename T> Node<T> *RBTree<T>::tree_minimum(Node<T> *node) const
{
    while (node->get_left())
        node = node->get_left();
    return node;
}

template <typename T> Node<T> *RBTree<T>::release_child(Node<T> *parent, Node<T> *child)
{
    (void)parent;
    (void)child;

    return child;
}

template <typename T> void RBTree<T>::attach_child(Node<T> *parent, std::unique_ptr<Node<T>> child, bool as_left)
{
    if (child)
        child->p_parent = parent;

    if (!parent)
    {
        this->p_head = std::move(child);
    }
    else if (as_left)
    {
        parent->p_left = std::move(child);
    }
    else
    {
        parent->p_right = std::move(child);
    }
}

template <typename T> void RBTree<T>::rotate_left(Node<T> *x)
{
    Node<T> *p = x->p_parent;
    bool x_is_left = (p && p->get_left() == x);

    std::unique_ptr<Node<T>> y_owned = x->detach_right();
    Node<T> *y = y_owned.get();

    std::unique_ptr<Node<T>> beta = y->detach_left();
    if (beta)
        beta->p_parent = x;
    x->p_right = std::move(beta);

    std::unique_ptr<Node<T>> x_owned;
    if (!p)
    {
        x_owned = std::move(this->p_head);
    }
    else if (x_is_left)
    {
        x_owned = p->detach_left();
    }
    else
    {
        x_owned = p->detach_right();
    }

    x->p_parent = y;
    y->p_left = std::move(x_owned);

    y->p_parent = p;
    if (!p)
    {
        this->p_head = std::move(y_owned);
    }
    else if (x_is_left)
    {
        p->p_left = std::move(y_owned);
    }
    else
    {
        p->p_right = std::move(y_owned);
    }
}

template <typename T> void RBTree<T>::rotate_right(Node<T> *x)
{
    Node<T> *p = x->p_parent;
    bool x_is_left = (p && p->get_left() == x);

    std::unique_ptr<Node<T>> y_owned = x->detach_left();
    Node<T> *y = y_owned.get();

    std::unique_ptr<Node<T>> beta = y->detach_right();
    if (beta)
        beta->p_parent = x;
    x->p_left = std::move(beta);

    std::unique_ptr<Node<T>> x_owned;
    if (!p)
    {
        x_owned = std::move(this->p_head);
    }
    else if (x_is_left)
    {
        x_owned = p->detach_left();
    }
    else
    {
        x_owned = p->detach_right();
    }

    x->p_parent = y;
    y->p_right = std::move(x_owned);

    y->p_parent = p;
    if (!p)
    {
        this->p_head = std::move(y_owned);
    }
    else if (x_is_left)
    {
        p->p_left = std::move(y_owned);
    }
    else
    {
        p->p_right = std::move(y_owned);
    }
}

template <typename T> void RBTree<T>::transplant(Node<T> *u, Node<T> *v)
{
    Node<T> *up = u->p_parent;

    std::unique_ptr<Node<T>> v_owned;
    if (v)
    {
        Node<T> *vp = v->p_parent;
        if (vp)
        {
            if (vp->get_left() == v)
                v_owned = vp->detach_left();
            else
                v_owned = vp->detach_right();
        }
    }

    std::unique_ptr<Node<T>> u_owned;
    if (!up)
    {
        u_owned = std::move(this->p_head);
    }
    else if (up->get_left() == u)
    {
        u_owned = up->detach_left();
    }
    else
    {
        u_owned = up->detach_right();
    }

    if (v)
        v->p_parent = up;

    if (!up)
    {
        if (v_owned)
            this->p_head = std::move(v_owned);
        else
            this->p_head = nullptr;
    }
    else if (!u_owned || up->get_left() == nullptr)
    {
    }

    (void)u_owned;
}

template <typename T> void RBTree<T>::insert(const T &val)
{

    auto new_node = std::make_unique<Node<T>>(val);
    new_node->m_color = Color::RED;
    Node<T> *z = new_node.get();

    if (!this->p_head)
    {
        z->m_color = Color::BLACK;
        z->p_parent = nullptr;
        this->p_head = std::move(new_node);
        return;
    }

    Node<T> *curr = this->p_head.get();
    Node<T> *parent = nullptr;

    while (curr)
    {
        parent = curr;
        if (val < curr->m_data)
        {
            curr = curr->get_left();
        }
        else if (val > curr->m_data)
        {
            curr = curr->get_right();
        }
        else
        {

            return;
        }
    }

    z->p_parent = parent;
    if (val < parent->m_data)
    {
        parent->p_left = std::move(new_node);
    }
    else
    {
        parent->p_right = std::move(new_node);
    }

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

        if (parent == grandparent->get_left())
        {
            Node<T> *uncle = grandparent->get_right();

            if (node_color(uncle) == Color::RED)
            {

                parent->m_color = Color::BLACK;
                uncle->m_color = Color::BLACK;
                grandparent->m_color = Color::RED;
                z = grandparent;
            }
            else
            {
                if (z == parent->get_right())
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

            Node<T> *uncle = grandparent->get_left();

            if (node_color(uncle) == Color::RED)
            {

                parent->m_color = Color::BLACK;
                uncle->m_color = Color::BLACK;
                grandparent->m_color = Color::RED;
                z = grandparent;
            }
            else
            {
                if (z == parent->get_left())
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

    Node<T> *z = this->p_head.get();
    while (z)
    {
        if (val < z->m_data)
            z = z->get_left();
        else if (val > z->m_data)
            z = z->get_right();
        else
            break;
    }
    if (!z)
        return;

    Node<T> *y = z;
    Color y_original_color = y->m_color;
    Node<T> *x = nullptr;
    Node<T> *x_parent = nullptr;

    if (!z->get_left())
    {

        x = z->get_right();
        x_parent = z->p_parent;

        Node<T> *zp = z->p_parent;
        std::unique_ptr<Node<T>> x_owned = z->detach_right();
        if (x)
            x->p_parent = zp;

        if (!zp)
        {

            this->p_head.reset();
            this->p_head = std::move(x_owned);
        }
        else if (zp->get_left() == z)
        {
            zp->p_left.reset();
            zp->p_left = std::move(x_owned);
        }
        else
        {
            zp->p_right.reset();
            zp->p_right = std::move(x_owned);
        }
    }
    else if (!z->get_right())
    {

        x = z->get_left();
        x_parent = z->p_parent;

        Node<T> *zp = z->p_parent;
        std::unique_ptr<Node<T>> x_owned = z->detach_left();
        if (x)
            x->p_parent = zp;

        if (!zp)
        {
            this->p_head.reset();
            this->p_head = std::move(x_owned);
        }
        else if (zp->get_left() == z)
        {
            zp->p_left.reset();
            zp->p_left = std::move(x_owned);
        }
        else
        {
            zp->p_right.reset();
            zp->p_right = std::move(x_owned);
        }
    }
    else
    {

        y = tree_minimum(z->get_right());
        y_original_color = y->m_color;
        x = y->get_right();

        if (y->p_parent == z)
        {

            x_parent = z;

            z->m_data = y->m_data;

            std::unique_ptr<Node<T>> x_owned = y->detach_right();
            if (x)
                x->p_parent = z;

            z->p_right.reset();
            z->p_right = std::move(x_owned);
        }
        else
        {

            x_parent = y->p_parent;

            z->m_data = y->m_data;

            Node<T> *yp = y->p_parent;
            std::unique_ptr<Node<T>> x_owned = y->detach_right();
            if (x)
                x->p_parent = yp;

            if (yp->get_left() == y)
            {
                yp->p_left.reset();
                yp->p_left = std::move(x_owned);
            }
            else
            {
                yp->p_right.reset();
                yp->p_right = std::move(x_owned);
            }
        }
    }

    if (y_original_color == Color::BLACK)
    {
        fix_delete_violation(x, x_parent);
    }
}

template <typename T> void RBTree<T>::fix_delete_violation(Node<T> *x, Node<T> *x_parent)
{
    while (x != this->p_head.get() && node_color(x) == Color::BLACK)
    {
        if (x_parent == nullptr)
            break;

        if (x == x_parent->get_left())
        {
            Node<T> *w = x_parent->get_right();

            if (!w)
                break;

            if (node_color(w) == Color::RED)
            {
                w->m_color = Color::BLACK;
                x_parent->m_color = Color::RED;
                rotate_left(x_parent);
                w = x_parent->get_right();
                if (!w)
                    break;
            }

            if (node_color(w->get_left()) == Color::BLACK && node_color(w->get_right()) == Color::BLACK)
            {
                w->m_color = Color::RED;
                x = x_parent;
                x_parent = x->p_parent;
            }
            else
            {

                if (node_color(w->get_right()) == Color::BLACK)
                {
                    if (w->get_left())
                        w->get_left()->m_color = Color::BLACK;
                    w->m_color = Color::RED;
                    rotate_right(w);
                    w = x_parent->get_right();
                }

                w->m_color = x_parent->m_color;
                x_parent->m_color = Color::BLACK;
                if (w->get_right())
                    w->get_right()->m_color = Color::BLACK;
                rotate_left(x_parent);
                x = this->p_head.get();
            }
        }
        else
        {

            Node<T> *w = x_parent->get_left();

            if (!w)
                break;

            if (node_color(w) == Color::RED)
            {
                w->m_color = Color::BLACK;
                x_parent->m_color = Color::RED;
                rotate_right(x_parent);
                w = x_parent->get_left();
                if (!w)
                    break;
            }

            if (node_color(w->get_left()) == Color::BLACK && node_color(w->get_right()) == Color::BLACK)
            {
                w->m_color = Color::RED;
                x = x_parent;
                x_parent = x->p_parent;
            }
            else
            {

                if (node_color(w->get_left()) == Color::BLACK)
                {
                    if (w->get_right())
                        w->get_right()->m_color = Color::BLACK;
                    w->m_color = Color::RED;
                    rotate_left(w);
                    w = x_parent->get_left();
                }

                w->m_color = x_parent->m_color;
                x_parent->m_color = Color::BLACK;
                if (w->get_left())
                    w->get_left()->m_color = Color::BLACK;
                rotate_right(x_parent);
                x = this->p_head.get();
            }
        }
    }

    if (x)
        x->m_color = Color::BLACK;
}

template <typename T> void RBTree<T>::clear()
{
    this->p_head.reset();
}

template <typename T> int RBTree<T>::compute_black_height(const Node<T> *node) const
{
    if (!node)
        return 1;

    int left_bh = compute_black_height(node->get_left());
    int right_bh = compute_black_height(node->get_right());

    if (left_bh == -1 || right_bh == -1 || left_bh != right_bh)
        return -1;

    return left_bh + (node->m_color == Color::BLACK ? 1 : 0);
}

template <typename T> int RBTree<T>::get_black_height() const
{
    return compute_black_height(this->p_head.get());
}

template <typename T> bool RBTree<T>::validate_rb_helper(const Node<T> *node) const
{
    if (!node)
        return true;

    if (node->m_color == Color::RED)
    {
        if (node_color(node->get_left()) == Color::RED || node_color(node->get_right()) == Color::RED)
        {
            return false;
        }
    }

    if (node->get_left() && node->get_left()->p_parent != node)
        return false;
    if (node->get_right() && node->get_right()->p_parent != node)
        return false;

    return validate_rb_helper(node->get_left()) && validate_rb_helper(node->get_right());
}

template <typename T> bool RBTree<T>::validate_rb_properties() const
{
    if (!this->p_head)
        return true;

    if (this->p_head->m_color != Color::BLACK)
        return false;

    if (!validate_rb_helper(this->p_head.get()))
        return false;

    if (compute_black_height(this->p_head.get()) == -1)
        return false;

    if (this->p_head->p_parent != nullptr)
        return false;

    return true;
}

template <typename T> std::vector<T> RBTree<T>::to_sorted_vector() const
{
    std::vector<T> result;
    std::stack<const Node<T> *> s;
    const Node<T> *curr = this->p_head.get();

    while (curr || !s.empty())
    {
        while (curr)
        {
            s.push(curr);
            curr = curr->get_left();
        }
        curr = s.top();
        s.pop();
        result.push_back(curr->get_data());
        curr = curr->get_right();
    }

    return result;
}

} // namespace stl_ext
