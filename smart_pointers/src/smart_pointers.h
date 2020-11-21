namespace task {

template <class T>
class UniquePtr {
 private:
  T *pointer;

 public:
  explicit UniquePtr(T*);
  UniquePtr();
  UniquePtr(const UniquePtr &other) = delete;
  UniquePtr(UniquePtr &&other) noexcept;

  void reset();
  T *release();
  T *get() const;
  T *swap(T *other);
  void replace(T *other);
  void swap(UniquePtr &other);

  ~UniquePtr();

  UniquePtr &operator=(const UniquePtr &other) = delete;
  UniquePtr &operator=(UniquePtr &&other) noexcept;

  T *operator->();
  T &operator*() const;
};

template <class T>
class SharedPtr {
 public:
  struct SharedCount {
    long *p;
    SharedCount();
    SharedCount(const SharedCount &count);
    void swap(SharedCount &other) noexcept;
    long use_count() const noexcept;
    void master(T *pnt);
    void free(T *pnt) noexcept;
  };

  SharedPtr() noexcept;
  explicit SharedPtr(T *p);
  SharedPtr(const SharedPtr<T> &ptr, T *p);
  SharedPtr(const SharedPtr &ptr) noexcept;

  SharedPtr& operator=(SharedPtr ptr) noexcept;
  T &operator*() const noexcept;
  T *operator->() const noexcept;
  T *get() const noexcept;

  ~SharedPtr();

  void reset(T *p);
  void reset() noexcept;
  int use_count() const noexcept;
  void swap(SharedPtr &other) noexcept;
  explicit operator bool() const noexcept;

 private:
  T *pointer;
  SharedCount cnt;

  void master(T *p) {
    cnt.master(p);
    pointer = p;
  }

  void release() noexcept {
    cnt.free(pointer);
    pointer = NULL;
  }
};

template <class T>
class WeakPtr {
  // Your code goes here...
};

template<class T>
UniquePtr<T>::UniquePtr(T* p): pointer(p) {}

template<class T>
UniquePtr<T>::UniquePtr(): pointer(nullptr) {}

template<class T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer(other.pointer){
  other.pointer = nullptr;
}

template<class T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
  if(this != &other) {
    reset();
    pointer = other.pointer;
    other.pointer = nullptr;
  }
  return *this;
}

template<class T>
T* UniquePtr<T>::operator->() {
  return pointer;
}

template<class T>
T& UniquePtr<T>::operator*() const {
  return (*pointer);
}

template<class T>
T* UniquePtr<T>::release() {
  T* temp = pointer;
  pointer = nullptr;
  return temp;
}

template<class T>
void UniquePtr<T>::reset() {
  pointer = nullptr;
}

template<class T>
T* UniquePtr<T>::swap(T* other) {
  T* temp = pointer;
  pointer = other;
  return temp;
}

template<class T>
void UniquePtr<T>::swap(UniquePtr& other) {
  T* temp = pointer;
  pointer = other.pointer;
  other.pointer = temp;
}

template<class T>
void UniquePtr<T>::replace(T *other) {
  reset();
  pointer = other;
}

template<class T>
T *UniquePtr<T>::get() const {
  return pointer;
}

template<class T>
UniquePtr<T>::~UniquePtr() {
  delete pointer;
}

template<class T>
void SharedPtr<T>::SharedCount::master(T *pnt){
  if (pnt) {
    if (!p) p = new long(1);
    else ++(*p);
  }
}

template<class T>
void SharedPtr<T>::SharedCount::free(T *point) noexcept {
  if (p) {
    --(*p);
    if (*p == 0) {
      delete point;
      delete p;
    }
    p = nullptr;
  }
}

template<class T>
long SharedPtr<T>::SharedCount::use_count() const noexcept  {
  long kCnt = 0;
  if (p) kCnt = *p;
  return kCnt;
}

template<class T>
void SharedPtr<T>::SharedCount::swap(SharedPtr::SharedCount &other) noexcept {
  std::swap(p, other.p);
}

template<class T>
SharedPtr<T>::SharedCount::SharedCount(): p(nullptr) {}

template<class T>
SharedPtr<T>::SharedCount::SharedCount(const SharedPtr::SharedCount &count): p(count.p) {}

template<class T>
SharedPtr<T>::SharedPtr() noexcept: pointer(nullptr), cnt() {}

template<class T>
SharedPtr<T>::SharedPtr(T *p): cnt() {
    master(p);
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &ptr, T *p): cnt(ptr.pn) {
  master(p);
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr &ptr) noexcept: cnt(ptr.cnt) {
  master(ptr.pointer);
}

template<class T>
SharedPtr<T> &SharedPtr<T>::operator=(SharedPtr ptr) noexcept {
  swap(ptr);
  return *this;
}

template<class T>
SharedPtr<T>::~SharedPtr() {
  release();
}

template<class T>
void SharedPtr<T>::reset() noexcept  {
  release();
}

template<class T>
void SharedPtr<T>::reset(T *p) {
  release();
  master(p);
}

template<class T>
void SharedPtr<T>::swap(SharedPtr &other) noexcept {
  std::swap(pointer, other.pointer);
  cnt.swap(other.cnt);
}

template<class T>
SharedPtr<T>::operator bool() const noexcept {
  return cnt.use_count() > 0;
}

template<class T>
int SharedPtr<T>::use_count() const noexcept {
  return cnt.use_count();
}

template<class T>
T &SharedPtr<T>::operator*() const noexcept  {
  return *pointer;
}

template<class T>
T *SharedPtr<T>::operator->() const noexcept {
  return pointer;
}

template<class T>
T *SharedPtr<T>::get() const noexcept {
  return pointer;
}

}  // namespace task
