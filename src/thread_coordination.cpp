#include <mutex>
#include <print>
#include <thread>

using namespace std::literals;

namespace thread_coordination {
namespace {
std::string sdata;
bool        update_progress{false};
bool        completed{false};

std::mutex data_mutex;
std::mutex completed_mutex;

// Data  fetching Thread
void fetch_data() {
    for (int i = 0; i < 10; ++i) {
        std::print("Fetcher thread waiting for data\n");
        std::this_thread::sleep_for(2s);
        // Update data, then notiy the progress bar thread
        std::lock_guard<std::mutex> data_lock(data_mutex);
        sdata += "  ";
        std::println("sdata: {}", sdata);
        update_progress = true;
    }
    std::print("Fetch sdata has ended\n");
    // tell progress bar thread to exit
    // and wake up the processing thread
    std::lock_guard<std::mutex> completed_lock(completed_mutex);
    completed = true;
}
// Progress bar thread
void progress_bar() {
    size_t len = 0;
    while (true) {
        std::print("Progress bar thread waiting for data...\n");
        // Wait until there is some new data to display
        std::unique_lock<std::mutex> data_lock(data_mutex);
        while (!update_progress) {
            data_lock.unlock();
            std::this_thread::sleep_for(10ms);
            data_lock.lock();
        }

        // wake up and use the new value
        len = sdata.size();

        update_progress = false;
        data_lock.unlock();
        std::print("Recieved {} bytes so far\n", len);

        // terminate when the download has finished
        std::lock_guard<std::mutex> completed_lock(completed_mutex);
        if (completed) {
            std::print("Progress bard thread has ended\n");
            break;
        }
    }
}
// Processing thread
void process_data() {
    std::print("Processing thread waiting for data\n");

    // Wait until the download is complete
    std::unique_lock<std::mutex> completed_lock(completed_mutex);

    while (!completed) {
        completed_lock.unlock();
        std::this_thread::sleep_for(10ms);
        completed_lock.lock();
    }

    completed_lock.unlock();

    std::lock_guard<std::mutex> data_lock(data_mutex);
    std::print("Processing sdata: {}", sdata);

    // processing data();
}
} // namespace
void run_demo() {
    std::thread fetcher(fetch_data);
    std::thread progress(progress_bar);
    std::thread processor(process_data);
    fetcher.join();
    progress.join();
    processor.join();
}
} // namespace thread_coordination
