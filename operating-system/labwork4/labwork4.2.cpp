#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>

using namespace std;

struct Process {
    string id;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    float weightedTurnaroundTime;
};

bool compareArrivalTime(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

bool compareBurstTime(const Process &a, const Process &b) {
    return a.burstTime < b.burstTime;
}

void sjfScheduling(vector<Process> &processes) {
    sort(processes.begin(), processes.end(), compareArrivalTime);

    int currentTime = 0;
    int totalProcesses = processes.size();
    int completedProcesses = 0;
    vector<bool> completed(totalProcesses, false);

    while (completedProcesses < totalProcesses) {
        int shortestJobIndex = -1;
        int shortestJobBurst = INT_MAX;

        for (int i = 0; i < totalProcesses; ++i) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime && processes[i].burstTime < shortestJobBurst) {
                shortestJobIndex = i;
                shortestJobBurst = processes[i].burstTime;
            }
        }

        if (shortestJobIndex != -1) {
            cout << "在时间 " << currentTime << " 执行进程 " << processes[shortestJobIndex].id << endl;
            processes[shortestJobIndex].waitingTime = currentTime - processes[shortestJobIndex].arrivalTime;
            currentTime += processes[shortestJobIndex].burstTime;
            processes[shortestJobIndex].turnaroundTime = currentTime - processes[shortestJobIndex].arrivalTime;
            processes[shortestJobIndex].weightedTurnaroundTime = (float)processes[shortestJobIndex].turnaroundTime / processes[shortestJobIndex].burstTime;
            completed[shortestJobIndex] = true;
            ++completedProcesses;
        } else {
            ++currentTime;
        }
    }
}

int main() {
    vector<Process> processes = {{"p1", 0, 5}, {"p2", 3, 6}, {"p3", 4, 3}, {"p4", 5, 1}};
    sjfScheduling(processes);

    float totalWaitingTime = 0;
    float totalTurnaroundTime = 0;
    float totalWeightedTurnaroundTime = 0;

    for (const auto &process : processes) {
        totalWaitingTime += process.waitingTime;
        totalTurnaroundTime += process.turnaroundTime;
        totalWeightedTurnaroundTime += process.weightedTurnaroundTime;
    }

    float avgWaitingTime = totalWaitingTime / processes.size();
    float avgTurnaroundTime = totalTurnaroundTime / processes.size();
    float avgWeightedTurnaroundTime = totalWeightedTurnaroundTime / processes.size();

    cout << "平均等待时间: " << avgWaitingTime << endl;
    cout << "平均周转时间: " << avgTurnaroundTime << endl;
    cout << "平均带权周转时间: " << avgWeightedTurnaroundTime << endl;

    return 0;
}
