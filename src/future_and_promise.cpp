#include <exception>
#include <future>
#include <print>
#include <thread>

using namespace std::literals;
namespace future_and_promise {
namespace {
void producer(std::promise<int>& px) {
    std::this_thread::sleep_for(2s);
    int x = 42;
    try {
        px.set_value(x);
    } catch (...) {
        px.set_exception(std::current_exception());
    }
}

void consumer(std::future<int>& fx) {
    try {
        int x = fx.get(); // blocks here until set_value`
        std::print("Got: {}\n", x);
    } catch (const std::exception& e) {
        std::print("Error: {}\n", e.what());
    }
}

} // namespace
void run_demo() {
    std::promise<int> prom;
    std::future<int>  fut = prom.get_future();
    std::thread       thr_producer(producer, std::ref(prom));
    std::thread       thr_consumer(consumer, std::ref(fut));
    thr_producer.join();
    thr_consumer.join();
};

} // namespace future_and_promise
