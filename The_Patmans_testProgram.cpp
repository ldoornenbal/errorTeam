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
    usleep(timeSec);
    stopArnold(port1, port2);
    
}

int main()
{
    arnold.detect();
    uint8_t Power = 0;
    uint32_t Degrees = 0;
    string userInput;
    uint8_t portRight = PORT_B;
    uint8_t portLeft = PORT_C;
    
    cout << "Hello, I am Arnold!" << endl;
    
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
            stopArnold(portRight, portLeft);
        }else if(userInput == "move"){
            cout << "Type back to move Arnold backwards, type fwd to move Arnold forward, type anything else to move him forward for a limited time" << endl;
            cin >> userInput;
            if(userInput == "back"){
                forwardBack(portRight, portLeft, -Power);
            }else if(userInput == "fwd"){
                forwardBack(portRight, portLeft, Power);
            }else{
                cout << "Enter time in ms" << endl;
                cin >> userInput;
                stringstream inputStreamTurn(userInput);
                if(inputStreamTurn >> Degrees){
                    forwardBack(portRight, portLeft, Power);
                    usleep(1000*Degrees); // usleep uses microseconds, we want miliseconds so: 1000*miliseconds = microseconds.
                    stopArnold(portRight, portLeft);
                }
            }
        }else if(userInput == "turn"){
            cout << "Enter time, in μs" << endl;
            cin >> userInput;
            stringstream inputStreamTurn(userInput);
            if(inputStreamTurn >> Degrees){
                turn(portRight, portLeft, Degrees, Power);
                continue;
            }
            cout << "invalid conversion" << endl;
        }
        
    }
    
    arnold.reset_all();
	return 0;
}
