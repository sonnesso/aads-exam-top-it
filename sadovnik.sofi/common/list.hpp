#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace sadovnik
{

  template< class T >
  class List;

  template< class T >
  class LCIter;

  namespace detail
  {

    template< class T >
    struct ListNode
    {
      T data_;
      ListNode * prev_;
      ListNode * next_;

      explicit ListNode(const T & value)
        : data_(value),
          prev_(nullptr),
          next_(nullptr)
      {
      }

      explicit ListNode(T && value)
        : data_(std::move(value)),
          prev_(nullptr),
          next_(nullptr)
      {
      }
    };

  }

  template< class T >
  class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    LIter()
      : cur_(nullptr)
    {
    }

    reference operator*() const
    {
      return cur_->data_;
    }

    pointer operator->() const
    {
      return &cur_->data_;
    }

    LIter & operator++()
    {
      cur_ = cur_->next_;
      return *this;
    }

    LIter operator++(int)
    {
      LIter tmp = *this;
      cur_ = cur_->next_;
      return tmp;
    }

    LIter & operator--()
    {
      cur_ = cur_->prev_;
      return *this;
    }

    LIter operator--(int)
    {
      LIter tmp = *this;
      cur_ = cur_->prev_;
      return tmp;
    }

    bool operator==(const LIter & other) const
    {
      return cur_ == other.cur_;
    }

    bool operator!=(const LIter & other) const
    {
      return cur_ != other.cur_;
    }

  private:
    explicit LIter(detail::ListNode< T > * node)
      : cur_(node)
    {
    }

    detail::ListNode< T > * cur_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T *;
    using reference = const T &;

    LCIter()
      : cur_(nullptr)
    {
    }

    LCIter(const LIter< T > & it)
      : cur_(it.cur_)
    {
    }

    reference operator*() const
    {
      return cur_->data_;
    }

    pointer operator->() const
    {
      return &cur_->data_;
    }

    LCIter & operator++()
    {
      cur_ = cur_->next_;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter tmp = *this;
      cur_ = cur_->next_;
      return tmp;
    }

    LCIter & operator--()
    {
      cur_ = cur_->prev_;
      return *this;
    }

    LCIter operator--(int)
    {
      LCIter tmp = *this;
      cur_ = cur_->prev_;
      return tmp;
    }

    bool operator==(const LCIter & other) const
    {
      return cur_ == other.cur_;
    }

    bool operator!=(const LCIter & other) const
    {
      return cur_ != other.cur_;
    }

  private:
    explicit LCIter(const detail::ListNode< T > * node)
      : cur_(node)
    {
    }

    const detail::ListNode< T > * cur_;
  };

  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List()
      : head_(nullptr),
        tail_(nullptr),
        size_(0)
    {
    }

    List(const List & other)
      : head_(nullptr),
        tail_(nullptr),
        size_(0)
    {
      try
      {
        for (LCIter< T > it = other.cbegin(); it != other.cend(); ++it)
        {
          pushBack(*it);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

    List(List && other) noexcept
      : head_(other.head_),
        tail_(other.tail_),
        size_(other.size_)
    {
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    List & operator=(const List & other)
    {
      if (this != &other)
      {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List & operator=(List && other) noexcept
    {
      if (this != &other)
      {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    ~List()
    {
      clear();
    }

    iterator begin() noexcept
    {
      return iterator(head_);
    }

    iterator end() noexcept
    {
      return iterator(nullptr);
    }

    const_iterator begin() const noexcept
    {
      return const_iterator(head_);
    }

    const_iterator end() const noexcept
    {
      return const_iterator(nullptr);
    }

    const_iterator cbegin() const noexcept
    {
      return const_iterator(head_);
    }

    const_iterator cend() const noexcept
    {
      return const_iterator(nullptr);
    }

    void pushBack(const T & value)
    {
      detail::ListNode< T > * node = new detail::ListNode< T >(value);
      linkBack(node);
    }

    void pushBack(T && value)
    {
      detail::ListNode< T > * node =
        new detail::ListNode< T >(std::move(value));
      linkBack(node);
    }

    void clear()
    {
      while (head_ != nullptr)
      {
        detail::ListNode< T > * next = head_->next_;
        delete head_;
        head_ = next;
      }
      tail_ = nullptr;
      size_ = 0;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    void swap(List & other) noexcept
    {
      detail::ListNode< T > * tmpHead = head_;
      detail::ListNode< T > * tmpTail = tail_;
      std::size_t tmpSize = size_;
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = tmpHead;
      other.tail_ = tmpTail;
      other.size_ = tmpSize;
    }

  private:
    detail::ListNode< T > * head_;
    detail::ListNode< T > * tail_;
    std::size_t size_;

    void linkBack(detail::ListNode< T > * node)
    {
      node->prev_ = tail_;
      if (tail_ != nullptr)
      {
        tail_->next_ = node;
      }
      else
      {
        head_ = node;
      }
      tail_ = node;
      ++size_;
    }
  };

}

#endif
