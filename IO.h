//
//  IO.h
//  temp
//
#pragma once

#include "CPU.h"

class IO : public CPU
{
    
public:
    bool checkIODevices = false;
    
    int num_of_hard_disks = 0;
    int num_of_printers = 0;
    
    std::vector<std::list<int>> hardDisks;
    std::vector<std::list<int>> printers;
    
    void commandRequestHardDisk(int num);  //command - d
    void commandRequestPrinter(int num);  //command - p
    void commandShowIO(); //command - S i
    
};
