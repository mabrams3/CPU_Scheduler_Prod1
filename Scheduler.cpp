#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>

   struct Schedule {
       std::string PID;
       int Arrival;
       int Burst;
       int Priority;
   };

int showMenu(){
     int choice;

     std::cout << "\nSelect Scheduling Algorithm\n";
     std::cout << "1. First Come First Served (FCFS)\n";
     std::cout << "2. Shortest Remaining Time First (SRTF)\n";
     std::cout << "3. Priority Scheduling\n";
     std::cout << "4. Round Robin\n";
     std::cout << "5. Exit\n";
     std::cout << "Enter Choice: ";

     std::cin >> choice;
     return choice;
}

void showTitleScreen() {
     char y;

     std::cout << "=====================================\n";
     std::cout << "           CPU SCHEDULER SIM         \n";
     std::cout << "=====================================\n";
     std::cout << "Press (Y) to Continue...";

     std::cin >> y;

     if(y != 'Y' && y!= 'y'){
         std::cout << "Continuing ...\n";
     }
 }

   int main() {

      //Read in the CPU.txt input file
       std::ifstream file("cpu.txt"); //read in text file

       if(!file){
           std::cout << "Error opening file\n";
           return 1;
       }


       std::string line;
       std::vector<Schedule> scheduler;

       std::getline(file, line); //skip the first header line

//Print the cpu.txt file into a readible table
       while(std::getline(file, line)){
           Schedule schedule;
           std::istringstream iss(line);
           iss >> schedule.PID >> schedule.Arrival >> schedule.Burst >> schedule.Priority;
           scheduler.push_back(schedule);
       }

       for (const auto& schedule : scheduler) { //prints the PCB
           std::cout << "PID: " << schedule.PID
                     << "  Arrival: " << schedule.Arrival
                     << "  Burst: " << schedule.Burst
                     << "  Priority: " << schedule.Priority
                     << std::endl;
       }

       showTitleScreen();

       int choice = showMenu();

       switch(choice) {
           case 1:
               std::cout << "Running FCFS Scheduler...\n";
               break;
           case 2:
               std::cout << "Running SRTF Scheduler...\n";
               break;
           case 3:
               std::cout << "Running Priority Scheduler...\n";
               break;
           case 4:
               std::cout << "Running Round Robin Scheduler...\n";
               break;
           case 5:
               std::cout << "Exiting Program.\n";
               return 0;
           default:
               std::cout << "Inavlid Selection.\n";
       }


       //Start Logic
       std::cout << "----CPU Scheduling FCFS----\n";
       int time = 0;

      std::cout << "Time " << time << std::endl;
      return 0;
}
