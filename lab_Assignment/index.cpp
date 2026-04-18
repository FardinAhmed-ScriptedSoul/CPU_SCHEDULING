#include <bits/stdc++.h>
using namespace std;

// Structure to hold process information
struct Process {
    int id;
    int burst_time;
    int arrival_time;
    int priority;
    int start_time;
    int end_time;
    int response_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
};

struct Metrics {
    double avg_rt;
    double avg_wt;
    double avg_tt;
};

// Function to input processes
void inputProcesses(vector<Process>& processes, int n, bool hasPriority = false) {
    for(int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        cout << "Enter the burst time of P" << p.id << ": ";
        cin >> p.burst_time;
        p.remaining_time = p.burst_time;
        cout << "Enter the arrival time of P" << p.id << ": ";
        cin >> p.arrival_time;
        if(hasPriority) {
            cout << "Enter the priority of P" << p.id << ": ";
            cin >> p.priority;
        }
        processes.push_back(p);
    }
}

// Function to display results
void displayResults(const vector<Process>& processes, double avg_wt, double avg_tt, double avg_rt) {
    cout << "\nProcess Details:\n";
    for(const auto& p : processes) {
        cout << "Process: P" << p.id
             << " Start time: " << p.start_time
             << " End time: " << p.end_time
             << " Response Time: " << p.response_time
             << " Waiting time: " << p.waiting_time
             << " Turnaround time: " << p.turnaround_time << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Response Time: " << avg_rt << endl;
    cout << "Average Waiting Time: " << avg_wt << endl;
    cout << "Average Turnaround Time: " << avg_tt << endl;
}

Metrics runFCFS(vector<Process> processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int n = (int)processes.size();
    int current_time = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    for(auto& p : processes) {
        current_time = max(current_time, p.arrival_time);
        p.start_time = current_time;
        p.response_time = p.start_time - p.arrival_time;
        current_time += p.burst_time;
        p.end_time = current_time;
        p.waiting_time = p.start_time - p.arrival_time;
        p.turnaround_time = p.end_time - p.arrival_time;
        total_wt += p.waiting_time;
        total_tt += p.turnaround_time;
        total_rt += p.response_time;
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

Metrics runSJFNonPreemptive(vector<Process> processes) {
    int n = (int)processes.size();
    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    while(completed_count < n) {
        int idx = -1, min_burst = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time && processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                idx = i;
            }
        }
        if(idx == -1) {
            current_time++;
            continue;
        }

        processes[idx].start_time = current_time;
        processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        current_time += processes[idx].burst_time;
        processes[idx].end_time = current_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        total_rt += processes[idx].response_time;
        completed[idx] = true;
        completed_count++;
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

Metrics runSJFPreemptive(vector<Process> processes) {
    int n = (int)processes.size();
    for(auto& p : processes) p.remaining_time = p.burst_time;

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    while(completed_count < n) {
        int idx = -1, min_remaining = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time && processes[i].remaining_time < min_remaining) {
                min_remaining = processes[i].remaining_time;
                idx = i;
            }
        }
        if(idx == -1) {
            current_time++;
            continue;
        }

        if(processes[idx].remaining_time == processes[idx].burst_time) {
            processes[idx].start_time = current_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        }

        current_time++;
        processes[idx].remaining_time--;

        if(processes[idx].remaining_time == 0) {
            processes[idx].end_time = current_time;
            processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            total_rt += processes[idx].response_time;
            completed[idx] = true;
            completed_count++;
        }
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

Metrics runPriorityNonPreemptive(vector<Process> processes) {
    int n = (int)processes.size();
    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    while(completed_count < n) {
        int idx = -1, min_priority = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time && processes[i].priority < min_priority) {
                min_priority = processes[i].priority;
                idx = i;
            }
        }
        if(idx == -1) {
            current_time++;
            continue;
        }

        processes[idx].start_time = current_time;
        processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        current_time += processes[idx].burst_time;
        processes[idx].end_time = current_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        total_rt += processes[idx].response_time;
        completed[idx] = true;
        completed_count++;
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

Metrics runPriorityPreemptive(vector<Process> processes) {
    int n = (int)processes.size();
    for(auto& p : processes) p.remaining_time = p.burst_time;

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    while(completed_count < n) {
        int idx = -1, min_priority = INT_MAX;
        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0 && processes[i].priority < min_priority) {
                min_priority = processes[i].priority;
                idx = i;
            }
        }
        if(idx == -1) {
            current_time++;
            continue;
        }

        if(processes[idx].remaining_time == processes[idx].burst_time) {
            processes[idx].start_time = current_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        }

        current_time++;
        processes[idx].remaining_time--;

        if(processes[idx].remaining_time == 0) {
            processes[idx].end_time = current_time;
            processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            total_rt += processes[idx].response_time;
            completed[idx] = true;
            completed_count++;
        }
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

Metrics runRoundRobin(vector<Process> processes, int quantum) {
    int n = (int)processes.size();
    for(auto& p : processes) p.remaining_time = p.burst_time;

    queue<int> ready_queue;
    vector<bool> in_queue(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    while(completed_count < n) {
        for(int i = 0; i < n; i++) {
            if(!in_queue[i] && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                ready_queue.push(i);
                in_queue[i] = true;
            }
        }

        if(ready_queue.empty()) {
            current_time++;
            continue;
        }

        int idx = ready_queue.front();
        ready_queue.pop();

        if(processes[idx].remaining_time == processes[idx].burst_time) {
            processes[idx].start_time = current_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        }

        int execute_time = min(quantum, processes[idx].remaining_time);
        current_time += execute_time;
        processes[idx].remaining_time -= execute_time;

        for(int i = 0; i < n; i++) {
            if(!in_queue[i] && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                ready_queue.push(i);
                in_queue[i] = true;
            }
        }

        if(processes[idx].remaining_time == 0) {
            processes[idx].end_time = current_time;
            processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            total_rt += processes[idx].response_time;
            completed_count++;
        } else {
            ready_queue.push(idx);
        }
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

Metrics runHRRN(vector<Process> processes) {
    int n = (int)processes.size();
    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    while(completed_count < n) {
        int idx = -1;
        double max_ratio = -1.0;

        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time) {
                double waiting_time = current_time - processes[i].arrival_time;
                double response_ratio = (waiting_time + processes[i].burst_time) / (double)processes[i].burst_time;
                if(response_ratio > max_ratio) {
                    max_ratio = response_ratio;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            current_time++;
            continue;
        }

        processes[idx].start_time = current_time;
        processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        current_time += processes[idx].burst_time;
        processes[idx].end_time = current_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        total_rt += processes[idx].response_time;
        completed[idx] = true;
        completed_count++;
    }

    return {total_rt / n, total_wt / n, total_tt / n};
}

// FCFS Scheduling
void FCFS() {
    cout << "\n=== First Come First Served (FCFS) Scheduling ===\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    inputProcesses(processes, n);

    // Sort by arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    for(auto& p : processes) {
        current_time = max(current_time, p.arrival_time);
        p.start_time = current_time;
        p.response_time = p.start_time - p.arrival_time;
        cout << current_time << " P" << p.id << " ";
        current_time += p.burst_time;
        p.end_time = current_time;
        p.waiting_time = p.start_time - p.arrival_time;
        p.turnaround_time = current_time - p.arrival_time;
        total_wt += p.waiting_time;
        total_tt += p.turnaround_time;
        total_rt += p.response_time;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// SJF Scheduling (Non-preemptive)
void SJFNonPreemptive() {
    cout << "\n=== Non-Preemptive Shortest Job First (SJF) Scheduling ===\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    inputProcesses(processes, n);

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    while(completed_count < n) {
        int idx = -1;
        int min_burst = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time) {
                if(processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            current_time++;
            continue;
        }

        processes[idx].start_time = current_time;
        processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        cout << current_time << " P" << processes[idx].id << " ";
        current_time += processes[idx].burst_time;
        processes[idx].end_time = current_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        total_rt += processes[idx].response_time;
        completed[idx] = true;
        completed_count++;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// SJF Scheduling (Preemptive)
void SJFPreemptive() {
    cout << "\n=== Preemptive Shortest Job First (SJF) Scheduling ===\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    inputProcesses(processes, n);

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    while(completed_count < n) {
        int idx = -1;
        int min_remaining = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time) {
                if(processes[i].remaining_time < min_remaining) {
                    min_remaining = processes[i].remaining_time;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            current_time++;
            continue;
        }

        if(processes[idx].remaining_time == processes[idx].burst_time) {
            processes[idx].start_time = current_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        }

        cout << current_time << " P" << processes[idx].id << " ";
        current_time++;
        processes[idx].remaining_time--;

        if(processes[idx].remaining_time == 0) {
            processes[idx].end_time = current_time;
            processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            total_rt += processes[idx].response_time;
            completed[idx] = true;
            completed_count++;
        }
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// Priority Scheduling (Non-preemptive)
void PriorityNonPreemptive() {
    cout << "\n=== Non-Preemptive Priority Scheduling ===\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    inputProcesses(processes, n, true);

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    while(completed_count < n) {
        int idx = -1;
        int min_priority = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time) {
                if(processes[i].priority < min_priority) {
                    min_priority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            current_time++;
            continue;
        }

        processes[idx].start_time = current_time;
        processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        cout << current_time << " P" << processes[idx].id << " ";
        current_time += processes[idx].burst_time;
        processes[idx].end_time = current_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        total_rt += processes[idx].response_time;
        completed[idx] = true;
        completed_count++;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// Priority Scheduling (Preemptive)
void PriorityPreemptive() {
    cout << "\n=== Preemptive Priority Scheduling ===\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    inputProcesses(processes, n, true);

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    while(completed_count < n) {
        int idx = -1;
        int min_priority = INT_MAX;

        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if(processes[i].priority < min_priority) {
                    min_priority = processes[i].priority;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            current_time++;
            continue;
        }

        if(processes[idx].remaining_time == processes[idx].burst_time) {
            processes[idx].start_time = current_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        }

        cout << current_time << " P" << processes[idx].id << " ";
        current_time++;
        processes[idx].remaining_time--;

        if(processes[idx].remaining_time == 0) {
            processes[idx].end_time = current_time;
            processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            total_rt += processes[idx].response_time;
            completed[idx] = true;
            completed_count++;
        }
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// Round Robin Scheduling
void RoundRobin() {
    cout << "\n=== Round Robin Scheduling ===\n";
    int n, quantum;
    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter time quantum: ";
    cin >> quantum;

    vector<Process> processes;
    inputProcesses(processes, n);

    queue<int> ready_queue;
    vector<bool> in_queue(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    while(completed_count < n) {
        // Add arrived processes to queue
        for(int i = 0; i < n; i++) {
            if(!in_queue[i] && processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                ready_queue.push(i);
                in_queue[i] = true;
            }
        }

        if(ready_queue.empty()) {
            current_time++;
            continue;
        }

        int idx = ready_queue.front();
        ready_queue.pop();

        if(processes[idx].remaining_time == processes[idx].burst_time) {
            processes[idx].start_time = current_time;
            processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        }

        cout << current_time << " P" << processes[idx].id << " ";

        int execute_time = min(quantum, processes[idx].remaining_time);
        current_time += execute_time;
        processes[idx].remaining_time -= execute_time;

        if(processes[idx].remaining_time == 0) {
            processes[idx].end_time = current_time;
            processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            total_rt += processes[idx].response_time;
            completed_count++;
        } else {
            // Re-add to queue if not completed
            ready_queue.push(idx);
        }
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// HRRN (Highest Response Ratio Next) Scheduling - Non-preemptive
void HRRN() {
    cout << "\n=== Highest Response Ratio Next (HRRN) Scheduling ===\n";
    int n;
    cout << "Enter number of processes: ";
    cin >> n;

    vector<Process> processes;
    inputProcesses(processes, n);

    vector<bool> completed(n, false);
    int current_time = 0, completed_count = 0;
    double total_wt = 0, total_tt = 0, total_rt = 0;

    cout << "\nGantt Chart:\n";
    while(completed_count < n) {
        int idx = -1;
        double max_ratio = -1.0;

        // Find process with highest response ratio among available ones
        for(int i = 0; i < n; i++) {
            if(!completed[i] && processes[i].arrival_time <= current_time) {
                double waiting_time = current_time - processes[i].arrival_time;
                double response_ratio = (waiting_time + processes[i].burst_time) / (double)processes[i].burst_time;
                
                if(response_ratio > max_ratio) {
                    max_ratio = response_ratio;
                    idx = i;
                }
            }
        }

        if(idx == -1) {
            current_time++;
            continue;
        }

        processes[idx].start_time = current_time;
        processes[idx].response_time = processes[idx].start_time - processes[idx].arrival_time;
        cout << current_time << " P" << processes[idx].id << " ";
        current_time += processes[idx].burst_time;
        processes[idx].end_time = current_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        total_rt += processes[idx].response_time;
        completed[idx] = true;
        completed_count++;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n, total_rt / n);
}

// Compare all algorithms on same input
void CompareAll() {
    int n;
    cout << "Number of process, n: ";
    cin >> n;

    vector<Process> base;
    for(int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        cout << "\nEnter the burst time of P" << p.id << ": ";
        cin >> p.burst_time;
        cout << "Enter the arrival time of P" << p.id << ": ";
        cin >> p.arrival_time;
        cout << "Enter the priority of P" << p.id << ": ";
        cin >> p.priority;
        p.remaining_time = p.burst_time;
        base.push_back(p);
    }

    int quantum;
    cout << "\nTime Quantum (for Round Robin): ";
    cin >> quantum;

    Metrics fcfs = runFCFS(base);
    Metrics sjf_np = runSJFNonPreemptive(base);
    Metrics sjf_p = runSJFPreemptive(base);
    Metrics pri_np = runPriorityNonPreemptive(base);
    Metrics pri_p = runPriorityPreemptive(base);
    Metrics rr = runRoundRobin(base, quantum);
    Metrics own = runHRRN(base);

    cout << fixed << setprecision(2);
    cout << "\nAlgo: FCFS Avg Response T.: " << fcfs.avg_rt << " Avg Waiting T.: " << fcfs.avg_wt << " Avg. Turnaround T: " << fcfs.avg_tt << endl;
    cout << "Algo: Non-Preemptive-SJF Avg Response T.: " << sjf_np.avg_rt << " Avg Waiting T.: " << sjf_np.avg_wt << " Avg. Turnaround T: " << sjf_np.avg_tt << endl;
    cout << "Algo: Preemptive-SJF Avg Response T.: " << sjf_p.avg_rt << " Avg Waiting T.: " << sjf_p.avg_wt << " Avg. Turnaround T: " << sjf_p.avg_tt << endl;
    cout << "Algo: Non-Preemptive-Priority Avg Response T.: " << pri_np.avg_rt << " Avg Waiting T.: " << pri_np.avg_wt << " Avg. Turnaround T: " << pri_np.avg_tt << endl;
    cout << "Algo: Preemptive-Priority Avg Response T.: " << pri_p.avg_rt << " Avg Waiting T.: " << pri_p.avg_wt << " Avg. Turnaround T: " << pri_p.avg_tt << endl;
    cout << "Algo: Round-Robin Avg Response T.: " << rr.avg_rt << " Avg Waiting T.: " << rr.avg_wt << " Avg. Turnaround T: " << rr.avg_tt << endl;
    cout << "Algo: Your-Own-Algorithm Avg Response T.: " << own.avg_rt << " Avg Waiting T.: " << own.avg_wt << " Avg. Turnaround T: " << own.avg_tt << endl;
}

int main() {
    int choice;
    do {
        cout << "\n=== CPU Scheduling Algorithms Simulator ===\n";
        cout << "1: FCFS\n";
        cout << "2: Non-Preemptive-SJF\n";
        cout << "3: Preemptive-SJF\n";
        cout << "4: Non-Preemptive-Priority\n";
        cout << "5: Preemptive-Priority\n";
        cout << "6: Round-Robin\n";
        cout << "7: Your-Own-Algorithm\n";
        cout << "8: Compare-All\n";
        cout << "9. Exit\n";
        cout << "Input your Choice: ";
        cin >> choice;

        switch(choice) {
            case 1:
                FCFS();
                break;
            case 2:
                SJFNonPreemptive();
                break;
            case 3:
                SJFPreemptive();
                break;
            case 4:
                PriorityNonPreemptive();
                break;
            case 5:
                PriorityPreemptive();
                break;
            case 6:
                RoundRobin();
                break;
            case 7:
                HRRN();
                break;
            case 8:
                CompareAll();
                break;
            case 9:
                cout << "Thank you for using CPU Scheduler!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 9);

    return 0;
}

