# Press-Machine-food-processing
small arduino code to control linear motor for a press machine

after press start button, it will gradually increase the speed of the motor to protect against surge in current.

# Components 
Arduino board

BTS7960 as a motor Driver

ACS712 30A Current Sensor to monitor how hard press machine is pushing.

12v 30A power supply

IC 7805

#Circuit Diagram
![press_cd](https://github.com/foreztgump/Press-Machine-food-processing/blob/master/press_circuit.png)

# Wiring
start_button - digital pin 3

LED - digital pin 2

stop_button - digital pin 4

BTS7960_EN - digital pin 8

BTS7960_L_PWM - digital pin 10

BTS7960_R_PWM - digital pin 9

ACS712_out - analog A5

