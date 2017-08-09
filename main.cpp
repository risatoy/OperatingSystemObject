//
//  main.cpp
//  osproject
//
//  Created by Risa Toyoshima
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

using namespace std;

double size_of_memory = 0;
int num_of_hard_disks = 0;
int num_of_printers = 0;

struct PCB{
    int pid;
    int priority;
    int begin;
    int end;
    PCB* next;
    PCB* prev;
};


class Memory{
    
public:
    PCB* head = NULL;
    PCB* tail = NULL;
    
    int gpid = 0;
    
    PCB* findHole(double size);
    PCB* findPidInMem(int wantPid);
    void commandAdd(int priority, double size);  //command - A
    void commandFinishWorkHardDisk(int num); //command - D
    void commandFinishWorkPrint(int num); //command - P
    void commandShowMemory();  //command - S m
    
    void removeProcessFromMem();
};

class CPU{
    
public:
    list<int> readyQueuePrior4;
    list<int> readyQueuePrior3;
    list<int> readyQueuePrior2;
    list<int> readyQueuePrior1;
    list<int> readyQueuePrior0;
    
    void addProcessToReadyQueue(int priority, int gpid);
    void commandShowReadyQueueAndCPU();  //command - S r
    
    void removeProcessfromCPU();
};

class IO{
    
public:
    vector<list<int>> hardDisks;
    vector<list<int>> printers;
    
    void commandRequestHardDisk(int num);  //command - d
    void commandRequestPrinter(int num);  //command - p
    void commandShowIO(); //command - S i
    
};

Memory mem;
CPU cpu;
IO io;

int current_process_in_CPU_priority = -1;
int current_process_in_CPU_pid = -1;

bool checkIODevices = false;

bool validInput(double input);
void userInputCommands(string userInput);
void commandTerminate();  //command - t

int main() {
    
    string userInput;
    bool userWantToContinue = true;
    
    //How much RAM memory? (receive # in bytes up to 4000000000)
    cout << "Please enter the number of RAM memory you would like to use (in Bytes): ";
    cin >> size_of_memory;
    while(!validInput(size_of_memory)){
        cin >> size_of_memory;
    }
    
    //How many hard disks? (Enumeration starts with 0)
    cout << "Please enter the number of Hard Disks you would like to use: ";
    cin >> num_of_hard_disks;
    while(!validInput(num_of_hard_disks)){
        cin >> num_of_hard_disks;
    }
    io.hardDisks.resize(num_of_hard_disks);
    
    //How many printers? (Enumeration starts with 0)
    cout << "Please enter the number of Printers you would like to use: ";
    cin >> num_of_printers;
    while(!validInput(num_of_printers)){
        cin >> num_of_printers;
    }
    io.printers.resize(num_of_printers);
    
    cout << "/--------------------------------" << endl;
    cout << "You have created a system with" << endl;
    cout << "RAM memory: " << size_of_memory << " bytes" << endl;
    cout << "Hard Disks: " << num_of_hard_disks << endl;
    cout << "Printers: " << num_of_printers << endl;
    cout << "---------------------------------/" << endl;
    cin.ignore(10000, '\n');

    
    //simulation begins
    while(userWantToContinue){
        cout << "Please enter your command ('c' for command details, 'e' to exit): ";
        getline(cin,userInput);
        
        if(userInput == "e"){
            userWantToContinue = false;
        }
        else{
            userInputCommands(userInput);
        }
    }
    return 0;
}

void userInputCommands(string userInput){
    
    if(userInput.at(0) == 'A'){
        char a;
        int priority = -1;
        double size = -1;
        
        
        stringstream ss;
        ss << userInput;
        ss >> a >> priority >> size;
        
        if(priority == -1 or size == -1){  //when priority or size is empty
            cout << "*Invalid command; Input has to have both priority and size (non-negative)." << endl;
            return;
        }
        
        else if(priority < 0 or 4 < priority){
            cout << "*Invalid command; The priority has to be the number from 0 to 4." << endl;
            return;
        }
        else if(size < -1 or size > size_of_memory){
            cout << "*Invalid command; The size of the memory has to be non-negative and less than the size of the memory you created." << endl;
            return;
        }
        else{
            //command - A
            mem.commandAdd(priority, size);
        }
        
    }
    else if(userInput == "t"){
        commandTerminate();
        
    }
    else if(userInput.at(0) == 'd'){
        char d;
        int num = -1;
        
        stringstream ss;
        ss << userInput;
        ss >> d >> num;
        
        if(num == -1){
            cout << "*Invalid command; Please mention Hard Disk Number." << endl;
            return;
        }
        else if(num < 1 or num > num_of_hard_disks){
            cout << "*Invalid command; The number must be bigger than 1 and smaller than the number of hard disks you created." << endl;
            return;
        }
        
        io.commandRequestHardDisk(num);
        
    }
    else if(userInput.at(0) == 'p'){
        char p;
        int num = -1;
        
        stringstream ss;
        ss << userInput;
        ss >> p >> num;
        
        if(num == -1){
            cout << "*Invalid command; Please mention Printer Number." << endl;
            return;
        }
        else if(num < 1 or num > num_of_printers){
            cout << "*Invalid command; The number must be bigger than 1 and smaller than the number of printers you created." << endl;
            return;
        }
        
        io.commandRequestPrinter(num);
        
    }
    else if(userInput.at(0) == 'D'){
        char e;
        int num = -1;
        
        stringstream ss;
        ss << userInput;
        ss >> e >> num;
        
        if(num == -1){
            cout << "*Invalid command; Please mention Hard Disk Number." << endl;
            return;
        }
        
        mem.commandFinishWorkHardDisk(num);
        
    }
    else if(userInput.at(0) == 'P'){
        char q;
        int num = -1;
        
        stringstream ss;
        ss << userInput;
        ss >> q >> num;
        
        if(num == -1){
            cout << "*Invalid command; Please mention Printer Number." << endl;
            return;
        }
        
        mem.commandFinishWorkPrint(num);
        
    }
    else if(userInput.at(0) == 'S'){
        char s;
        char t;
        
        stringstream ss;
        ss << userInput;
        ss >> s >> t;
        
        if(t == 'r'){
            cpu.commandShowReadyQueueAndCPU();
        }
        else if(t == 'i'){
            io.commandShowIO();
        }
        else if (t == 'm'){
            mem.commandShowMemory();
        }
        else{
            cout << "*Invalid command." << endl;
            return;
        }
    }
    else if(userInput == "c"){
        string line;
        ifstream inputFile("c.txt");
        if(inputFile.is_open()){
            while(getline(inputFile, line)){
                cout << line << endl;
            }
            inputFile.close();
        }
        else
            cout << "unable to open the txt file." << endl;
        
        return;
        
    }
    else{
        cout << "*Invalid command." << endl;
    }
}

//check if the input is valid
bool validInput(double input){
    if(input < 0 or 4000000000 < input){
        cout << "*Invalid input; It has to be the number between 0 to 4 billions." << endl;
        cout << "Please enter the number again: ";
        return false;
    }
    else if(!input){   //check if the input is number
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "*Invalid input; It has to be a number." << endl;
        cout << "Please enter the number again: ";
        return false;
    }
    else{
        return true;
    }
}

//check if there is a hole in memory that is bigger than the size of the new process
PCB* Memory::findHole(double size){
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
PCB* Memory::findPidInMem(int wantPid){
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

//add process to ready queue
void CPU::addProcessToReadyQueue(int priority, int gpid){
    //check if the new process is higher than the process running in CPU
    if(priority > current_process_in_CPU_priority){
        if(current_process_in_CPU_priority == -1){  //when CPU is empty
            current_process_in_CPU_priority = priority;
            current_process_in_CPU_pid = gpid;
        }
        else{  //when CPU has process and and need to push back the old one
            if(current_process_in_CPU_priority == 3)
                readyQueuePrior3.push_back(current_process_in_CPU_pid);
            else if(current_process_in_CPU_priority == 2)
                readyQueuePrior2.push_back(current_process_in_CPU_pid);
            else if(current_process_in_CPU_priority == 1)
                readyQueuePrior1.push_back(current_process_in_CPU_pid);
            else if(current_process_in_CPU_priority == 0)
                readyQueuePrior0.push_back(current_process_in_CPU_pid);
            else
                cout << "*Error; Cannot push back the process that was running in CPU!" << endl;
            
            current_process_in_CPU_priority = priority;
            current_process_in_CPU_pid = gpid;
        }
    }
    else{
        if(priority == 4)
            readyQueuePrior4.push_back(gpid);
        else if(priority == 3)
            readyQueuePrior3.push_back(gpid);
        else if(priority == 2)
            readyQueuePrior2.push_back(gpid);
        else if(priority == 1)
            readyQueuePrior1.push_back(gpid);
        else if(priority == 0)
            readyQueuePrior0.push_back(gpid);
        else{
            cout << "*Error; Cannot add process to Ready Queue." << endl;
            return;
        }
    }
    
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
    
    cpu.addProcessToReadyQueue(priority, gpid);
    
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
void commandTerminate(){
    
    //remove the process from memory
        mem.removeProcessFromMem();
    //remove from CPU
        cpu.removeProcessfromCPU();
    
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

//remove process from CPU when terminate
void CPU::removeProcessfromCPU(){
    current_process_in_CPU_priority = -1;
    current_process_in_CPU_pid = -1;
    
    //push the highest priority process in CPU
    if(!readyQueuePrior4.empty()){
        current_process_in_CPU_priority = 4;
        current_process_in_CPU_pid = readyQueuePrior4.front();
        readyQueuePrior4.pop_front();
    }
    else if(!readyQueuePrior3.empty()){
        current_process_in_CPU_priority = 3;
        current_process_in_CPU_pid = readyQueuePrior3.front();
        readyQueuePrior3.pop_front();
    }
    else if(!readyQueuePrior2.empty()){
        current_process_in_CPU_priority = 2;
        current_process_in_CPU_pid = readyQueuePrior2.front();
        readyQueuePrior2.pop_front();
    }
    else if(!readyQueuePrior1.empty()){
        current_process_in_CPU_priority = 1;
        current_process_in_CPU_pid = readyQueuePrior1.front();
        readyQueuePrior1.pop_front();
    }
    else if(!readyQueuePrior0.empty()){
        current_process_in_CPU_priority = 0;
        current_process_in_CPU_pid = readyQueuePrior0.front();
        readyQueuePrior0.pop_front();
    }

}

//command - d #
void IO::commandRequestHardDisk(int num){
    
    if(current_process_in_CPU_pid != -1){
        //push the process that was in CPU into the hard disk #
        hardDisks[num-1].push_back(current_process_in_CPU_pid);
        
        //remove from CPU
        cpu.removeProcessfromCPU();
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
        cpu.removeProcessfromCPU();
    }
    else{
        cout << "*Error; There is no process in CPU." << endl;
        return;
    }
    
}

//command - D #
void Memory::commandFinishWorkHardDisk(int num){
    PCB* temp;
    
    if(!io.hardDisks[num-1].empty()){
        temp = findPidInMem(io.hardDisks[num-1].front());
        if(temp == NULL){
            cout << "*Error; didn't find the pid in Memory" << endl;
        }
        else{
            cpu.addProcessToReadyQueue(temp -> pid, io.hardDisks[num-1].front());
            io.hardDisks[num-1].pop_front();
        }
    }
    
    if(io.hardDisks[num-1].empty())
        checkIODevices = false;
}

//command - P #
void Memory::commandFinishWorkPrint(int num){
    PCB* temp;
    
    if(!io.printers[num-1].empty()){
        temp = findPidInMem(io.printers[num-1].front());
        if(temp == NULL){
            cout << "*Error; didn't find the pid in memory" << endl;
        }
        else{
            cpu.addProcessToReadyQueue(temp -> pid, io.printers[num-1].front());
            io.printers[num-1].pop_front();
        }
    }
    
    if(io.printers[num-1].empty())
        checkIODevices = false;
}


//command - S r
void CPU::commandShowReadyQueueAndCPU(){
    if(current_process_in_CPU_pid == -1)
        cout << "No process in CPU" << endl;
    else
        cout << "In CPU: P" << current_process_in_CPU_pid<< endl;
    
    if(!readyQueuePrior4.empty()){
        cout << "Ready Queue Priority 4: ";
        for (list<int>::iterator it = readyQueuePrior4.begin(); it !=readyQueuePrior4.end(); ++it)
            cout << "P" << *it << " ";
        cout << endl;
    }
    if(!readyQueuePrior3.empty()){
        cout << "Ready Queue Priority 3: ";
        for (list<int>::iterator it = readyQueuePrior3.begin(); it !=readyQueuePrior3.end(); ++it)
            cout << "P" << *it << " ";
        cout << endl;
    }
    if(!readyQueuePrior2.empty()){
        cout << "Ready Queue Priority 2: ";
        for (list<int>::iterator it = readyQueuePrior2.begin(); it !=readyQueuePrior2.end(); ++it)
            cout << "P" << *it << " ";
        cout << endl;
    }
    if(!readyQueuePrior1.empty()){
        cout << "Ready Queue Priority 1: ";
        for (list<int>::iterator it = readyQueuePrior1.begin(); it !=readyQueuePrior1.end(); ++it)
            cout << "P" << *it << " ";
        cout << endl;
    }
    if(!readyQueuePrior0.empty()){
        cout << "Ready Queue Priority 0: ";
        for (list<int>::iterator it = readyQueuePrior0.begin(); it !=readyQueuePrior0.end(); ++it)
            cout << "P" << *it << " ";
        cout << endl;
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

//command - S m
void Memory::commandShowMemory(){
    PCB* p;
    p = head -> next;
    while(p != tail){
        cout << "P" << p -> pid << ": " << p -> begin << " - " << p -> end << endl;
        p = p -> next;
    }
}
