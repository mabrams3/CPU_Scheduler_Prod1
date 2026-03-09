#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>

struct Schedule {
    char PID[10];
    int Arrival;
    int Burst;
    int Priority;
};

//Creates a menu for the user
int showMenu(){
    int choice;

    while(true){
        std::cout << "\nSelect Scheduling Algorithm\n";
        std::cout << "1. First Come First Served (FCFS)\n";
        std::cout << "2. Shortest Remaining Time First (SRTF)\n";
        std::cout << "3. Priority Scheduling\n";
        std::cout << "4. Round Robin\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter Choice: ";

        std::cin >> choice;

        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        if(choice >= 1 && choice <= 5){
            return choice;
        }

        std::cout << "Invalid menu option. Try again.\n";
    }
}

//First Screen the user sees when operating the program
void showTitleScreen() {
    char y;

    while(true){
        std::cout << "=====================================\n";
        std::cout << "           CPU SCHEDULER SIM         \n";
        std::cout << "=====================================\n";
        std::cout << "Press (Y) to Continue..."; //Y is for Yes

        std::cin >> y;

        if(y == 'Y' || y == 'y'){
            break;
        }

        std::cout << "Invalid input. Press Y to continue.\n";
    }
}

//1. First Come First Served
void FCFS(Schedule* scheduler, int n){
    std::ofstream out("output_FCFS.txt"); //output file

    int* completion = new int[n];
    int* turnaround = new int[n];
    int* wait = new int[n];

    for(int i=0; i<n; i++) completion[i]=0;

    //this for loop sorts the processes based on arrival time. The lowest time will be stored in the scheduler variable
    for(int i = 0; i < n-1; i++){
        for(int j = i+1; j<n; j++){
            if(scheduler[j].Arrival < scheduler[i].Arrival){
                Schedule temp = scheduler[i];
                scheduler[i] = scheduler[j];
                scheduler[j] = temp;
            }
        }
    }

    //the remaining variable keeps track of how much time each process has left in the scheduler
    int* remaining = new int[n];

    //Then the variable remaining will keep incrementing until the remaining time at any process index i is equal to the burst time of the process.
    for(int i=0; i<n; i++) remaining[i] = scheduler[i].Burst;

    //Gantt Chart -- records each process as it enters the scheduler
    char (*gantt)[10] = new char[1000][10];
    int ganttIndex = 0;

    int time = 0;
    int running = -1;
    int terminated = 0;

    //While the process is still in the scheduler:
    while(terminated < n)
    {
        //Intialize the system
        if(running == - 1)
        {
            //Checks to see if any process has arrived or any process has time left to run
            for(int i = 0; i<n; i++){
                if(scheduler[i].Arrival <= time && remaining[i] > 0){
                    running = i;
                    break;
                }
            }
        }

        out << "Time " << time << ":\n";

        out << "RUNNING :\n";

        //If the process is in the running state:
        if(running != -1){
            out << "PID= " << scheduler[running].PID //Prints all the processes information as it runs
                      << " Arr= " << scheduler[running].Arrival
                      << " Burst= " <<scheduler[running].Burst
                      << " Rem= " << remaining[running]
                      << " Prio= " << scheduler[running].Priority
                      << " State = RUNNING\n";
            strcpy(gantt[ganttIndex++], scheduler[running].PID); //Adds the process to the gantt chart array to be printed at the end
        } else {
            strcpy(gantt[ganttIndex++], "-");
        }

        //if the process is in the ready state:
        out << "READY :\n";
        for(int i = 0; i < n; i++){
            if(i != running && scheduler[i].Arrival <= time && remaining[i] > 0){
                out << "PID= " << scheduler[i].PID
                          << " Arr= " << scheduler[i].Arrival
                          << " Burst= " <<scheduler[i].Burst
                          << " Rem= " << remaining[i]
                          << " Prio= " << scheduler[i].Priority
                          << " State = READY\n";
            }
        }

        if(running != -1){
            remaining[running]--; //if the process was running then subtract how much the process ran for in that instance
            if(remaining[running] == 0){ //if the process has no more remaining time then terminate it from the scheduler
                completion[running] = time+1;
                terminated++;
                running = -1;
            }
        }

        out << "\n";
        time++; //increment the time
    }

    for(int i=0;i<n;i++){
        turnaround[i] = completion[i] - scheduler[i].Arrival;
        wait[i] = turnaround[i] - scheduler[i].Burst;
    }

    out << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for(int i=0;i<n;i++){
        out << scheduler[i].PID << "\t\t"
            << scheduler[i].Arrival << "\t\t"
            << scheduler[i].Burst << "\t\t\t"
            << completion[i] << "\t\t\t"
            << turnaround[i] << "\t\t"
            << wait[i] << "\n";
    }

    out << "\nGANTT CHART\n";
    out << "Time: ";
    for(int i=0; i<ganttIndex; i++) out << i << " ";
    out << "\nPID:  ";
    for(int i=0; i<ganttIndex; i++) out << gantt[i] << " ";
    out << "\n";

    out.close();

    delete[] remaining;
    delete[] gantt;
    delete[] completion;
    delete[] wait;
    delete[] turnaround;

    std::cout << "Output File: output_FCFS.txt Generated!\n";

    //Re-Prints the Menu for another algorithm selection
    char choice;

    while(true){
        std::cout << "Continue? (Y)es or (N)o: ";
        std::cin >> choice;

        if(choice == 'Y' || choice == 'y'){
            return;
        }
        else if(choice == 'N' || choice == 'n'){
            std::cout << "Exiting Program.\n";
            exit(0);
        }
        else{
            std::cout << "Invalid selection.\n";
        }
    }

}

//2. Shortest Remaining Time First
void SRTF(Schedule* scheduler, int n){

    std::ofstream out("output_SRTF.txt"); //output file

    int* completion = new int[n];
    int* turnaround = new int[n];
    int* wait = new int[n];

    for(int i=0; i<n; i++) completion[i]=0;

     for(int i = 0; i<n-1; i++){
         for(int j =i+1; j<n; j++){
             if(scheduler[j].Arrival < scheduler[i].Arrival){
                 Schedule temp = scheduler[i];
                 scheduler[i] = scheduler[j];
                 scheduler[j] = temp;
             }
         }
     }

     int* remaining = new int[n];
     for(int i=0; i<n; i++) remaining[i] = scheduler[i].Burst;

     char (*gantt)[10] = new char[1000][10];
     int ganttIndex = 0;

     int time = 0;
     int running = -1;
     int terminated = 0;

     while(terminated < n){
         int shortest = -1; //sets the variable so the algorithm will pick the shortest remaining time
         for(int i=0; i<n; i++){
             if(scheduler[i].Arrival <= time && remaining[i] > 0){
                 if(shortest == -1 || remaining[i] < remaining[shortest]){ //if the process has the shortest remaining time or the remaining time of the indexed process is less than the remaining of the shortest time then shortest is set to i.
                     shortest = i;
                 }
             }
         }

         running = shortest; //ensures that the running process is the one with the shortest remaining time

         out << "Time " << time << ":\n";
         out << "RUNNING :\n";

         //If the process is in the running state:
         if(running != -1){
             out << "PID= " << scheduler[running].PID
                        << " Arr= " << scheduler[running].Arrival
                        << " Burst= " <<scheduler[running].Burst
                        << " Rem= " << remaining[running]
                        << " Prio= " << scheduler[running].Priority
                        << " State = RUNNING\n";
              strcpy(gantt[ganttIndex++], scheduler[running].PID);
          } else {
              strcpy(gantt[ganttIndex++], "-");
          }

          //if the process is in the ready state
          out << "READY :\n";
          for(int i = 0; i < n; i++){
              if(i != running && scheduler[i].Arrival <= time && remaining[i] > 0){
                  out << "PID= " << scheduler[i].PID
                            << " Arr= " << scheduler[i].Arrival
                            << " Burst= " <<scheduler[i].Burst
                            << " Rem= " << remaining[i]
                            << " Prio= " << scheduler[i].Priority
                            << " State = READY\n";
              }
          }

          if(running != -1){
              remaining[running]--;
              if(remaining[running] == 0){
                completion[running] = time+1;
                terminated++;
              }
          }

          out << "\n";
          time++;
      }
      for(int i=0;i<n;i++){
        turnaround[i] = completion[i] - scheduler[i].Arrival;
        wait[i] = turnaround[i] - scheduler[i].Burst;
    }

    out << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for(int i=0;i<n;i++){
        out << scheduler[i].PID << "\t\t"
            << scheduler[i].Arrival << "\t\t"
            << scheduler[i].Burst << "\t\t\t"
            << completion[i] << "\t\t\t"
            << turnaround[i] << "\t\t"
            << wait[i] << "\n";
    }

    out << "\nGANTT CHART\n";
    out << "Time: ";
    for(int i=0; i<ganttIndex; i++) out << i << " ";
    out << "\nPID:  ";
    for(int i=0; i<ganttIndex; i++) out << gantt[i] << " ";
    out << "\n";

    out.close();

      delete[] remaining;
      delete[] gantt;
      delete[] completion;
      delete[] turnaround;
      delete[] wait;

    std::cout << "Output File: output_SRTF.txt Generated!\n";

    char choice;
     while(true){
        std::cout << "Continue? (Y)es or (N)o: ";
        std::cin >> choice;
        
        if(choice == 'Y' || choice == 'y'){
            return;
        }
        else if(choice == 'N' || choice == 'n'){
            std::cout << "Exiting Program.\n";
            exit(0);
        }
        else{
            std::cout << "Invalid selection.\n";
        }
    }
 }

 //3. Priority Scheduler (Non-Preemptive)
 void Priority(Schedule* scheduler, int n){

    std::ofstream out("output_priority.txt"); //output file

    int* completion = new int[n];
    int* turnaround = new int[n];
    int* wait = new int[n];

    for(int i=0; i<n; i++) completion[i]=0;


     for(int i = 0; i<n-1; i++){
         for(int j =i+1; j<n; j++){
             if(scheduler[j].Arrival < scheduler[i].Arrival){
                 Schedule temp = scheduler[i];
                 scheduler[i] = scheduler[j];
                 scheduler[j] = temp;
             }
         }
     }

     int* remaining = new int[n];
     for(int i=0; i<n; i++) remaining[i] = scheduler[i].Burst;

     char (*gantt)[10] = new char[1000][10];
     int ganttIndex = 0;

     int time = 0;
     int running = -1;
     int terminated = 0;

     while(terminated < n){
         if(running == -1){
             int priority = -1; //if running is equal to -1 then the first process priority is set to -1 to ensure the process at arrial time zero can enter the scheduler

             for(int i=0; i<n; i++){
                 if(scheduler[i].Arrival <= time && remaining[i] > 0){
                     if(priority == -1 || scheduler[i].Priority < scheduler[priority].Priority){ //if the priority variable is equal to -1 or the priority of the process at index i is less than the priority of the process at the next index then priority will be set to i for the next comparison
                         priority = i;
                     }
                 }
             }

             running = priority;
         }

         out << "Time " << time << ":\n";

         //if the process is in the running state:
         out << "RUNNING :\n";
         if(running != -1){
             out << "PID= " << scheduler[running].PID
                        << " Arr= " << scheduler[running].Arrival
                        << " Burst= " <<scheduler[running].Burst
                        << " Rem= " << remaining[running]
                        << " Prio= " << scheduler[running].Priority
                        << " State = RUNNING\n";
              strcpy(gantt[ganttIndex++], scheduler[running].PID);
          } else {
              strcpy(gantt[ganttIndex++], "-");
          }

          //if the process is in the ready state:
          out << "READY :\n";
          for(int i = 0; i < n; i++){
              if(i != running && scheduler[i].Arrival <= time && remaining[i] > 0){
                  out << "PID= " << scheduler[i].PID
                            << " Arr= " << scheduler[i].Arrival
                            << " Burst= " <<scheduler[i].Burst
                            << " Rem= " << remaining[i]
                            << " Prio= " << scheduler[i].Priority
                            << " State = READY\n";
              }
          }

          if(running != -1){
              remaining[running]--;
              if(remaining[running] == 0){
                completion[running] = time+1;
                terminated++;
                running = -1;
              }
          }

          out << "\n";
          time++;
      }

    for(int i=0;i<n;i++){
        turnaround[i] = completion[i] - scheduler[i].Arrival;
        wait[i] = turnaround[i] - scheduler[i].Burst;
    }

    out << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for(int i=0;i<n;i++){
        out << scheduler[i].PID << "\t\t"
            << scheduler[i].Arrival << "\t\t"
            << scheduler[i].Burst << "\t\t\t"
            << completion[i] << "\t\t\t"
            << turnaround[i] << "\t\t"
            << wait[i] << "\n";
    }

    out << "\nGANTT CHART\n";
    out << "Time: ";
    for(int i=0; i<ganttIndex; i++) out << i << " ";
    out << "\nPID:  ";
    for(int i=0; i<ganttIndex; i++) out << gantt[i] << " ";
    out << "\n";

    out.close();

      delete[] remaining;
      delete[] gantt;
      delete[] completion;
      delete[] turnaround;
      delete[] wait;

    std::cout << "Output File: output_priority.txt Generated!\n";

    char choice;
    while(true){
        std::cout << "Continue? (Y)es or (N)o: ";
        std::cin >> choice;
        
        if(choice == 'Y' || choice == 'y'){
            return;
        }
        else if(choice == 'N' || choice == 'n'){
            std::cout << "Exiting Program.\n";
            exit(0);
        }
        else{
            std::cout << "Invalid selection.\n";
        }
    }
 }

 //4. Round Robin Scheduler
 void RR(Schedule* scheduler, int n){
    std::ofstream out("output_rr.txt"); //output file

    int* completion = new int[n];
    int* turnaround = new int[n];
    int* wait = new int[n];

    for(int i=0; i<n; i++) completion[i]=0;

     for(int i = 0; i<n-1; i++){
         for(int j =i+1; j<n; j++){
             if(scheduler[j].Arrival < scheduler[i].Arrival){
                 Schedule temp = scheduler[i];
                 scheduler[i] = scheduler[j];
                 scheduler[j] = temp;
             }
         }
     }

     int quantum; //sets up the time slicing for the scheduler
     std::cout << "Enter Time Quantum: "; //allows the user to set a time quantum (for the input a time quantum of 2 is recommended)
     std::cin >> quantum;

     int* remaining = new int[n];
     for(int i=0; i<n; i++) remaining[i] = scheduler[i].Burst;

     char (*gantt)[10] = new char[1000][10];
     int ganttIndex = 0;

     int* queue = new int[1000]; //helps in keeping the order together (for which process gets the scheduler next)
     int front = 0; //front of the queue 
     int rear = 0; //back of the queue

     int time = 0;
     int terminated = 0;

     for(int i=0; i<n; i++){
         if(scheduler[i].Arrival == 0){ //when the first process arrives then it will be placed at the back of the queue when it's preempted by another process.
             queue[rear++] = i;
         }
     }

     while(terminated < n){
         if(front == rear){ //if the front of the queue and the rear of the queue are at the same element then the process will be recorded in the gantt chart 
             strcpy(gantt[ganttIndex++], "-");
             time++;
             continue;
         }

         int running = queue[front++];
         int slice = quantum;

         out << "Time " << time << ":\n";

         //if the process is in the running state:
         out << "RUNNING :\n";

         out << "PID= " << scheduler[running].PID
                        << " Arr= " << scheduler[running].Arrival
                        << " Burst= " <<scheduler[running].Burst
                        << " Rem= " << remaining[running]
                        << " Prio= " << scheduler[running].Priority
                        << " State = RUNNING\n";

         while(slice > 0 && remaining[running] > 0){
             strcpy(gantt[ganttIndex++], scheduler[running].PID);

             remaining[running]--;
             time++;
             slice--;

             for(int i=0; i<n; i++){
                 if(scheduler[i].Arrival == time){
                     queue[rear++] = i;
                 }
             }
         }

         //if the process is in the ready state:
         out << "READY :\n";
         for(int i = 0; i < n; i++){
             if(i != running && scheduler[i].Arrival <= time && remaining[i] > 0){
                 out << "PID= " << scheduler[i].PID
                           << " Arr= " << scheduler[i].Arrival
                           << " Burst= " <<scheduler[i].Burst
                           << " Rem= " << remaining[i]
                           << " Prio= " << scheduler[i].Priority
                           << " State = READY\n";
             }
         }

         if(remaining[running] > 0){
             queue[rear++] = running;
         }
         else{
            completion[running] = time+1;
            terminated++;
         }
         out << "\n";
     }

    for(int i=0;i<n;i++){
        turnaround[i] = completion[i] - scheduler[i].Arrival;
        wait[i] = turnaround[i] - scheduler[i].Burst;
    }

    out << "PID\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n";
    for(int i=0;i<n;i++){
        out << scheduler[i].PID << "\t\t"
            << scheduler[i].Arrival << "\t\t"
            << scheduler[i].Burst << "\t\t\t"
            << completion[i] << "\t\t\t"
            << turnaround[i] << "\t\t"
            << wait[i] << "\n";
    }

    out << "\nGANTT CHART\n";
    out << "Time: ";
    for(int i=0; i<ganttIndex; i++) out << i << " ";
    out << "\nPID:  ";
    for(int i=0; i<ganttIndex; i++) out << gantt[i] << " ";
    out << "\n";

    out.close();

      delete[] remaining;
      delete[] gantt;
      delete[] completion;
      delete[] turnaround;
      delete[] wait;

    std::cout << "Output File: output_rr.txt Generated!\n";

     char choice;
     while(true){
         std::cout << "Continue? (Y)es or (N)o: ";
         std::cin >> choice;

         if(choice == 'Y' || choice == 'y'){
             return;
         }
         else if(choice == 'N' || choice == 'n'){
             std::cout << "Exiting Program.\n";
             exit(0);
         }
         else{
             std::cout << "Invalid selection.\n";
         }
     }
}


   int main() {
       std::ifstream file("cpu.txt"); //read in input text file

       if(!file){
           std::cout << "Error opening file\n";
           return 1;
       }

       int n = 0;
       std::string line;
       std::getline(file, line);

       while(std::getline(file, line)) n++;
       file.clear();
       file.seekg(0);
       std::getline(file, line);

       Schedule* scheduler = new Schedule[n];
       int idx = 0;

       while(std::getline(file, line)){
           std::istringstream iss(line);
           iss >> scheduler[idx].PID >> scheduler[idx].Arrival >> scheduler[idx].Burst >> scheduler[idx].Priority;
           idx++;
       }

       for (int i = 0; i < n; i++) { //prints the PCB
           std::cout << "PID: " << scheduler[i].PID
                     << "  Arrival: " << scheduler[i].Arrival
                     << "  Burst: " << scheduler[i].Burst
                     << "  Priority: " << scheduler[i].Priority
                     << std::endl;
       }

       showTitleScreen();

       while(true){
           int choice = showMenu();

           switch(choice) {
             case 1:
               std::cout << "Running FCFS Scheduler...\n";
               FCFS(scheduler,n);
               break;

             case 2:
               std::cout << "Running SRTF Scheduler...\n";
               SRTF(scheduler,n);
               break;

             case 3:
               std::cout << "Running Priority Scheduler...\n";
               Priority(scheduler, n);
               break;

             case 4:
               std::cout << "Running Round Robin Scheduler...\n";
               RR(scheduler, n);
               break;

             case 5:
               std::cout << "Exiting Program.\n";
               delete[] scheduler;
               return 0;

             default:
               std::cout << "Inavlid Selection.\n";
           }
       }
       delete[] scheduler;
       return 0;
  }
