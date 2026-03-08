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

//4. Round Robin (Preemptive)
void RR(Schedule* scheduler, int n){
     for(int i = 0; i<n-1; i++){
         for(int j =i+1; j<n; j++){
             if(scheduler[j].Arrival < scheduler[i].Arrival){
                 Schedule temp = scheduler[i];
                 scheduler[i] = scheduler[j];
                 scheduler[j] = temp;
             }
         }
     }

     int quantum;
     std::cout << "Enter Time Quantum: ";
     std::cin >> quantum;

     int* remaining = new int[n];
     for(int i=0; i<n; i++) remaining[i] = scheduler[i].Burst;

     char (*gantt)[10] = new char[1000][10];
     int ganttIndex = 0;

     int* queue = new int[1000];
     int front = 0;
     int rear = 0;

     int time = 0;
     int terminated = 0;

     for(int i=0; i<n; i++){
         if(scheduler[i].Arrival == 0){
             queue[rear++] = i;
         }
     }

     while(terminated < n){
         if(front == rear){
             strcpy(gantt[ganttIndex++], "-");
             time++;
             continue;
         }

         int running = queue[front++];
         int slice = quantum;

         std::cout << "Time " << time << ":\n";
         std::cout << "RUNNING :\n";

         std::cout << "PID= " << scheduler[running].PID
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

         std::cout << "READY :\n";
         for(int i = 0; i < n; i++){
             if(i != running && scheduler[i].Arrival <= time && remaining[i] > 0){
                 std::cout << "PID= " << scheduler[i].PID
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
             terminated++;
         }
         std::cout << "\n";
     }

      //Prints the Gantt Chart
     std::cout << "GANTT CHART:\nTime: ";
     for(int i=0; i<ganttIndex; i++) std::cout << i << " ";
     std::cout << "\nPID:  ";
     for(int i=0; i<ganttIndex; i++) std::cout << gantt[i] << " ";
     std::cout << "\n";

     delete[] remaining;
     delete[] gantt;
     delete[] queue;

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
       std::ifstream file("cpu.txt"); //read in text file

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
       
       RR(scheduler,n);
     
       delete[] scheduler;
       return 0;
 }