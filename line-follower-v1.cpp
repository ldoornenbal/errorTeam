#include "BrickPi3.h" //for BrickPi3, this file sets up the BrickPi for usage
#include <iostream> //for cout
#include <unistd.h> //for sleep
#include <signal.h> //for exit signals
#include <iomanip> //for setw and setprecision
using namespace std;

BrickPi3 BP;

// give portnumbers
uint8_t motorleft = PORT_C;
uint8_t motorright = PORT_B;
uint16_t BWsensorleft = PORT_2;
uint16_t BWsensorright = PORT_1;
uint8_t motortop = PORT_D;
uint8_t ultrasonic = PORT_3;

void exit_signal_handler(int signo);

void movement(uint8_t powerleft, uint8_t powerright){
    // Move Arnold forward
    BP.set_motor_power(motorleft, powerleft);
    BP.set_motor_power(motorright, powerright);
}

int main(){
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;
    
    //setting up sensors
	//BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	BP.set_sensor_type(BWsensorleft, SENSOR_TYPE_NXT_LIGHT_ON);
	BP.set_sensor_type(BWsensorright, SENSOR_TYPE_NXT_LIGHT_ON);
	
	sensor_light_t bwleft;
	sensor_light_t bwright;
	
	while (true){
		error = 0;
        
        //if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
		//	cout << "Ultrasonic sensor sees this distance in cm: " << Ultrasonic.cm << endl;
		//}
		if(BP.get_sensor(BWsensorright, bwright) == 0){
			cout << "Right sees " << bwright.reflected << endl;
            if(bwright.reflected > 2100){
                //set right motor speed to lower value
                movement(90,-90);
            }
            else{
                //set right motor speed to normal value
                movement(50,50);
            }
		}
        else{
            cout << "Right light sensor not working!" << endl;
        }
		if(BP.get_sensor(BWsensorleft, bwleft) == 0){
			cout << "Left sees " << bwleft.reflected << endl;
            if(bwleft.reflected > 2100){
                //set left motor speed to lower value
                movement(-90,90);
            }
            else{
                //set left motor speed to normal value
                movement(50,50);
            }
		}
		else{
            cout << "Left light sensor not working!" << endl;
        }
    }
}

//Signal handler when Ctrl-C is pressed, makes sure nothing stays running
void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}

