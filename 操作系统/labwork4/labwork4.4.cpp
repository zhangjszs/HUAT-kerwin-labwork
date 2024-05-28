/*
为了匹配最高优先级算法，自行设定了优先级
每个 processes的参数为 进程 到达时间 服务时间 优先级、
*/
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process
{
    string name;
    int arrival_time;
    int burst_time;
    int priority;
    int waiting_time;
    int turnaround_time;
};

bool comparePriority(const Process &a, const Process &b)
{
    return a.priority < b.priority;
}

bool compareArrivalTime(const Process &a, const Process &b)
{
    return a.arrival_time < b.arrival_time;
}

void priorityScheduling(vector<Process> &processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);

    int current_time = 0;
    for (size_t i = 0; i < processes.size(); ++i)
    {
        sort(processes.begin() + i, processes.end(), comparePriority);

        cout << "在时间 " << current_time << " 执行进程 " << processes[i].name << endl;
        processes[i].turnaround_time = current_time + processes[i].burst_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
        current_time += processes[i].burst_time;
    }
}

int main()
{
    vector<Process> processes = {{"p1", 0, 5, 2}, {"p2", 3, 6, 1}, {"p3", 4, 3, 3}, {"p4", 5, 1, 4}};

    priorityScheduling(processes);

    float total_waiting_time = 0;
    float total_turnaround_time = 0;

    cout << "进程\t到达时间\t执行时间\t优先级\t等待时间\t周转时间" << endl;
    for (const auto &process : processes)
    {
        cout << process.name << "\t\t" << process.arrival_time << "\t\t" << process.burst_time << "\t\t" << process.priority << "\t\t" << process.waiting_time << "\t\t" << process.turnaround_time << endl;
        total_waiting_time += process.waiting_time;
        total_turnaround_time += process.turnaround_time;
    }

    float avg_waiting_time = total_waiting_time / processes.size();
    float avg_turnaround_time = total_turnaround_time / processes.size();

    cout << "平均等待时间: " << avg_waiting_time << endl;
    cout << "平均周转时间: " << avg_turnaround_time << endl;

    return 0;
}