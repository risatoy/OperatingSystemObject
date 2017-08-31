//
//  Memory.h
//
#pragma once

#include "IO.h"


class Memory : public IO
{
private:
    
    struct PCB {
        int pid;
        int priority;
        int begin;
        int end;
        PCB* next;
        PCB* prev;
    };
    
public:
    PCB* head = NULL;
    PCB* tail = NULL;
    
    int gpid = 0;
    
    double size_of_memory = 0;
    
    PCB* findHole(double size);
    PCB* findPidInMem(int wantPid);
    
    void commandAdd(int priority, double size);  //command - A
    void commandTerminate();  //command - t
    void commandFinishWorkHardDisk(int num); //command - D
    void commandFinishWorkPrint(int num); //command - P
    void commandShowMemory();  //command - S m
    
    void removeProcessFromMem();
};
