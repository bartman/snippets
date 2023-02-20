/* using CppInsights on https://godbolt.org/z/6WjGcn */

#include <coroutine>
#include <memory>
#include <iostream>

template<typename T>
struct Generator
{
  struct promise_type;
  using handle_type = std::coroutine_handle<promise_type>;
  inline Generator(handle_type h)
  : coro{h}
  {
    std::operator<<(std::cout, "        Generator::Generator").operator<<(std::endl);
  }
  
  handle_type coro;
  std::shared_ptr<T> value;
  inline ~Generator()
  {
    std::operator<<(std::cout, "        Generator::~Generator").operator<<(std::endl);
    if(this->coro) {
      this->coro.destroy();
    } 
    
  }
  
  // inline Generator(const Generator<T> &) = delete;
  // inline Generator<T> & operator=(const Generator<T> &) = delete;
  inline Generator(Generator<T> && oth)
  : coro{oth.coro}
  {
    oth.coro = nullptr;
  }
  
  inline Generator<T> & operator=(Generator<T> && oth)
  {
    this->coro = oth.coro;
    oth.coro = nullptr;
    return *this;
  }
  
  inline int getNextValue()
  {
    std::operator<<(std::cout, "        Generator::getNextValue").operator<<(std::endl);
    this->coro.resume();
    return this->coro.promise().current_value;
  }
  
  struct promise_type
  {
    inline promise_type()
    {
      std::operator<<(std::cout, "            promise_type::promise_type").operator<<(std::endl);
    }
    
    inline ~promise_type()
    {
      std::operator<<(std::cout, "            promise_type::~promise_type").operator<<(std::endl);
    }
    
    inline std::suspend_always initial_suspend()
    {
      std::operator<<(std::cout, "            promise_type::initial_suspend").operator<<(std::endl);
      return {};
    }
    
    inline std::suspend_always final_suspend() noexcept
    {
      std::operator<<(std::cout, "            promise_type::final_suspend").operator<<(std::endl);
      return {};
    }
    
    inline auto get_return_object()
    {
      std::operator<<(std::cout, "            promise_type::get_return_object").operator<<(std::endl);
      return Generator<T>{{handle_type::from_promise(*this)}};
    }
    
    inline std::suspend_always yield_value(int value)
    {
      std::operator<<(std::cout, "            promise_type::yield_value").operator<<(std::endl);
      this->current_value = value;
      return {};
    }
    
    inline void unhandled_exception()
    {
      exit(1);
    }
    
    T current_value;
  };
  
};

/* First instantiated from: insights.cpp:74 */
#ifdef INSIGHTS_USE_TEMPLATE
template<>
struct Generator<int>
{
  struct promise_type
  {
    inline promise_type()
    {
      std::operator<<(std::cout, "            promise_type::promise_type").operator<<(std::endl);
    }
    
    inline ~promise_type() noexcept
    {
      std::operator<<(std::cout, "            promise_type::~promise_type").operator<<(std::endl);
    }
    
    inline std::suspend_always initial_suspend()
    {
      std::operator<<(std::cout, "            promise_type::initial_suspend").operator<<(std::endl);
      return {};
    }
    
    inline std::suspend_always final_suspend() noexcept
    {
      std::operator<<(std::cout, "            promise_type::final_suspend").operator<<(std::endl);
      return {};
    }
    
    inline Generator<int> get_return_object()
    {
      std::operator<<(std::cout, "            promise_type::get_return_object").operator<<(std::endl);
      return Generator<int>{std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    
    inline std::suspend_always yield_value(int value)
    {
      std::operator<<(std::cout, "            promise_type::yield_value").operator<<(std::endl);
      this->current_value = value;
      return {};
    }
    
    inline void unhandled_exception()
    {
      exit(1);
    }
    
    int current_value;
  };
  
  using handle_type = std::coroutine_handle<promise_type>;
  inline Generator(std::coroutine_handle<promise_type> h)
  : coro{std::coroutine_handle<promise_type>(h)}
  , value{std::shared_ptr<int>()}
  {
    std::operator<<(std::cout, "        Generator::Generator").operator<<(std::endl);
  }
  
  std::coroutine_handle<promise_type> coro;
  std::shared_ptr<int> value;
  inline ~Generator() noexcept
  {
    std::operator<<(std::cout, "        Generator::~Generator").operator<<(std::endl);
    if(static_cast<bool>(this->coro.operator bool())) {
      this->coro.destroy();
    } 
    
  }
  
  // inline Generator(const Generator<int> &) = delete;
  // inline Generator<int> & operator=(const Generator<int> &) = delete;
  inline Generator(Generator<int> && oth);
  
  inline Generator<int> & operator=(Generator<int> && oth);
  
  inline int getNextValue()
  {
    std::operator<<(std::cout, "        Generator::getNextValue").operator<<(std::endl);
    this->coro.resume();
    return this->coro.promise().current_value;
  }
  
  struct promise_type;
};

#endif


Generator<int> getNext(int start, int step)
{
  std::operator<<(std::cout, "    getNext: start").operator<<(std::endl);
  int value = start;
  for(int i = 0; ; ++i) {
    std::operator<<(std::cout, "    getNext: before co_yield").operator<<(std::endl);
    co_yield value;
    std::operator<<(std::cout, "    getNext: after co_yield").operator<<(std::endl);
    value = static_cast<int>(value + step);
  }
  
}


int main()
{
  Generator<int> gen = getNext(10, 10);
  for(int i = 0; i <= 3; ++i) {
    int val = gen.getNextValue();
    std::operator<<(std::cout, "main: ").operator<<(val).operator<<(std::endl);
  }
  
  return 0;
}

