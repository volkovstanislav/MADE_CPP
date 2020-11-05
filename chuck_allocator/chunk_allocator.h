#include <climits>
#include <cstddef>
#include <cstdint>

int const const_size = 4096;

namespace task {

  template<typename T>
  class Chunk_Allocator {
   private:
    struct chunk {
      chunk* next;
      T current_elem;
    };

    typedef char *new_pointer;
    typedef chunk *new_block;

    static inline new_block current;
    static inline new_block current_block;
    static inline new_block prev_block;
    static inline new_block free_block;

   public:
    using value_type = T;
    using pointer = T*;
    using const_pointer =	const T*;
    using reference =	T&;
    using const_reference =	const T&;
    using size_type	= std::size_t;
    using difference_type	= std::ptrdiff_t;

    struct rebind {
      typedef Chunk_Allocator<T> other;
    };

    Chunk_Allocator() {
      this->current = nullptr;
      this->current_block = nullptr;
      this->prev_block = nullptr;
      this->free_block = nullptr;
    }

    ~Chunk_Allocator() {
      new_block cur_ptr = current;
      while (cur_ptr != nullptr) {
        new_block pre_ptr = cur_ptr->next;
        if (cur_ptr) {
          delete cur_ptr;
          cur_ptr = nullptr;
        } else {
          cur_ptr = pre_ptr;
        }
      }
    }

    T* allocate(size_t n = 1) {
      if (free_block) {
        T* result = reinterpret_cast<T*>(free_block);
        free_block = free_block->next;
        return result;
      }
      else {
        if (current_block >= prev_block) {
          auto create_new_block = new_pointer(operator new(const_size));
          new_block(create_new_block)->next = current;
          current = new_block(create_new_block);

          new_pointer main_part = create_new_block + sizeof(new_block);
          auto result = uintptr_t(main_part);
          size_t main_pad = result -
              (result / std::alignment_of<chunk>::value *
              std::alignment_of<chunk>::value);
          current_block = new_block(main_part + main_pad);
          prev_block = new_block(create_new_block + const_size - sizeof(chunk));
        }
        return (T*)current_block++;
      }
    }

    void deallocate(T* ptr, size_t size = 1) {
      if (ptr != nullptr) {
        new_block(ptr)->next = free_block;
        free_block = new_block(ptr);
      }
    }

    template<typename... Args>
    void *construct(T* p, Args... args) {
      new (p)T(std::forward<Args>(args)...);
    }

    void destroy(T* p) {
      p->~T();
    }
  };
}
