#include <condition_variable>
#include <mutex>
#include <print>
#include <thread>

using namespace std::literals;

namespace condition_variable_example {
namespace {
std::mutex              mtx;
std::condition_variable cv;
std::string             sdata;
bool                    ready = false;

void reader() {
    std::print("Reader thread locking the mutex\n");
    std::unique_lock<std::mutex> uniq_lock(mtx);
    std::print("Reader thread acquired the mutex\n");

    // Call wait()
    std::print("Reader thread sleeping...\n");
    cv.wait(uniq_lock, [&] { return ready; });
    std::print("Reader thread woken up\n"); // Woken up with lock held
    if (sdata == "Populated.") {            // Display the new value of the string
        std::print("{}", sdata);
    }
}
void writer() {
    std::print("Writer thread locking the mutex\n");
    {
        std::lock_guard<std::mutex> uniq_lock(mtx); // lock the mutex
        std::print("Writer thread has locked the mutex\n");
        std::this_thread::sleep_for(10ms); // modify shared data
        std::print("Modifying the string...\n");
        sdata = "Populated.";
        ready = true;
    }
    std::print("Writer thread notifying the condition variable\n"); // notify the condition variable
    cv.notify_one();
}
} // namespace

void run_demo() {
    std::thread thrA(reader);
    std::thread thrB(writer);
    thrA.join();
    thrB.join();
}
} // namespace condition_variable_example
