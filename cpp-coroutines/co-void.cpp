#include <coroutine>
#include <future>
#include <iostream>

using namespace std;

struct resumable_thing
{
	struct promise_type {
		resumable_thing  get_return_object()
		{
			return resumable_thing(coroutine_handle<promise_type>::from_promise(*this));
		}

		auto initial_suspend() { return suspend_always{}; }
		auto final_suspend() noexcept { return suspend_always{}; }
		void return_void() noexcept { }
		void unhandled_exception() {
			cout << __func__ << endl;
		}
	};

	coroutine_handle<promise_type> _coroutine = nullptr;

	operator coroutine_handle<promise_type>(){ return _coroutine; }

	explicit resumable_thing(coroutine_handle<promise_type> coroutine)
		: _coroutine(coroutine) {}

	~resumable_thing()
	{
		cout << __func__ << endl;
		if (_coroutine) { _coroutine.destroy(); }
	}

	resumable_thing() = default;
	resumable_thing(resumable_thing const&) = delete;
	resumable_thing& operator=(resumable_thing const&) = delete;

	resumable_thing(resumable_thing&& other)
		: _coroutine(other._coroutine) { other._coroutine = nullptr; }

	resumable_thing& operator=(resumable_thing&& other)
	{
		if (&other != this) {
			_coroutine = other._coroutine;
			other._coroutine = nullptr;
		}
		return *this;
	}

	void resume() { _coroutine.resume(); }
};

resumable_thing counter()
{
	cout << "counter: called\n";
	for (unsigned i=1; ; i++) {
		cout << "counter: loop iteration (#" << i << ")\n";
		co_await suspend_always{};
	}
	cout << "counter: exiting\n";
}

int main(void)
{
	cout << "main: calling counter\n";
	auto the_counter = counter();
	the_counter.resume();
	the_counter.resume();
	the_counter.resume();
	cout << "main: done\n";
	return 0;
}
