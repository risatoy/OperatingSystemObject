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

#include "CPU.h"
#include "Memory.h"
#include "IO.h"

using namespace std;

Memory mem;


bool validInput(double input);
void userInputCommands(string userInput);

int main() {
    
    string userInput;
    bool userWantToContinue = true;
    
    //How much RAM memory? (receive # in bytes up to 4000000000)
    cout << "Please enter the number of RAM memory you would like to use (in Bytes): ";
    cin >> mem.size_of_memory;
    while(!validInput(mem.size_of_memory)){
        cin >> mem.size_of_memory;
    }
    
    //How many hard disks? (Enumeration starts with 0)
    cout << "Please enter the number of Hard Disks you would like to use: ";
    cin >> mem.num_of_hard_disks;
    while(!validInput(mem.num_of_hard_disks)){
        cin >> mem.num_of_hard_disks;
    }
    mem.hardDisks.resize(mem.num_of_hard_disks);
    
    //How many printers? (Enumeration starts with 0)
    cout << "Please enter the number of Printers you would like to use: ";
    cin >> mem.num_of_printers;
    while(!validInput(mem.num_of_printers)){
        cin >> mem.num_of_printers;
    }
    mem.printers.resize(mem.num_of_printers);
    
    cout << "/--------------------------------" << endl;
    cout << "You have created a system with" << endl;
    cout << "RAM memory: " << mem.size_of_memory << " bytes" << endl;
    cout << "Hard Disks: " << mem.num_of_hard_disks << endl;
    cout << "Printers: " << mem.num_of_printers << endl;
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
        else if(size < -1 or size > mem.size_of_memory){
            cout << "*Invalid command; The size of the memory has to be non-negative and less than the size of the memory you created." << endl;
            return;
        }
        else{
            mem.commandAdd(priority, size);  //command - A
        }
        
    }
    else if(userInput == "t"){    //command - t
        mem.commandTerminate();
        
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
        else if(num < 1 or num > mem.num_of_hard_disks){
            cout << "*Invalid command; The number must be bigger than 1 and smaller than the number of hard disks you created." << endl;
            return;
        }
        
        mem.commandRequestHardDisk(num);   //command - d
        
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
        else if(num < 1 or num > mem.num_of_printers){
            cout << "*Invalid command; The number must be bigger than 1 and smaller than the number of printers you created." << endl;
            return;
        }
        
        mem.commandRequestPrinter(num);     //command - p
        
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
        
        mem.commandFinishWorkHardDisk(num);     //command - D
        
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
        
        mem.commandFinishWorkPrint(num);    //command - P
        
    }
    else if(userInput.at(0) == 'S'){
        char s;
        char t;
        
        stringstream ss;
        ss << userInput;
        ss >> s >> t;
        
        if(t == 'r'){
            mem.commandShowReadyQueueAndCPU();    //command - S r
        }
        else if(t == 'i'){
            mem.commandShowIO();    //command - S i
        }
        else if (t == 'm'){
            mem.commandShowMemory();    //command - S m
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



