//
//  CPU.h
//
#pragma once

class CPU
{
    
public:
    int current_process_in_CPU_priority = -1;
    int current_process_in_CPU_pid = -1;
    
    std::list<int> readyQueuePrior4;
    std::list<int> readyQueuePrior3;
    std::list<int> readyQueuePrior2;
    std::list<int> readyQueuePrior1;
    std::list<int> readyQueuePrior0;
    
    void addProcessToReadyQueue(int priority, int gpid);
    void commandShowReadyQueueAndCPU();  //command - S r
    
    void removeProcessfromCPU();
};
