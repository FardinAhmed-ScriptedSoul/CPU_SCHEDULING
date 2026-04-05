#include <bits/stdc++.h>
using namespace std;

// Structure to hold process information
struct Process {
    int id;
    int burst_time;
    int arrival_time;
    int priority;
    int start_time;
    int waiting_time;
    int turnaround_time;
    int remaining_time;
};

// Function to input processes
void inputProcesses(vector<Process>& processes, int n, bool hasPriority = false) {
    for(int i = 0; i < n; i++) {
        Process p;
        p.id = i + 1;
        cout << "Enter burst time for P" << p.id << ": ";
        cin >> p.burst_time;
        p.remaining_time = p.burst_time;
        cout << "Enter arrival time for P" << p.id << ": ";
        cin >> p.arrival_time;
        if(hasPriority) {
            cout << "Enter priority for P" << p.id << ": ";
            cin >> p.priority;
        }
        processes.push_back(p);
    }
}

// Function to display results
void displayResults(const vector<Process>& processes, double avg_wt, double avg_tt) {
    cout << "\nProcess Details:\n";
    cout << "Process\tStart Time\tWaiting Time\tTurnaround Time\n";
    for(const auto& p : processes) {
        cout << "P" << p.id << "\t\t" << p.start_time << "\t\t" << p.waiting_time << "\t\t" << p.turnaround_time << endl;
    }
    cout << fixed << setprecision(2);
    cout << "\nAverage Waiting Time: " << avg_wt << endl;
    cout << "Average Turnaround Time: " << avg_tt << endl;
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
    double total_wt = 0, total_tt = 0;

    cout << "\nGantt Chart:\n";
    for(auto& p : processes) {
        current_time = max(current_time, p.arrival_time);
        p.start_time = current_time;
        cout << current_time << " P" << p.id << " ";
        current_time += p.burst_time;
        p.waiting_time = p.start_time - p.arrival_time;
        p.turnaround_time = current_time - p.arrival_time;
        total_wt += p.waiting_time;
        total_tt += p.turnaround_time;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
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
    double total_wt = 0, total_tt = 0;

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
        cout << current_time << " P" << processes[idx].id << " ";
        current_time += processes[idx].burst_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        completed[idx] = true;
        completed_count++;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
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
    double total_wt = 0, total_tt = 0;

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
        }

        cout << current_time << " P" << processes[idx].id << " ";
        current_time++;
        processes[idx].remaining_time--;

        if(processes[idx].remaining_time == 0) {
            processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            completed[idx] = true;
            completed_count++;
        }
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
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
    double total_wt = 0, total_tt = 0;

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
        cout << current_time << " P" << processes[idx].id << " ";
        current_time += processes[idx].burst_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        completed[idx] = true;
        completed_count++;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
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
    double total_wt = 0, total_tt = 0;

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
        }

        cout << current_time << " P" << processes[idx].id << " ";
        current_time++;
        processes[idx].remaining_time--;

        if(processes[idx].remaining_time == 0) {
            processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            completed[idx] = true;
            completed_count++;
        }
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
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
    double total_wt = 0, total_tt = 0;

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
        }

        cout << current_time << " P" << processes[idx].id << " ";

        int execute_time = min(quantum, processes[idx].remaining_time);
        current_time += execute_time;
        processes[idx].remaining_time -= execute_time;

        if(processes[idx].remaining_time == 0) {
            processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
            processes[idx].waiting_time = processes[idx].turnaround_time - processes[idx].burst_time;
            total_wt += processes[idx].waiting_time;
            total_tt += processes[idx].turnaround_time;
            completed_count++;
        } else {
            // Re-add to queue if not completed
            ready_queue.push(idx);
        }
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
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
    double total_wt = 0, total_tt = 0;

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
        cout << current_time << " P" << processes[idx].id << " ";
        current_time += processes[idx].burst_time;
        processes[idx].waiting_time = processes[idx].start_time - processes[idx].arrival_time;
        processes[idx].turnaround_time = current_time - processes[idx].arrival_time;
        total_wt += processes[idx].waiting_time;
        total_tt += processes[idx].turnaround_time;
        completed[idx] = true;
        completed_count++;
    }
    cout << current_time << endl;

    displayResults(processes, total_wt / n, total_tt / n);
}

// Display algorithm comparison
void DisplayComparison() {
    cout << "\n==================================\n";
    cout << "     ALGORITHM COMPARISON\n";
    cout << "==================================\n\n";
    
    cout << "1. FCFS (First Come First Served)\n";
    cout << "   - Preemption: No\n";
    cout << "   - Complexity: O(n)\n";
    cout << "   - Avg Wait Time: Generally Poor\n";
    cout << "   - Pros: Simple, fair\n";
    cout << "   - Cons: High waiting time, not suitable for interactive systems\n\n";

    cout << "2. SJF (Shortest Job First)\n";
    cout << "   - Preemption: No\n";
    cout << "   - Complexity: O(n²)\n";
    cout << "   - Avg Wait Time: Better than FCFS\n";
    cout << "   - Pros: Minimizes average waiting time\n";
    cout << "   - Cons: Starvation possible, requires prior knowledge of burst time\n\n";

    cout << "3. Priority Scheduling\n";
    cout << "   - Preemption: No\n";
    cout << "   - Complexity: O(n²)\n";
    cout << "   - Avg Wait Time: Depends on priority assignment\n";
    cout << "   - Pros: Flexible, importance-based\n";
    cout << "   - Cons: Starvation possible, requires priority mechanism\n\n";

    cout << "4. Round Robin\n";
    cout << "   - Preemption: Yes\n";
    cout << "   - Complexity: O(n * time_quantum)\n";
    cout << "   - Avg Wait Time: Fair, depends on quantum\n";
    cout << "   - Pros: Fair to all, no starvation, good for interactive\n";
    cout << "   - Cons: Higher context switching, sensitive to time quantum\n\n";

    cout << "5. HRRN (Highest Response Ratio Next) ⭐ NEW\n";
    cout << "   - Preemption: No\n";
    cout << "   - Complexity: O(n²)\n";
    cout << "   - Avg Wait Time: Better than SJF in many cases\n";
    cout << "   - Pros: Balances SJF and FCFS, prevents starvation for long jobs\n";
    cout << "   - Cons: More complex, requires calculation of response ratio\n";
    cout << "   - Formula: Response Ratio = (Waiting Time + Burst Time) / Burst Time\n\n";

    cout << "==================================\n";
    cout << "      COMPARISON TABLE\n";
    cout << "==================================\n\n";
    cout << "Algorithm    | Preemption | Avg Wait | Starvation | Complexity\n";
    cout << "-------------|------------|----------|------------|------------\n";
    cout << "FCFS         |    No      |  Poor    |    No      |    O(n)\n";
    cout << "SJF          |    No      |  Good    |   Possible |   O(n²)\n";
    cout << "Priority     |    No      |  Medium  |   Possible |   O(n²)\n";
    cout << "Round Robin  |    Yes     |  Fair    |    No      | O(n*Q)\n";
    cout << "HRRN         |    No      |  V.Good  |    No      |   O(n²)\n\n";

    cout << "Key Insights:\n";
    cout << "- HRRN combines advantages of FCFS (prevents starvation) and SJF (minimizes avg wait)\n";
    cout << "- Round Robin is best for interactive systems despite higher context switches\n";
    cout << "- SJF optimal if burst times are known, but impractical in real systems\n";
    cout << "- FCFS simple but generally poor performance for most workloads\n\n";
}

int main() {
    int choice;
    do {
        cout << "\n=== CPU Scheduling Algorithms Simulator ===\n";
        cout << "1. First Come First Served (FCFS)\n";
        cout << "2. Non-Preemptive Shortest Job First (SJF)\n";
        cout << "3. Preemptive Shortest Job First (SJF)\n";
        cout << "4. Non-Preemptive Priority Scheduling\n";
        cout << "5. Preemptive Priority Scheduling\n";
        cout << "6. Round Robin\n";
        cout << "7. Highest Response Ratio Next (HRRN) ⭐ NEW\n";
        cout << "8. View Algorithm Comparison\n";
        cout << "9. Exit\n";
        cout << "Enter your choice: ";
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
                DisplayComparison();
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