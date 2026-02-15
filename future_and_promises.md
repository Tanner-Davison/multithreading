# C++ std::future & std::promise — Learning Guide

## The Mental Model

Think of `std::promise` and `std::future` as two ends of a **one-shot communication channel** between threads:

- **`std::promise<T>`** — the **write end** (producer). Lives on the thread doing the work.
- **`std::future<T>`** — the **read end** (consumer). Lives on the thread waiting for the result.
- **Shared State** — an internal, ref-counted object that both sides point to. You never touch it directly.

A promise can only be fulfilled **once**. A future can only be read **once**. This is a single-use, one-way pipe.

---

## Lifecycle Step-by-Step

```
1. Create a promise         → shared state is allocated
2. Call promise.get_future() → get the linked future (can only call once!)
3. Move the promise into a worker thread
4. Call future.get() on the consumer side → BLOCKS until ready
5. Worker calls promise.set_value(result) → stores result in shared state
6. future.get() unblocks and returns the result
```

---

## Core API Reference

### std::promise<T>

| Method                                  | What it does                                                    |
| --------------------------------------- | --------------------------------------------------------------- |
| `promise.get_future()`                  | Returns the associated `std::future<T>`. **Call exactly once.** |
| `promise.set_value(val)`                | Fulfills the promise. Unblocks any waiting future.              |
| `promise.set_exception(eptr)`           | Fulfills with an exception instead of a value.                  |
| `promise.set_value_at_thread_exit(val)` | Sets value but defers notification until thread exits.          |

### std::future<T>

| Method                          | What it does                                                                              |
| ------------------------------- | ----------------------------------------------------------------------------------------- |
| `future.get()`                  | Blocks until result is ready, returns it. **Call exactly once** (invalidates the future). |
| `future.wait()`                 | Blocks until ready but doesn't consume the result.                                        |
| `future.wait_for(duration)`     | Waits with a timeout. Returns `future_status::ready`, `timeout`, or `deferred`.           |
| `future.wait_until(time_point)` | Waits until a specific clock time.                                                        |
| `future.valid()`                | Returns `true` if the future has a shared state (hasn't been moved from or `.get()`'d).   |

---

## Example 1: Basic Promise/Future

```cpp
#include <future>
#include <thread>
#include <iostream>

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread worker([&prom]() {
        // Simulate work
        std::this_thread::sleep_for(std::chrono::seconds(1));
        prom.set_value(42);
    });

    std::cout << "Waiting for result...\n";
    int result = fut.get();  // blocks here
    std::cout << "Got: " << result << "\n";

    worker.join();
}
```

> **Note:** Moving the promise into the lambda (`std::move(prom)`) is safer than capturing by reference. The ref works here because `main` outlives the thread, but in real code prefer moves.

---

## Example 2: Proper Ownership with std::move

```cpp
#include <future>
#include <thread>
#include <iostream>

int main() {
    std::promise<std::string> prom;
    auto fut = prom.get_future();

    // Move the promise into the thread — clean ownership
    std::thread worker([p = std::move(prom)]() mutable {
        p.set_value("Hello from worker thread!");
    });

    std::cout << fut.get() << "\n";
    worker.join();
}
```

Key point: `std::promise` is **move-only** (not copyable). The `mutable` on the lambda is required because `set_value` is a non-const operation.

---

## Example 3: Propagating Exceptions

```cpp
std::promise<int> prom;
auto fut = prom.get_future();

std::thread worker([p = std::move(prom)]() mutable {
    try {
        throw std::runtime_error("something broke");
    } catch (...) {
        p.set_exception(std::current_exception());
    }
});

try {
    int val = fut.get();  // rethrows the exception!
} catch (const std::exception& e) {
    std::cerr << "Caught: " << e.what() << "\n";
}

worker.join();
```

This is one of the most powerful features — exceptions cross thread boundaries cleanly.

---

## Example 4: std::async (the Easy Way)

`std::async` creates a promise/future pair internally and manages the thread for you:

```cpp
#include <future>
#include <iostream>

int expensive_computation(int x) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return x * x;
}

int main() {
    // Launch policy: std::launch::async forces a new thread
    auto fut = std::async(std::launch::async, expensive_computation, 7);

    std::cout << "Doing other work...\n";

    std::cout << "Result: " << fut.get() << "\n";  // 49
}
```

> `std::async` with `std::launch::deferred` doesn't launch a thread — it runs the function lazily on `.get()`. The default policy lets the runtime choose.

---

## Example 5: std::shared_future (Multiple Readers)

`std::future` is move-only. If multiple threads need the same result, convert it:

```cpp
std::promise<int> prom;
std::shared_future<int> shared_fut = prom.get_future().share();

// Now multiple threads can call shared_fut.get() safely
std::thread t1([shared_fut]() { std::cout << "T1: " << shared_fut.get() << "\n"; });
std::thread t2([shared_fut]() { std::cout << "T2: " << shared_fut.get() << "\n"; });

prom.set_value(100);
t1.join();
t2.join();
```

Unlike `std::future::get()`, `std::shared_future::get()` can be called **multiple times** and is **copyable**.

---

## Common Pitfalls

### 1. Broken Promise

If a `std::promise` is destroyed without calling `set_value()` or `set_exception()`, the future gets a `std::future_error` with `broken_promise`:

```cpp
{
    std::promise<int> prom;
    auto fut = prom.get_future();
    // prom destroyed here without setting a value!
}
// fut.get() throws std::future_error(std::future_errc::broken_promise)
```

### 2. Calling get_future() Twice

```cpp
std::promise<int> prom;
auto f1 = prom.get_future();  // OK
auto f2 = prom.get_future();  // THROWS std::future_error
```

### 3. Calling get() Twice on std::future

```cpp
auto val1 = fut.get();  // OK, returns value
auto val2 = fut.get();  // THROWS — future is now invalid
```

### 4. Forgetting to join/detach the thread

The promise/future mechanism doesn't manage thread lifetime. You still need `join()` or `detach()`.

---

## When to Use What

| Scenario                                      | Use                                                                  |
| --------------------------------------------- | -------------------------------------------------------------------- |
| Fire-and-forget async task                    | `std::async`                                                         |
| Need explicit thread control + result passing | `std::promise` / `std::future`                                       |
| Multiple consumers for one result             | `std::shared_future`                                                 |
| Continuous producer/consumer stream           | Use a queue + condition_variable instead                             |
| Need to cancel work                           | Futures don't support cancellation — use an `std::atomic<bool>` flag |

---

## Relationship Diagram

```
std::async(func)
    └─ creates promise + future + thread internally
       └─ returns std::future<T>

std::promise<T>  ──────────────────────────────  std::future<T>
   (write end)         Shared State               (read end)
  .set_value()    ←─ allocated by promise ─→       .get()
  .set_exception()                                 .wait()
                                                   .wait_for()
                                                   .share() → std::shared_future<T>
```

---

## Quick Decision Flowchart

```
Need async result?
  ├─ Yes, simple case → std::async
  ├─ Yes, need thread control → std::promise + std::future + std::thread
  ├─ Multiple readers? → .share() → std::shared_future
  └─ Continuous data stream? → Don't use futures. Use a thread-safe queue.
```
