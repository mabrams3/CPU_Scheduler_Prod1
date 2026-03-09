# CPU_Scheduler_Prod1
CPU Scheduler Operating Systems Project 1

All Scheduling alogrithms have been hand designed (FCFS, SRTF, Priority, and Round Robin)
Implementation of FCFS has begun in the terminal

Added a menu function for navigating between different scheduling algorithms. 
CPU.txt created 
The program reads in the text file and generates the PCB table.

Changes made to the array structure. Vector logic is not allowed so the program was changed to accomodate that
FCFS Scheduling algorithm is complete. Still trying to adjust the print so that the final shows the last output. i.e P5 should terminate at 27 but the code ends at 26

Work started and completed on the SRTF algorithm
Quick to program - used the algorithm from FCFS and modified the burst time priority

Work started on the priority scheduling algorithm
Finished working on the priority scheduler
Worked on fail proofing the menu choices and created a redundant menu system

Work started on the Round Robin Scheduler
Finsihed work on Round Robin Scheduler

-- How to Run the Program --
1. The main program is Scheduler.cpp
2. All the other scheduling algorithms can be run separatly in their own (cpp) files
3. To Run the program type the command: g++ (file name.cpp) -o (file name) --> compile
                                        ./file name --> execute
4. cpu.txt is the input file
5. On launch the menu is navigatable through written commands

Something to consider: 
1. cpu2.txt is the main input file (cpu.txt has processes arriving at the same time and will schedule properly but it will appear different in order between the two processes arriving at the same time).

Added output file creation to the main program Scheduler.cpp not the sub programs (FCFS, etc.) 
- The subfile will print their output directly into the terminal

Project Complete!
