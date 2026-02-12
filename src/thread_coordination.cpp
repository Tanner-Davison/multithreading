/* This program demonstrates how to use std::mutex to coordinate
 * threads. The program has three threads:
 * 1. Data fetching thread
 * 2. Progress bar thread
 * 3. Processing thread
 *
 * The data fetching thread fetches data from a remote server and
 * updates the progress bar thread with the new data. The progress bar
 * thread displays the data fetched by the data fetching thread.
 * Finally, the processing thread processes the data fetched by the
 * data fetching thread.
 *
 * The program uses std::mutex to coordinate the threads. The mutexes
 * are used to protect the shared data and to notify the threads when
 * there is new data to display.
 *
 * The program uses ANSI escape codes to move the cursor and clear the
 * screen. The ANSI escape codes are used to position the cursor and
 * clear the screen.
 *
 * The program uses std::print and std::println to print the output.
 * The std::print and std::println functions are part of the C++
 * standard library and are used to print formatted output to the
 * console.
 *
 * The program uses std::flush to ensure that the output is immediately
 * displayed to the console.
 *
 * The program uses std::this_thread::sleep_for to introduce a delay
 * between the threads

*/
/* Final Output:
*
* Fetch sdata has ended
* Progress bar thread has ended
* Processing sdata:  (completed) 100%
* Downloaded: 27 bytes.



Execution Total time: 5007ms */

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
        std::this_thread::sleep_for(500ms);
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
            std::this_thread::sleep_for(250ms);
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
        std::this_thread::sleep_for(500ms);
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
