#include <iostream>
#include <mutex>
#include <print>
#include <string>
#include <thread>
using namespace std::literals;
namespace thread_coordination {
namespace {
std::string sdata;
bool        update_progress{false};
bool        completed{false};
std::mutex  data_mutex;
std::mutex  completed_mutex;

// Data  fetching Thread
void fetch_data() {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(1s);
        // Update data, then notify the progress bar thread
        std::lock_guard<std::mutex> data_lock(data_mutex);
        if (i == 0) {
            sdata += "";
        } else if (i < 9) {
            sdata += "";
        } else {
            sdata = sdata + " (completed) 100%\nDownloaded: " + std::to_string(sdata.size()) +
                    " bytes.";
        }
        std::print("\033[1;1H\033[KFetching data: {}", sdata);
        std::cout << std::flush;
        update_progress = true;
    }
    std::print("\033[1;1H\033[KFetch sdata has ended");
    std::cout << std::flush;
    // tell progress bar thread to exit
    // and wake up the processing thread
    std::lock_guard<std::mutex> completed_lock(completed_mutex);
    completed = true;
}

// Progress bar thread
void progress_bar() {
    size_t len = 0;
    while (true) {
        // Wait until there is some new data to display
        std::unique_lock<std::mutex> data_lock(data_mutex);
        while (!update_progress) {
            // Show waiting state while checking
            std::print("\033[2;1H\033[KReceived {} bytes so far... (waiting)", len);
            std::cout << std::flush;

            data_lock.unlock();
            std::this_thread::sleep_for(10ms);
            data_lock.lock();
        }
        // wake up and use the new value
        len             = sdata.size();
        update_progress = false;
        data_lock.unlock();

        std::print("\033[2;1H\033[KReceived {} bytes so far...", len);
        std::cout << std::flush;

        // terminate when the download has finished
        std::lock_guard<std::mutex> completed_lock(completed_mutex);
        if (completed) {
            std::print("\033[2;1H\033[KProgress bar thread has ended");
            std::cout << std::flush;
            break;
        }
    }
}

// Processing thread
void process_data() {
    std::print("\033[3;1H\033[KProcessing thread waiting for data...");
    std::cout << std::flush;
    // Wait until the download is complete
    std::unique_lock<std::mutex> completed_lock(completed_mutex);
    while (!completed) {
        completed_lock.unlock();
        std::this_thread::sleep_for(10ms);
        completed_lock.lock();
    }
    completed_lock.unlock();
    std::lock_guard<std::mutex> data_lock(data_mutex);
    std::print("\033[3;1H\033[KProcessing sdata: {}", sdata);
    std::cout << std::flush;
}

} // namespace

void run_demo() {
    // Clear screen and set up initial display
    std::print("\033[2J\033[1;1H");
    std::cout << std::flush;

    std::thread fetcher(fetch_data);
    std::thread progress(progress_bar);
    std::thread processor(process_data);
    fetcher.join();
    progress.join();
    processor.join();

    // Move cursor below all threads' output
    std::println("\n\n\n");
}

} // namespace thread_coordination
