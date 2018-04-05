#include <iostream>
#include <unistd.h>
#include "BrickPi3.h"
#include <sstream>

using namespace std;

/*
 * Program to test how much power it takes to achieve a certain speed and how much power it takes to turn a certain amount of degrees
 * 
 * Right motor = B
 * Left motor = C
 * Turret motor = A
 */

BrickPi3 arnold;

void stopArnold(uint8_t port1, uint8_t port2){
    // Reset Arnold
    arnold.set_motor_power(port1, 0);
    arnold.set_motor_power(port2, 0);
}

void forwardBack(uint8_t port1, uint8_t port2, uint8_t power){
    // Move Arnold forward
    arnold.set_motor_power(port1, power);
    arnold.set_motor_power(port2, power);
}

void turn(uint8_t port1, uint8_t port2, int32_t timeSec, uint8_t power){
    // Code below caused Arnold to shake after a turn.
    //stopArnold(port1, port2);
    //usleep(100000);
    //arnold.set_motor_position_relative(port1, degrees);
    //arnold.set_motor_position_relative(port2, -degrees);
    
    arnold.set_motor_power(port1, power);
    arnold.set_motor_power(port2, -power);
    sleep(timeSec);
    stopArnold(port1, port2);
    
}

int main()
{
    arnold.detect();
    uint8_t Power = 0;
    uint32_t Degrees = 0;
    string userInput;
    while(true){
        cout << "Type options for power options, break to terminate the program, stop to stop Arnold, move to move Arnold forward or back, turn to turn Arnold." << endl;
        cin >> userInput;
        
        if(userInput == "options"){
            while(true){
                cout << "Give the power value: " << endl;
                cin >> userInput;
                stringstream inputStream(userInput);
                if(inputStream >> Power){
                    break;
                }else if(userInput == "break"){
                    Power = 0;
                    cout << "Power is now set to 0" << endl;
                    break;
                }
                cout << "Invalid conversion from input to string" << endl;
            }
            
        }else if(userInput == "break"){
            break;
        }else if(userInput == "stop"){
            stopArnold(PORT_B, PORT_C);
        }else if(userInput == "move"){
            cout << "Type back to move Arnold backwards, type anything else to move him forward" << endl;
            cin >> userInput;
            if(userInput != "back"){
                forwardBack(PORT_B, PORT_C, Power);
            }else{
                forwardBack(PORT_B, PORT_C, -Power);
            }
        }else if(userInput == "turn"){
            cout << "Enter degrees" << endl;
            cin >> userInput;
            stringstream inputStreamTurn(userInput);
            if(inputStreamTurn >> Degrees){
                turn(PORT_B, PORT_C, Degrees, Power);
                continue;
            }
            cout << "invalid conversion" << endl;
        }
        
    }
    
	return 0;
}
