#pragma once
#include <iterator>

namespace task {
template<class T, class Alloc = std::allocator<T>>
class list {
 private:
  struct Node {
    T value;
    Node *next;
    Node *prev;
  };

  Node *first;
  Node *cur;
  Node *last;
  size_t list_size;

 public:
  class iterator {
   private:
    Node *it;
   public:
    using difference_type = ptrdiff_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator() = default;
    iterator(const iterator &);
    iterator(list::Node *p);
    iterator &operator=(const iterator &);

    iterator &operator++();
    iterator operator++(int);
    reference operator*() const;
    pointer operator->() const;
    iterator &operator--();
    iterator operator--(int);

    bool operator==(iterator other) const;
    bool operator!=(iterator other) const;
  };

  class const_iterator {
   public:
    explicit const_iterator(Node *it = nullptr);
    const_iterator &operator=(const const_iterator &rhs);
    const_iterator &operator++();
    const_iterator operator++(int);
    const_iterator &operator--();
    const_iterator operator--(int);

    const T &operator*() const;
    bool operator!=(const const_iterator &rhs) const;

   private:
    operator iterator() const;
    friend class list;
    friend class iterator;
    Node *get_iterator() const {return it;}
    Node *it;
  };

  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  list();
  list(size_t count, const T &value, const Alloc &alloc = Alloc());
  explicit list(size_t count, const Alloc &alloc = Alloc());

  ~list();

  list(const list &other);
  list(list &&other);
  list &operator=(const list &other);
  list &operator=(list &&other);

  Alloc get_allocator() const;

  T &front();
  const T &front() const;

  T &back();
  const T &back() const;

  iterator begin();
  iterator end();

  const_iterator cbegin() const;
  const_iterator cend() const;

  reverse_iterator rbegin();
  reverse_iterator rend();

  const_reverse_iterator crbegin() const;
  const_reverse_iterator crend() const;

  [[nodiscard]] bool empty() const;
  [[nodiscard]] size_t size() const;
  [[nodiscard]] size_t max_size() const;
  void clear();

  iterator insert(const_iterator pos, const T &value);
  iterator insert(const_iterator pos, T &&value);
  iterator insert(const_iterator pos, size_t count, const T &value);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  void push_back(const T &value);
  void push_back(T &&value);
  void pop_back();

  void push_front(const T &value);
  void push_front(T &&value);
  void pop_front();

  template<class... Args>
  iterator emplace(const_iterator pos, Args &&... args);

  template<class... Args>
  void emplace_back(Args &&... args);

  template<class... Args>
  void emplace_front(Args &&... args);

  void resize(size_t count);
  void swap(list &other);

  void merge(list &other);
  void splice(const_iterator pos, list &other);
  void remove(const T &value);
  void reverse();
  void unique();
  void sort();

};

template<class T, class Alloc>
list<T, Alloc>::list(): first{nullptr}, cur{nullptr}, last{nullptr}, list_size{0} {}

template<class T, class Alloc>
list<T, Alloc>::list(size_t count, const Alloc &alloc):
    first{nullptr}, cur{nullptr}, last{nullptr}, list_size{0} {
  for (int i = {}; i < count; i++) {
    Node *new_node{new Node};
    if (!i) {
      first = new_node;
      cur = new_node;
      last = new_node;
      new_node->prev = nullptr;
      new_node->next = nullptr;
      new_node->value = T{};
    } else {
      cur->next = new_node;
      last = new_node;
      new_node->next = nullptr;
      new_node->prev = cur;
      new_node->value = T{};
      cur = new_node;
    }
  }
  list_size += count;
}

template<class T, class Alloc>
list<T, Alloc>::list(size_t count, const T &value, const Alloc &alloc):
    first{nullptr}, cur{nullptr}, last{nullptr}, list_size{} {
  for (int i{}; i < count; i++)
    push_back(value);
}

template<class T, class Alloc>
list<T, Alloc>::list(const list &other):
    first{nullptr}, cur{nullptr}, last{nullptr}, list_size{} {
  Node *new_node{other.first};
  while (new_node != nullptr) {
    push_back(new_node->value);
    new_node = new_node->next;
  }
}

template<class T, class Alloc>
list<T, Alloc>::list(list &&other) {
  list_size = other.list_size;
  first = other.first;
  cur = other.cur;
  last = other.last;
  other.list_size = 0;
  other.first = nullptr;
  other.cur = nullptr;
  other.last = nullptr;
}

template<class T, class Alloc>
list<T, Alloc> &list<T, Alloc>::operator=(const list &other) {
  auto copy = other;
  std::swap(*this, copy);
  return *this;
}

template<class T, class Alloc>
list<T, Alloc> &list<T, Alloc>::operator=(list &&other) {
  *this = std::move(other);
  return *this;
}

template<class T, class Alloc>
Alloc list<T, Alloc>::get_allocator() const {
  return Alloc();
}

template<class T, class Alloc>
T &list<T, Alloc>::front() {
  return this->first->value;
}

template<class T, class Alloc>
const T &list<T, Alloc>::front() const {
  return this->first->value;
}

template<class T, class Alloc>
T &list<T, Alloc>::back() {
  return this->last->value;
}

template<class T, class Alloc>
const T &list<T, Alloc>::back() const {
  return this->last->value;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::begin() {
  return iterator(first);
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::end() {
  if (!list_size) {
    return iterator();
  }
  return iterator(last);
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const {
  return const_iterator(first);
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const {
  if (!list_size) {
    return const_iterator();
  } else {
    return const_iterator(last);
  }
}

template<class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin() {
  return reverse_iterator(last);
}

template<class T, class Alloc>
typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend() {
  if (!list_size) {
    return reverse_iterator();
  } else {
    return reverse_iterator(first->prev);
  }
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const {
  return const_reverse_iterator(last);
}

template<class T, class Alloc>
typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const {
  if (!list_size) {
    return const_reverse_iterator();
  } else {
    return const_reverse_iterator(first->prev);
  }
}

template<class T, class Alloc>
bool list<T, Alloc>::empty() const {
  if (list_size == 0)
    return true;
  else
    return false;
}

template<class T, class Alloc>
size_t list<T, Alloc>::size() const {
  return list_size;
}

template<class T, class Alloc>
void list<T, Alloc>::clear() {
  while (first != nullptr) {
    Node *ptr {first};
    first = first->next;
    delete ptr;
  }
  first = nullptr;
  last = nullptr;
  list_size = 0;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(list::const_iterator pos, const T &value) {
  auto *p = pos.cur;
  auto *ptr = new list<T, Alloc>::Node{value, p->prev, p};
  p->prev->next = ptr;
  p->prev = ptr;
  typename list<T, Alloc>::iterator iter{ptr};
  list_size++;
  return iter;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(list::const_iterator pos, T &&value) {
  auto *p = pos.get_iterator();
  auto *ptr = new list<T, Alloc>::Node{std::move(value), p->prev, p};
  p->prev->next = ptr;
  p->prev = ptr;
  typename list<T, Alloc>::iterator iter{ptr};
  list_size++;
  return iter;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::insert(list::const_iterator pos, size_t count, const T &value) {
  size_t cnt = 0;
  while (cnt++ < count) {
    insert(pos, value);
  }
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(list::const_iterator pos) {
  const_iterator it{cbegin()};
  while (it != pos) {
    it++;
  }
  Node *cur_it{it.get_iterator()};
  if (cur_it == first && cur_it == last) {
    first = nullptr;
    cur = nullptr;
    last = nullptr;
    list_size--;
  } else if (cur_it == first) {
    Node *ptr_next_node{cur_it->next};
    delete cur_it;
    first = ptr_next_node;
    ptr_next_node->prev = nullptr;
    list_size--;
  } else if (cur_it == last) {
    return it;
  } else {
    Node *ptr_prev_node{cur_it->prev};
    Node *ptr_next_node{cur_it->next};
    ptr_prev_node->next = ptr_next_node;
    ptr_next_node->prev = ptr_prev_node;
  }
  return it;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::erase(list::const_iterator first, list::const_iterator last) {
  for (auto itr = first; itr != last;)
    itr = erase(itr);
  return last;
}

template<class T, class Alloc>
void list<T, Alloc>::push_front(T &&value) {
  Node *new_node{new Node};
  if (!list_size) {
    first = new_node;
    cur = new_node;
    last = new_node;
    new_node->prev = nullptr;
    new_node->next = nullptr;
    new_node->value = value;
  } else {
    first = new_node;
    new_node->next = nullptr;
    new_node->next = cur;
    new_node->value = value;
    cur = new_node;
  }
  list_size++;
}

template<class T, class Alloc>
void list<T, Alloc>::push_front(const T &value) {
  Node *new_node{new Node};
  if (!list_size) {
    first = new_node;
    cur = new_node;
    last = new_node;
    new_node->prev = nullptr;
    new_node->next = nullptr;
    new_node->value = value;
  } else {
    first = new_node;
    new_node->next = nullptr;
    new_node->next = cur;
    new_node->value = value;
    cur = new_node;
  }
  list_size++;
}

template<class T, class Alloc>
void list<T, Alloc>::push_back(T &&value) {
  Node *new_node{new Node};
  if (!list_size) {
    first = new_node;
    cur = new_node;
    last = new_node;
    new_node->prev = nullptr;
    new_node->next = nullptr;
    new_node->value = value;
  } else {
    cur->next = new_node;
    last = new_node;
    new_node->next = nullptr;
    new_node->prev = cur;
    new_node->value = value;
    cur = new_node;
  }
  list_size++;
}

template<class T, class Alloc>
void list<T, Alloc>::push_back(const T &value) {
  Node *new_node{new Node};
  if (!list_size) {
    first = new_node;
    cur = new_node;
    last = new_node;
    new_node->prev = nullptr;
    new_node->next = nullptr;
    new_node->value = value;
  } else {
    cur->next = new_node;
    last = new_node;
    new_node->next = nullptr;
    new_node->prev = cur;
    new_node->value = value;
    cur = new_node;
  }
  list_size++;
}

template<class T, class Alloc>
void list<T, Alloc>::pop_front() {
  erase(cbegin());
}

template<class T, class Alloc>
void list<T, Alloc>::pop_back() {
  erase(cend());
}

template<class T, class Alloc>
list<T, Alloc>::~list() {
  while (first != nullptr) {
    Node *new_node{first};
    first = first->next;
    delete new_node;
  }
}

template<class T, class Alloc>
void list<T, Alloc>::resize(size_t count) {
  if (list_size > count) {
    while (list_size != count) {
      pop_back();
    }
  } else {
    while (list_size != count) {
      const T insert_default_value;
      push_back(insert_default_value);
    }
  }
}

template<class T, class Alloc>
list<T, Alloc>::const_iterator::const_iterator(list::Node *it): it{it} {}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator &list<T, Alloc>::const_iterator::operator=(const list::const_iterator &rhs) {
  it = rhs.it;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator &list<T, Alloc>::const_iterator::operator++() {
  it = it->next;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator++(int) {
  const_iterator cit(*this);
  ++(*this);
  return cit;
}

template<class T, class Alloc>
typename list<T, Alloc>::const_iterator &list<T, Alloc>::const_iterator::operator--() {
  it = it->next;
  return *this;
}
template<class T, class Alloc>
typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator--(int) {
  const_iterator cit(*this);
  --(*this);
  return cit;
}

template<class T, class Alloc>
const T &list<T, Alloc>::const_iterator::operator*() const {
  return it->value;
}

template<class T, class Alloc>
bool list<T, Alloc>::const_iterator::operator!=(const list::const_iterator &rhs) const {
  return it != rhs.it;
}

template<class T, class Alloc>
list<T, Alloc>::const_iterator::operator iterator() const {}

template<class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator++() {
  it = it->next;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
  iterator cit(*this);
  ++(*this);
  return cit;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator--() {
  it = it->prev;
  return *this;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
  iterator cit(*this);
  --(*this);
  return cit;
}

template<class T, class Alloc>
T &list<T, Alloc>::iterator::operator*() const {
  return it->value;
}

template<class T, class Alloc>
bool list<T, Alloc>::iterator::operator==(list::iterator other) const {
  return it != other.it;
}

template<class T, class Alloc>
bool list<T, Alloc>::iterator::operator!=(list::iterator other) const {
  return it == other.it;
}

template<class T, class Alloc>
typename list<T, Alloc>::iterator &list<T, Alloc>::iterator::operator=(const list::iterator &other) {
  it = other.it;
  return *this;
}

template<class T, class Alloc>
list<T, Alloc>::iterator::iterator(const list::iterator &pos) {}

template<class T, class Alloc>
template<class... Args>
typename list<T, Alloc>::iterator list<T, Alloc>::emplace(list::const_iterator pos, Args &&... args) {
  list<T, Alloc>::const_iterator it = pos;
  it--;
  pos.cur->prev = new Node(std::forward<Args>(args)..., pos.cur, it.cur);
  if (!(it.cur == this->lend || it.cur == nullptr))
    it.cur->next = pos.cur->prev;
  else {
    this->first = pos.cur->prev;
    this->first->prev = nullptr;
  }
}

template<class T, class Alloc>
template<class... Args>
void list<T, Alloc>::emplace_back(Args &&... args) {
  if (list_size == 0) {
    this->first = new Node(std::forward<Args>(args)..., this->last, nullptr);
    this->ls->prev = this->first;
    list_size++;
  } else {
    Node *it = this->lend->prev_node;
    this->last->prev = new Node(std::forward<Args>(args)..., this->last, it);
    it->next = this->last->prev;
    list_size++;
  }
}

template<class T, class Alloc>
template<class... Args>
void list<T, Alloc>::emplace_front(Args &&... args) {
  if (list_size == 0) {
    this->first = new Node(std::forward<Args>(args)..., this->last, nullptr);
    this->last->prev = this->first;
    list_size++;
  } else {
    Node *it = this->first;
    this->first = new Node(std::forward<Args>(args)..., it, nullptr);
    it->prev = this->first;
    list_size++;
  }
}

template<class T, class Alloc>
void list<T, Alloc>::swap(list &other) {
  list<T, Alloc> *temp = this;
  this = other;
  other = temp;
}

template<class T, class Alloc>
void list<T, Alloc>::merge(list &other) {

}

template<class T, class Alloc>
void list<T, Alloc>::remove(const T &value) {
  for (auto it = begin(); it != end(); it++) {
    if (*it == value)
      erase(it);
  }
}

template<class T, class Alloc>
void list<T, Alloc>::reverse() {
  if (!empty()) {
    auto current_ptr = first;
    while (current_ptr != nullptr) {
      std::swap(current_ptr->next, current_ptr->prev);
      current_ptr = current_ptr->prev;
    }
  }
  std::swap(first, last);
}

template<class T, class Alloc>
void list<T, Alloc>::unique() {
  list<T, Alloc> new_unique_list;
  auto first_ptr = first;
  while (first_ptr) {
    if (first_ptr != first_ptr->next) {
      new_unique_list.push_back(first_ptr->value);
      first_ptr = first_ptr->next;
    }
  }
  this = new_unique_list;
}

template<class T, class Alloc>
size_t list<T, Alloc>::max_size() const {
  auto first_ptr = first;
  size_t max_val = first_ptr.value;
  while (first_ptr) {
    if (first_ptr->value < first_ptr->next->value) {
      max_val = first_ptr->next->value;
      first_ptr = first_ptr->next;
    }
  }
  return max_val;
}

}
