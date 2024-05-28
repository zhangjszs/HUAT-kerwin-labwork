#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Process {
    string name;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int waiting_time;
    int turnaround_time;
};

int main() {
    vector<Process> processes = {
        {"p1", 0, 4, 4, 0, 0},
        {"p2", 1, 6, 6, 0, 0},
        {"p3", 2, 3, 3, 0, 0},
        {"p4", 3, 2, 2, 0, 0}
    };
    int time_slice = 2;

    queue<int> ready_queue;
    for (int i = 0; i < processes.size(); i++) {
        ready_queue.push(i);
    }

    int current_time = 0;
    while (!ready_queue.empty()) {
        int process_id = ready_queue.front();
        ready_queue.pop();

        int run_time = min(time_slice, processes[process_id].remaining_time);
        cout << "在时间 " << current_time << " 执行进程 " << processes[process_id].name << endl;
        processes[process_id].remaining_time -= run_time;
        current_time += run_time;

        if (processes[process_id].remaining_time > 0) {
            ready_queue.push(process_id);
        } else {
            processes[process_id].turnaround_time = current_time - processes[process_id].arrival_time;
            processes[process_id].waiting_time = processes[process_id].turnaround_time - processes[process_id].burst_time;
        }
    }

    cout << "进程\t到达时间\t执行时间\t等待时间\t周转时间" << endl;
    for (const auto& process : processes) {
        cout << process.name << "\t\t" << process.arrival_time << "\t\t" << process.burst_time << "\t\t" << process.waiting_time << "\t\t" << process.turnaround_time << endl;
    }

    return 0;
}