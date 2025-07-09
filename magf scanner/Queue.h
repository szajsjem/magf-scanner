#include <mutex>
#include <list>
#include <vector>
#include <functional>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <algorithm>

template<class T>
class Queue {
private:
    mutable std::mutex m;
    std::list<T> items;
    static constexpr size_t MAX_THREADS = 10;

    std::vector<std::thread> threads;
    std::function<bool(T&)> fun;
    mutable std::mutex taskM;
    std::atomic<bool> should_terminate{ false };
    std::condition_variable cv;

    auto size() {
        return items.size();
    }

    static void worker_thread(Queue* queue) {
        while (!queue->should_terminate) {
            std::unique_lock<std::mutex> lock(queue->taskM);
            queue->cv.wait(lock, [queue] {
                return queue->should_terminate.load()||(queue->size()>0);
                });
            if (queue->should_terminate) return;
            T item;
            if (queue->get(item)) {
                if (queue->fun(item)) {
                    queue->insert(item);
                }
            }
        }
    }

public:
    Queue() = default;
    ~Queue() {
        should_terminate = true;
        cv.notify_all();
        for (auto& thread : threads) {
            if (thread.joinable()) thread.join();
        }
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    Queue(Queue&&) = delete;
    Queue& operator=(Queue&&) = delete;

    void insert(const T& t) {
        std::lock_guard<std::mutex> lock(m);
        if (std::find(items.begin(), items.end(), t) == items.end()) {
            items.push_back(t);
        }
        cv.notify_one();
    }

    void sort(bool (*f)(const T&, const T&)) {
        std::lock_guard<std::mutex> lock(m);
        items.sort(f);
    }

    void remove(const T& t) {
        std::lock_guard<std::mutex> lock(m);
        items.remove(t);
    }

    bool get(T& t) {
        std::lock_guard<std::mutex> lock(m);
        if (items.empty()) {
            return false;
        }
        t = std::move(items.front());
        items.pop_front();
        return true;
    }

    void start_exec(std::function<bool(T&)> func) {
        if (threads.empty()) {
            for (size_t i = 0; i < MAX_THREADS; ++i) {
                threads.emplace_back(&Queue::worker_thread, this);
            }
        }
        fun = func;
        cv.notify_one();
    }
};