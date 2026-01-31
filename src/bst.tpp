#include <algorithm>
#include <vector>

namespace stl_ext
{
template <typename T> void BST<T>::insert(const T &val)
{
    if (!this->p_head)
    {
        this->p_head = BinaryTree<T>::make_node(val);
        return;
    }
    insert_iterative(this->p_head.get(), val);
}

template <typename T> void BST<T>::insert_iterative(Node<T> *node, const T &val)
{
    Node<T> *curr = node;
    std::vector<Node<T> *> path;

    while (true)
    {
        path.push_back(curr);

        if (val <= curr->get_data())
        {
            if (!curr->get_left())
            {
                curr->set_left(BinaryTree<T>::make_node(val));
                break;
            }
            curr = curr->get_left();
        }
        else
        {
            if (!curr->get_right())
            {
                curr->set_right(BinaryTree<T>::make_node(val));
                break;
            }
            curr = curr->get_right();
        }
    }
    for (auto it = path.rbegin(); it != path.rend(); ++it)
    {
        Node<T> *ancestor = *it;
        int h_left = ancestor->get_left() ? ancestor->get_left()->get_height_val() : 0;
        int h_right = ancestor->get_right() ? ancestor->get_right()->get_height_val() : 0;

        ancestor->set_height_val(1 + std::max(h_left, h_right));
    }
}

template <typename T> inline std::unique_ptr<Node<T>> BST<T>::remove_impl(std::unique_ptr<Node<T>> node, const T &val)
{
    if (!node)
        return nullptr;

    if (val < node->m_data)
    {
        node->p_left = remove_impl(std::move(node->p_left), val);
    }
    else if (val > node->m_data)
    {
        node->p_right = remove_impl(std::move(node->p_right), val);
    }
    else
    {
        if (!node->p_left)
            return std::move(node->p_right);

        if (!node->p_right)
            return std::move(node->p_left);

        Node<T> *temp = node->p_right.get();
        while (temp->p_left)
        {
            temp = temp->p_left.get();
        }

        node->m_data = temp->m_data;

        node->p_right = remove_impl(std::move(node->p_right), temp->m_data);
    }
    return node;
}

template <typename T> bool BST<T>::contains(const T &val) const
{
    Node<T> *curr = this->p_head.get();
    while (curr)
    {
        if (val == curr->get_data())
            return true;
        if (val < curr->get_data())
            curr = curr->get_left();
        else
            curr = curr->get_right();
    }
    return false;
}

template <typename T> void BST<T>::remove(const T &val)
{
    this->p_head = remove_impl(std::move(this->p_head), val);
}

template <typename T> T BST<T>::get_min() const
{
    Node<T> *r = this->p_head.get();
    if (!r)
        throw std::runtime_error("Empty");
    while (r->get_left())
        r = r->get_left();
    return r->get_data();
}

template <typename T> T BST<T>::get_max() const
{
    Node<T> *r = this->p_head.get();
    if (!r)
        throw std::runtime_error("Empty");
    while (r->get_right())
        r = r->get_right();
    return r->get_data();
}

template <typename T> T BST<T>::get_successor(const T &val) const
{
    Node<T> *curr = this->get_root();
    Node<T> *ancestor_succ = nullptr;

    while (curr)
    {
        if (val < curr->get_data())
        {
            ancestor_succ = curr;
            curr = curr->get_left();
        }
        else if (val > curr->get_data())
        {
            curr = curr->get_right();
        }
        else
        {
            if (curr->get_right())
            {
                Node<T> *temp = curr->get_right();
                while (temp->get_left())
                    temp = temp->get_left();
                return temp->get_data();
            }

            if (ancestor_succ)
                return ancestor_succ->get_data();

            throw std::runtime_error("No successor");
        }
    }

    throw std::runtime_error("Value not found");
}

template <typename T> T BST<T>::get_predecessor(const T &val) const
{
    Node<T> *curr = this->get_root();
    Node<T> *ancestor_pred = nullptr;

    while (curr)
    {
        if (val < curr->get_data())
        {
            curr = curr->get_left();
        }
        else if (val > curr->get_data())
        {
            ancestor_pred = curr;
            curr = curr->get_right();
        }
        else
        {
            if (curr->get_left())
            {
                Node<T> *temp = curr->get_left();
                while (temp->get_right())
                    temp = temp->get_right();
                return temp->get_data();
            }

            if (ancestor_pred)
                return ancestor_pred->get_data();

            throw std::runtime_error("No predecessor");
        }
    }
    throw std::runtime_error("Value not found");
}

} // namespace stl_ext