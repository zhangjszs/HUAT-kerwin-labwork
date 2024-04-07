/*2. 修改第1题，显示哪个窗口卖出的票（显示线程ID）。*/
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex ticket_mutex; // 用于保护票数的互斥锁
int ticket_sum = 20; // 假设有20张票待售

// 售票函数
void sell_ticket() {
    for (int i = 0; i < 20; ++i) {
        std::unique_lock<std::mutex> guard(ticket_mutex); // 锁定互斥锁
        if (ticket_sum > 0) {
            std::cout << "Window " << std::this_thread::get_id() << " sells the " << (20 - ticket_sum + 1) << "th ticket." << std::endl;
            -- ticket_sum; // 卖出一张票
        }
        guard.unlock(); // 解锁互斥锁
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟售票操作耗时
    }
}

int main() {
    const int num_windows = 4; // 售票窗口数量
    std::vector<std::thread> threads;

    // 创建并启动4个售票线程
    for (int i = 0; i < num_windows; ++i) {
        threads.emplace_back(sell_ticket);
    }

    // 等待所有售票线程完成
    for (auto &thread : threads) {
        thread.join();
    }

    std::cout << "All tickets have been sold." << std::endl;
    return 0;
}