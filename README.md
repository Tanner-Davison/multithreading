# Multithreading

# C++ Locking Mechanisms

A high-level overview of the locking primitives available in C++ for thread synchronization.

---

## `std::mutex`

The most basic lock. Only one thread can hold it at a time. You manually call `lock()` and `unlock()`, but you almost never should — use a wrapper instead.

```cpp
std::mutex mtx;
mtx.lock();
// critical section
mtx.unlock(); // if an exception throws before this, you deadlock
```

**When to use:** You don't use it directly — use one of the wrappers below.

---

## `std::lock_guard<std::mutex>`

RAII wrapper around a mutex. Locks on construction, unlocks on destruction (when it goes out of scope). Cannot be unlocked early or moved.

```cpp
{
    std::lock_guard<std::mutex> lock(mtx);
    // critical section — safe even if an exception is thrown
} // automatically unlocked here
```

**When to use:** When you need to lock a mutex for the entire duration of a scope and don't need to unlock/relock it manually. This is your default choice.

---

## `std::unique_lock<std::mutex>`

Like `lock_guard` but more flexible. Can be unlocked and relocked, supports deferred locking, and works with condition variables.

```cpp
std::unique_lock<std::mutex> lock(mtx);
// do some work with the lock held
lock.unlock();
// do work that doesn't need the lock
lock.lock();
// back to holding the lock
```

**When to use:**

- When you need to unlock and relock within a scope (like the polling loops in our example)
- When working with `std::condition_variable` (requires `unique_lock`)
- When you need to transfer lock ownership

---

## `std::condition_variable`

Not a lock itself, but a signaling mechanism that works **with** `std::unique_lock`. Lets a thread sleep until another thread wakes it up with `notify_one()` or `notify_all()`. Eliminates busy-polling.

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// Waiting thread
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [&] { return ready; }); // sleeps until ready == true
// woken up with lock held, ready is true

// Signaling thread
{
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
}
cv.notify_one(); // wakes up one waiting thread
```

**When to use:** Whenever one thread needs to wait for a condition that another thread will make true. Replaces the `while (!flag) { sleep; }` pattern.

---

## `std::scoped_lock` (C++17)

Like `lock_guard` but can lock **multiple** mutexes simultaneously without deadlock. Uses a deadlock-avoidance algorithm internally.

```cpp
std::mutex mtx1, mtx2;
{
    std::scoped_lock lock(mtx1, mtx2); // locks both atomically
    // work with both shared resources
}
```

**When to use:** When you need to lock two or more mutexes at the same time. Prevents deadlocks that can happen when two threads lock the same mutexes in different orders.

---

## `std::shared_mutex` + `std::shared_lock` (C++17)

A reader-writer lock. Multiple threads can hold a **shared** (read) lock simultaneously, but only one thread can hold an **exclusive** (write) lock.

```cpp
std::shared_mutex rw_mtx;

// Reader threads (can run concurrently with each other)
{
    std::shared_lock<std::shared_mutex> lock(rw_mtx);
    // read shared data
}

// Writer thread (exclusive access)
{
    std::unique_lock<std::shared_mutex> lock(rw_mtx);
    // modify shared data
}
```

**When to use:** When reads are far more frequent than writes. Multiple readers can proceed in parallel without blocking each other.

---

## `std::recursive_mutex`

A mutex that the **same thread** can lock multiple times without deadlocking. Must be unlocked the same number of times it was locked.

```cpp
std::recursive_mutex rmtx;

void foo() {
    std::lock_guard<std::recursive_mutex> lock(rmtx);
    bar(); // also locks rmtx — no deadlock because same thread
}

void bar() {
    std::lock_guard<std::recursive_mutex> lock(rmtx);
    // works fine
}
```

**When to use:** Rarely. Usually a sign of design issues. Useful in recursive functions or when refactoring legacy code where the same mutex may be acquired multiple times in a call chain.

---

## `std::atomic<T>`

Lock-free synchronization for simple types. No mutex needed — the hardware guarantees atomic reads and writes.

```cpp
std::atomic<bool> ready{false};
std::atomic<int> counter{0};

// Thread 1
counter.fetch_add(1); // atomic increment
ready.store(true);

// Thread 2
while (!ready.load()) { /* spin */ }
int val = counter.load(); // guaranteed to see the increment
```

**When to use:** For simple flags, counters, or pointers where you just need individual operations to be atomic. Much faster than a mutex for simple cases. Not suitable for protecting multi-step operations on complex data.

---

## Quick Reference

| Mechanism            | Use Case                                                     |
| -------------------- | ------------------------------------------------------------ |
| `lock_guard`         | Default choice — simple scoped locking                       |
| `unique_lock`        | Need to unlock/relock, or using condition variables          |
| `scoped_lock`        | Locking multiple mutexes at once                             |
| `condition_variable` | Thread needs to sleep until a condition is met               |
| `shared_mutex`       | Many readers, few writers                                    |
| `recursive_mutex`    | Same thread re-entering a locked section (avoid if possible) |
| `atomic<T>`          | Simple flags/counters without needing a full mutex           |

---

## The Pattern You'll Use 90% of the Time

```cpp
std::mutex mtx;
std::condition_variable cv;
bool data_ready = false;

// Producer
{
    std::lock_guard<std::mutex> lock(mtx);
    // modify shared data
    data_ready = true;
}
cv.notify_one();

// Consumer
std::unique_lock<std::mutex> lock(mtx);
cv.wait(lock, [&] { return data_ready; });
// use shared data (lock is held)
```

This replaces the manual polling pattern and is both more efficient and more correct.
