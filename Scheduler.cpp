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

void showTitleScreen() {
    char y;

    while(true){
        std::cout << "=====================================\n";
        std::cout << "           CPU SCHEDULER SIM         \n";
        std::cout << "=====================================\n";
        std::cout << "Press (Y) to Continue...";

        std::cin >> y;

        if(y == 'Y' || y == 'y'){
            break;
        }

        std::cout << "Invalid input. Press Y to continue.\n";
    }
}

//1. First Come First Served
void FCFS(Schedule* scheduler, int n){

    for(int i = 0; i < n-1; i++){
        for(int j = i+1; j<n; j++){
            if(scheduler[j].Arrival < scheduler[i].Arrival){
                Schedule temp = scheduler[i];
                scheduler[i] = scheduler[j];
                scheduler[j] = temp;
            }
        }
    }

    int* remaining = new int[n];
    for(int i=0; i<n; i++) remaining[i] = scheduler[i].Burst;

    //Gantt Chart
    char (*gantt)[10] = new char[1000][10];
    int ganttIndex = 0;

    int time = 0;
    int running = -1;
    int terminated = 0;

    while(terminated < n)
    {
        if(running == - 1)
        {
            for(int i = 0; i<n; i++){
                if(scheduler[i].Arrival <= time && remaining[i] > 0){
                    running = i;
                    break;
                }
            }
        }

        std::cout << "Time " << time << ":\n";

        std::cout << "RUNNING :\n";
        if(running != -1){
            std::cout << "PID= " << scheduler[running].PID
                      << " Arr= " << scheduler[running].Arrival
                      << " Burst= " <<scheduler[running].Burst
                      << " Rem= " << remaining[running]
                      << " Prio= " << scheduler[running].Priority
                      << " State = RUNNING\n";
            strcpy(gantt[ganttIndex++], scheduler[running].PID);
        } else {
            strcpy(gantt[ganttIndex++], "-");
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

        if(running != -1){
            remaining[running]--;
            if(remaining[running] == 0){
                terminated++;
                running = -1;
            }
        }

        std::cout << "\n";
        time++;
    }

    //Prints the Gantt Chart
    std::cout << "GANTT CHART:\nTime: ";
    for(int i=0; i<ganttIndex; i++) std::cout << i << " ";
    std::cout << "\nPID:  ";
    for(int i=0; i<ganttIndex; i++) std::cout << gantt[i] << " ";
    std::cout << "\n";

    delete[] remaining;
    delete[] gantt;

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
         int shortest = -1;
         for(int i=0; i<n; i++){
             if(scheduler[i].Arrival <= time && remaining[i] > 0){
                 if(shortest == -1 || remaining[i] < remaining[shortest]){
                     shortest = i;
                 }
             }
         }

         running = shortest;

         std::cout << "Time " << time << ":\n";
         std::cout << "RUNNING :\n";
         if(running != -1){
             std::cout << "PID= " << scheduler[running].PID
                        << " Arr= " << scheduler[running].Arrival
                        << " Burst= " <<scheduler[running].Burst
                        << " Rem= " << remaining[running]
                        << " Prio= " << scheduler[running].Priority
                        << " State = RUNNING\n";
              strcpy(gantt[ganttIndex++], scheduler[running].PID);
          } else {
              strcpy(gantt[ganttIndex++], "-");
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

          if(running != -1){
              remaining[running]--;
              if(remaining[running] == 0){
                  terminated++;
                  //running = -1;
              }
          }

          std::cout << "\n";
          time++;
      }

      //Prints the Gantt Chart
      std::cout << "GANTT CHART:\nTime: ";
      for(int i=0; i<ganttIndex; i++) std::cout << i << " ";
      std::cout << "\nPID:  ";
      for(int i=0; i<ganttIndex; i++) std::cout << gantt[i] << " ";
      std::cout << "\n";

      delete[] remaining;
      delete[] gantt;

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

 //3. Priority Scheduler
 void Priority(Schedule* scheduler, int n){
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
             int priority = -1;

             for(int i=0; i<n; i++){
                 if(scheduler[i].Arrival <= time && remaining[i] > 0){
                     if(priority == -1 || scheduler[i].Priority < scheduler[priority].Priority){
                         priority = i;
                     }
                 }
             }

             running = priority;
         }

         std::cout << "Time " << time << ":\n";
         std::cout << "RUNNING :\n";
         if(running != -1){
             std::cout << "PID= " << scheduler[running].PID
                        << " Arr= " << scheduler[running].Arrival
                        << " Burst= " <<scheduler[running].Burst
                        << " Rem= " << remaining[running]
                        << " Prio= " << scheduler[running].Priority
                        << " State = RUNNING\n";
              strcpy(gantt[ganttIndex++], scheduler[running].PID);
          } else {
              strcpy(gantt[ganttIndex++], "-");
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

          if(running != -1){
              remaining[running]--;
              if(remaining[running] == 0){
                  terminated++;
                  running = -1;
              }
          }

          std::cout << "\n";
          time++;
      }

      //Prints the Gantt Chart
      std::cout << "GANTT CHART:\nTime: ";
      for(int i=0; i<ganttIndex; i++) std::cout << i << " ";
      std::cout << "\nPID:  ";
      for(int i=0; i<ganttIndex; i++) std::cout << gantt[i] << " ";
      std::cout << "\n";

      delete[] remaining;
      delete[] gantt;

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
