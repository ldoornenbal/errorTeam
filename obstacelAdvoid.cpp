#include "BrickPi3.h"
#include <unistd.h>

BrickPi3 BP;

void exit_signal_handler(int signo);

uint8_t portRight = PORT_B;
uint8_t portLeft = PORT_C;

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

int main() {
	signal(SIGINT, exit_signal_handler); //exit function for ctrl c
	
	BP.detect(); //make sure Pi is communicating and up to date
	
	int error;

	BP.set_sensor_type(PORT_3, SENSOR_TYPE_NXT_ULTRASONIC);
	
	
	sensor_ultrasonic_t Ultrasonic;
	
	if(BP.get_sensor(PORT_3, Ultrasonic) == 0){
			if (Ultrasonic.cm < 15) {
				//OBSTACLE DETECTED
				goAroundAbstacle();
			} else {
				//NO OBSTACLE DETECTED
				driveAhead();
			}
	}
	
}

void goAroundObstacle() {
	sensor_ultrasonic_t Ultrasonic;
	bool seeSomething = true;
	int i = 0;
	
	
	while (seeSomething) {
		turn(portLeft, portRight, 1082430, 50);
		forwardBack(portLeft, portRight, 50);
		usleep(1000000);
		stopArnold();
		turn(portLeft, portRight, 1082430, -50);
		++i;
		while (BP.get_sensor(PORT3, Ultrasonic) != 0) {
			usleep(10);
		}
		if (Ultrasonic.cm > 15) {
			seeSomething = false;
		}
	}
	
	//DRIVE LITTLE FORWARD
	//TURN RIGHT TO CHECK IF IT SEES THE OBJECT
	
	forwardBack(portLeft, portRight, 50);
	usleep(1000000 * i);
	stopArnold();
	turn(portLeft, portRight, 1082430, 50);
	
}

void driveAhead() {
	//EVENTUEEL LINE VOLG FUNCTIE
}

void exit_signal_handler(int signo){
	if(signo == SIGINT){
		BP.reset_all();
		exit(-2);
	}
}