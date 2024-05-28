#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process
{
    string name;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    double response_ratio;
};

bool compareArrivalTime(const Process &a, const Process &b)
{
    return a.arrival_time < b.arrival_time;
}

bool compareResponseRatio(const Process &a, const Process &b)
{
    return a.response_ratio > b.response_ratio;
}

void calculateResponseRatio(vector<Process> &processes, int current_time)
{
    for (auto &process : processes)
    {
        if (process.arrival_time <= current_time && process.turnaround_time == -1)
        {
            process.response_ratio = (double)(current_time - process.arrival_time + process.burst_time) / process.burst_time;
        }
    }
}

void calculateWaitingTime(vector<Process> &processes)
{
    for (size_t i = 0; i < processes.size(); ++i)
    {
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
    }
}

void calculateTurnaroundTime(vector<Process> &processes)
{
    int current_time = 0;
    for (size_t i = 0; i < processes.size(); ++i)
    {
        current_time += processes[i].burst_time;
        processes[i].turnaround_time = current_time - processes[i].arrival_time;
    }
}

void scheduleProcesses(vector<Process> &processes)
{
    sort(processes.begin(), processes.end(), compareArrivalTime);

    int current_time = 0;
    for (size_t i = 0; i < processes.size(); ++i)
    {
        calculateResponseRatio(processes, current_time);
        sort(processes.begin() + i, processes.end(), compareResponseRatio);

        cout << "在时间 " << current_time << " 执行进程 " << processes[i].name << endl;
        current_time += processes[i].burst_time;
        processes[i].turnaround_time = current_time - processes[i].arrival_time;
    }

    calculateWaitingTime(processes);
    calculateTurnaroundTime(processes);
}

int main()
{
    vector<Process> processes = {{"p1", 0, 4, -1, -1}, {"p2", 1, 6, -1, -1}, {"p3", 2, 3, -1, -1}, {"p4", 3, 2, -1, -1}};

    scheduleProcesses(processes);

    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;

    cout << "进程\t到达时间\t执行时间\t等待时间\t周转时间" << endl;
    for (const auto &process : processes)
    {
        cout << process.name << "\t\t" << process.arrival_time << "\t\t" << process.burst_time << "\t\t" << process.waiting_time << "\t\t" << process.turnaround_time << endl;
        totalWaitingTime += process.waiting_time;
        totalTurnaroundTime += process.turnaround_time;
    }

    float avgWaitingTime = totalWaitingTime / processes.size();
    float avgTurnaroundTime = totalTurnaroundTime / processes.size();

    cout << fixed << setprecision(2);
    cout << "平均等待时间: " << avgWaitingTime << endl;
    cout << "平均周转时间: " << avgTurnaroundTime << endl;

    return 0;
}