#include <exception>
#include <future>
#include <print>
#include <thread>

using namespace std::literals;
namespace future_and_promise {
namespace {
void producer(std::promise<int>& px) {
    std::this_thread::sleep_for(2s);
    try {
        int x = 42;
        // Exception demo:
        /* if (1) { // change to 1 to throw OOops!
            px.set_exception(std::make_exception_ptr(std::out_of_range("OOoops!")));
            return;
        } */
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
