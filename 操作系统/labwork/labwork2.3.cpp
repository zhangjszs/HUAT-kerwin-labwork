/*3. 修改上题，用锁机制实现线程互斥进入临界区。*/
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx; // 定义一个互斥锁
int ticket_sum = 20; // 假设有20张票待售

// 售票函数
void sell_ticket(int window_id) {
    for (int i = 0; i < 20; ++i) {
        std::unique_lock<std::mutex> lock(mtx); // 尝试获取互斥锁
        if (ticket_sum > 0) {
            std::cout << "Window " << window_id << " sells the " << (20 - ticket_sum + 1) << "th ticket." << std::endl;
            --ticket_sum; // 卖出一张票
        }
        lock.unlock(); // 释放互斥锁
        std::this_thread::sleep_for(std::chrono::seconds(1)); // 模拟售票操作耗时
    }
}

int main() {
    const int num_windows = 4; // 售票窗口数量
    std::vector<std::thread> threads;

    // 创建并启动4个售票线程
    for (int i = 0; i < num_windows; ++i) {
        threads.emplace_back(sell_ticket, i + 1);
    }

    // 等待所有售票线程完成
    for (auto &thread : threads) {
        thread.join();
    }

    std::cout << "All tickets have been sold." << std::endl;
    return 0;
}