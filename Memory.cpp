//
//  Memory.cpp
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

#include "IO.h"
#include "Memory.h"

using namespace std;

//check if there is a hole in memory that is bigger than the size of the new process
Memory::PCB* Memory::findHole(double size){
    PCB* current;
    current = head;
    
    while(current != tail){
        
        if(current -> next -> begin - current -> end > size)
            return current;
        else
            current = current -> next;
    }
    return NULL;
}

//find pid in memory
Memory::PCB* Memory::findPidInMem(int wantPid){
    PCB* check;
    check = head;
    
    while(check != tail){
        if(check->pid == wantPid)
            return check;
        else
            check = check -> next;
    }
    return NULL;
}

//command - A priority size
void Memory::commandAdd(int priority, double size){
    PCB* firstNode = NULL;
    PCB* newNode;
    PCB* nextNode;
    PCB* limitNode;
    
    //create first node
    if(head == NULL){
        firstNode = new PCB;
        firstNode -> end = -1;
        head = firstNode;
    }
    
    newNode = new PCB;
    newNode -> pid = ++gpid;
    newNode -> priority = priority;
    
    addProcessToReadyQueue(priority, gpid);
    
    if(gpid == 1){
        //create first node
        newNode -> begin = 0;
        newNode -> end = size -1;
        newNode -> prev = firstNode;
        firstNode -> next = newNode;
        
        //add limit node
        limitNode = new PCB;
        limitNode -> begin = size_of_memory;
        newNode -> next = limitNode;
        limitNode -> prev = newNode;
        tail = limitNode;
    }
    else{
        PCB* node = findHole(size);
        if(node == NULL){ // if there is no hole
            cout << "*Error; There is no enough space in Memory!" << endl;
        }
        else{
            nextNode = node -> next;
            newNode -> next = nextNode;
            newNode -> prev = node;
            node -> next = newNode;
            nextNode -> prev = newNode;
            newNode -> begin = newNode -> prev -> end +1;
            newNode -> end = newNode -> begin + size -1;
        }
        
    }
}

//command - t
void Memory::commandTerminate(){
    
    //remove the process from memory
    removeProcessFromMem();
    //remove from CPU
    removeProcessfromCPU();
    
}

//remove process from Mem when terminate
void Memory::removeProcessFromMem(){
    PCB* checkPid;
    PCB* sucNode;
    PCB* preNode;
    
    checkPid = head;
    
    while(checkPid -> pid != current_process_in_CPU_pid){
        checkPid = checkPid -> next;
    }
    sucNode = checkPid -> next;
    preNode = checkPid -> prev;
    
    sucNode -> prev = preNode;
    if(preNode != 0)
        preNode -> next = sucNode;
    if(checkPid == head)
        head = sucNode;
    
}

//command - D #
void Memory::commandFinishWorkHardDisk(int num){
    PCB* temp;
    
    if(!hardDisks[num-1].empty()){
        temp = findPidInMem(hardDisks[num-1].front());
        if(temp == NULL){
            cout << "*Error; didn't find the pid in Memory" << endl;
        }
        else{
            addProcessToReadyQueue(temp -> pid, hardDisks[num-1].front());
            hardDisks[num-1].pop_front();
        }
    }
    
    if(hardDisks[num-1].empty())
        checkIODevices = false;
}

//command - P #
void Memory::commandFinishWorkPrint(int num){
    PCB* temp;
    
    if(!printers[num-1].empty()){
        temp = findPidInMem(printers[num-1].front());
        if(temp == NULL){
            cout << "*Error; didn't find the pid in memory" << endl;
        }
        else{
            addProcessToReadyQueue(temp -> pid, printers[num-1].front());
            printers[num-1].pop_front();
        }
    }
    
    if(printers[num-1].empty())
        checkIODevices = false;
}

//command - S m
void Memory::commandShowMemory(){
    PCB* p;
    p = head -> next;
    while(p != tail){
        cout << "P" << p -> pid << ": " << p -> begin << " - " << p -> end << endl;
        p = p -> next;
    }
}

