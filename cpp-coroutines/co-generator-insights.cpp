/* using CppInsights + coroutine transformations on https://godbolt.org/z/6WjGcn */

/*************************************************************************************
 * NOTE: The coroutine transformation you've enabled is a hand coded transformation! *
 *       Most of it is _not_ present in the AST. What you see is an approximation.   *
 *************************************************************************************/
// infiniteDataStream.cpp

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



struct __getNextFrame
{
  void (*resume_fn)(__getNextFrame *);
  void (*destroy_fn)(__getNextFrame *);
  std::__coroutine_traits_impl<Generator<int> >::promise_type __promise;
  int __suspend_index;
  bool __initial_await_suspend_called;
  int start;
  int step;
  int value;
  int i;
  std::suspend_always __suspend_74_16;
  std::suspend_always __suspend_79_9;
  std::suspend_always __suspend_74_16_1;
};

Generator<int> getNext(int start, int step)
{
  /* Allocate the frame including the promise */
  __getNextFrame * __f = reinterpret_cast<__getNextFrame *>(operator new(__builtin_coro_size()));
  __f->__suspend_index = 0;
  __f->__initial_await_suspend_called = false;
  __f->start = std::forward<int>(start);
  __f->step = std::forward<int>(step);
  
  /* Construct the promise. */
  new (&__f->__promise)std::__coroutine_traits_impl<Generator<int> >::promise_type{};
  
  /* Forward declare the resume and destroy function. */
  void __getNextResume(__getNextFrame * __f);
  void __getNextDestroy(__getNextFrame * __f);
  
  /* Assign the resume and destroy function pointers. */
  __f->resume_fn = &__getNextResume;
  __f->destroy_fn = &__getNextDestroy;
  
  /* Call the made up function with the coroutine body for initial suspend.
     This function will be called subsequently by coroutine_handle<>::resume()
     which calls __builtin_coro_resume(__handle_) */
  __getNextResume(__f);
  
  
  return __promise.get_return_object();
}

/* This function invoked by coroutine_handle<>::resume() */
void __getNextResume(__getNextFrame * __f)
{
  try 
  {
    /* Create a switch to get to the correct resume point */
    switch(__f->__suspend_index) {
      case 0: break;
      case 1: goto __resume_getNext_1;
      case 2: goto __resume_getNext_2;
    }
    
    /* co_await insights.cpp:74 */
    __f->__suspend_74_16 = __f->__promise.initial_suspend();
    if(!__f->__suspend_74_16.await_ready()) {
      __f->__suspend_74_16.await_suspend(std::coroutine_handle<Generator<int>::promise_type>::from_address(static_cast<void *>(__f)).operator coroutine_handle());
      __f->__suspend_index = 1;
      __f->__initial_await_suspend_called = true;
      return;
    } 
    
    __resume_getNext_1:
    __f->__suspend_74_16.await_resume();
    std::operator<<(std::cout, "    getNext: start").operator<<(std::endl);
    __f->value = __f->start;
    for( __f->i = 0; ; ++__f->i) {
      std::operator<<(std::cout, "    getNext: before co_yield").operator<<(std::endl);
      
      /* co_yield insights.cpp:79 */
      __f->__suspend_79_9 = __f->__promise.yield_value(__f->value);
      if(!__f->__suspend_79_9.await_ready()) {
        __f->__suspend_79_9.await_suspend(std::coroutine_handle<Generator<int>::promise_type>::from_address(static_cast<void *>(__f)).operator coroutine_handle());
        __f->__suspend_index = 2;
        return;
      } 
      
      __resume_getNext_2:
      __f->__suspend_79_9.await_resume();
      std::operator<<(std::cout, "    getNext: after co_yield").operator<<(std::endl);
      __f->value = static_cast<int>(__f->value + __f->step);
    }
    
    goto __final_suspend;
  } catch(...) {
    if(!__f->__initial_await_suspend_called) {
      throw ;
    } 
    
    __f->__promise.unhandled_exception();
  }
  
  __final_suspend:
  
  /* co_await insights.cpp:74 */
  __f->__suspend_74_16_1 = __f->__promise.final_suspend();
  if(!__f->__suspend_74_16_1.await_ready()) {
    __f->__suspend_74_16_1.await_suspend(std::coroutine_handle<Generator<int>::promise_type>::from_address(static_cast<void *>(__f)).operator coroutine_handle());
  } 
  
  ;
}

/* This function invoked by coroutine_handle<>::destroy() */
void __getNextDestroy(__getNextFrame * __f)
{
  /* destroy all variables with dtors */
  __f->~__getNextFrame();
  /* Deallocating the coroutine frame */
  operator delete(__builtin_coro_free(static_cast<void *>(__f)));
}
#endif

Generator<int> getNext(int start = 10, int step = 10) {
    std::cout << "    getNext: start" << std::endl;
    auto value = start;
    for (int i = 0;; ++i){
        std::cout << "    getNext: before co_yield" << std::endl;
        co_yield value;
        std::cout << "    getNext: after co_yield" << std::endl;
        value += step;
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

