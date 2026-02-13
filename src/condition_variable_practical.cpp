#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

using namespace std::literals;

namespace condition_variable_practical {
namespace {

bool       update_progress{false};
bool       completed{false};
std::mutex data_mtx;
std::mutex completed_mtx;

std::condition_variable data_cv;
std::condition_variable completed_cv;

std::string sdata;

void data_fetcher() {
    for (int i = 0; i < 10; ++i) {
        std::print("\033[1;1H\033[KStarting Download ...");
        std::cout << std::flush;
        std::this_thread::sleep_for(2s);
        std::unique_lock<std::mutex> uniq_lock(data_mtx);
        if (i == 0) {
            sdata += "";
        } else if (i < 9) {
            sdata += "";
        } else {
            int current_size = sdata.size();
            sdata = sdata + " (completed) 100%\nDownloaded: " + std::to_string(current_size) +
                    " bytes.";
        }
        update_progress = true;
        uniq_lock.unlock();
        data_cv.notify_all();
    }

    std::print("\033[1;1H\033[KDone.");
    std::cout << std::flush;
    std::lock_guard<std::mutex> lg(completed_mtx);
    completed = true;
    completed_cv.notify_all();
}
void progress_bar() {
    size_t len = 0;
    while (true) {
        std::print("\033[2;1H\033[KFetching data...");
        std::unique_lock<std::mutex> data_lock(data_mtx);
        data_cv.wait(data_lock, [&] { return update_progress; });
        len             = sdata.size();
        update_progress = false;
        data_lock.unlock();
        std::print("\033[3;1H\033[K{}", sdata);
        std::cout << std::flush;
        std::print("\033[4;1H\033[KReceived {} bytes", len);
        std::cout << std::flush;
        std::unique_lock<std::mutex> compl_lck(completed_mtx);
        if (completed_cv.wait_for(compl_lck, 10ms, [&] { return completed; })) {
            std::print("\033[4;1H\033[KDownload Complete!✅ \n\n");
            std::cout << std::flush;
            break;
        }
    }
}

void processing() {
    std::this_thread::sleep_for(200ms);
    std::print("\033[5;1H\033[KPlease wait while Downloading...\n");
    std::cout << std::flush;
    // wait until the download is complete
    std::unique_lock<std::mutex> compl_lck(completed_mtx);

    completed_cv.wait(compl_lck, [&] { return completed; });
    compl_lck.unlock();

    std::lock_guard<std::mutex> data_lck(data_mtx);

    std::print("\033[5;1H\033[KThanks for downloading!\n");
    std::cout << std::flush;
}

} // namespace

void run_demo() {
    std::print("\033[2J\033[1;1H");
    std::cout << std::flush;
    std::thread thr1(data_fetcher);
    std::thread thr2(progress_bar);
    std::thread thr3(processing);
    thr1.join();
    thr2.join();
    thr3.join();
}
} // namespace condition_variable_practical
