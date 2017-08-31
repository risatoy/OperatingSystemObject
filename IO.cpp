//
//  IO.cpp
//

#include <iostream>
#include <string>
#include <sstream>
#include <stddef.h>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <math.h>

#include "CPU.h"
#include "IO.h"

using namespace std;


//command - d #
void IO::commandRequestHardDisk(int num){
    
    if(current_process_in_CPU_pid != -1){
        //push the process that was in CPU into the hard disk #
        hardDisks[num-1].push_back(current_process_in_CPU_pid);
        
        //remove from CPU
        removeProcessfromCPU();
    }
    else{  //when there is no process in cpu
        cout << "*Error; There is no process in CPU." << endl;
        return;
    }
    
    
}

//command - p #
void IO::commandRequestPrinter(int num){
    if(current_process_in_CPU_pid != -1){
        //push the process that was in CPU into the printer #
        printers[num-1].push_back(current_process_in_CPU_pid);
        
        //remove from CPU
        removeProcessfromCPU();
    }
    else{
        cout << "*Error; There is no process in CPU." << endl;
        return;
    }
    
}

//command - S i
void IO::commandShowIO(){
    //print out hard disks are currently used and in ready queue
    for(int i = 0; i < num_of_hard_disks; i++){
        if(!hardDisks[i].empty()){
            checkIODevices = true;
            cout << "Hard Disk " << i+1 << ": P" << hardDisks[i].front();
            if(hardDisks[i].size() > 1){
                cout << " ( ";
                for(list<int>::iterator it= next(hardDisks[i].begin(), 1); it !=hardDisks[i].end(); ++it){
                    cout << "P" << *it << " ";
                }
                cout << ")";
            }
            cout << endl;
        }
    }
    
    
    //print out printers are currently used and in ready queue
    for(int j = 0; j < num_of_printers; j++){
        if(!printers[j].empty()){
            checkIODevices = true;
            cout << "Printer " << j+1 << ": P" << printers[j].front();
            if(printers[j].size() > 1){
                cout << " ( ";
                for(list<int>::iterator it= next(printers[j].begin(), 1); it !=printers[j].end(); ++it){
                    cout << "P" << *it << " ";
                }
                cout << ")";
            }
            cout << endl;
        }
    }
    
    if(!checkIODevices){
        cout << "There is no process in I/O devices." << endl;
    }
    else
        cout << "*The processes in ( ) are in its Ready Queue." << endl;
    
}

