// - MotorControl simple -
// This simple example creates a MotorControl handle, hooks the event handlers, then opens and waits for a MotorControl Phidget to be attached.
// Once on has been attaced it will display device information and display any event data read.  The program will then do a short simulation
// of the functionality of the motor by increasing and decreasing the speed of an attached motor.  PLEASE NOTE:  This assignment
// was desgined assuming only one motor attached at motor index 0 (for motorcontrol boards with multiple motors).
//
// Copyright 2008 Phidgets Inc.  All rights reserved.
// This work is licensed under the Creative Commons Attribution 2.5 Canada License. 
// view a copy of this license, visit http://creativecommons.org/licenses/by/2.5/ca/

#include <stdio.h>
#include <phidget21.h>

#include <libpowerbutton.h><cr>

//Declare global variables
//Declare a motor control handle
CPhidgetMotorControlHandle motoControl = 0; 
int irLeft=0;
int irRight=0;
CPhidgetInterfaceKitHandle ifKit = 0;

//**************************************************************************************************
//IR Sensor Initialisation
//**************************************************************************************************
int IRAttachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName(IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int IRDetachHandler(CPhidgetHandle IFK, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (IFK, &name);
	CPhidget_getSerialNumber(IFK, &serialNo);

	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int IRErrorHandler(CPhidgetHandle IFK, void *userptr, int ErrorCode, const char *unknown)
{
	printf("Error handled. %d - %s", ErrorCode, unknown);
	return 0;
}

//callback that will run if an input changes.
//Index - Index of the input that generated the event, State - boolean (0 or 1) representing the input state (on or off)
int InputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	printf("Digital Input: %d > State: %d\n", Index, State);
	return 0;
}

//callback that will run if an output changes.
//Index - Index of the output that generated the event, State - boolean (0 or 1) representing the output state (on or off)
int OutputChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int State)
{
	printf("Digital Output: %d > State: %d\n", Index, State);
	return 0;
}

//callback that will run if the sensor value changes by more than the OnSensorChange trigger.
//Index - Index of the sensor that generated the event, Value - the sensor read value
int SensorChangeHandler(CPhidgetInterfaceKitHandle IFK, void *usrptr, int Index, int Value)
{
	if(Index=0){
	irLeft=Value;
	}
	if(Index=7){
	irRight=Value;
	}
	printf("Sensor: %d > Value: %d\n", Index, Value);
	return 0;
}

int display_properties(CPhidgetInterfaceKitHandle phid)
{
	int serialNo, version, numInputs, numOutputs, numSensors, triggerVal, ratiometric, i;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);

	CPhidgetInterfaceKit_getInputCount(phid, &numInputs);
	CPhidgetInterfaceKit_getOutputCount(phid, &numOutputs);
	CPhidgetInterfaceKit_getSensorCount(phid, &numSensors);
	CPhidgetInterfaceKit_getRatiometric(phid, &ratiometric);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Digital Inputs: %d\n# Digital Outputs: %d\n", numInputs, numOutputs);
	printf("# Sensors: %d\n", numSensors);
	printf("Ratiometric: %d\n", ratiometric);

	for(i = 0; i < numSensors; i++)
	{
		CPhidgetInterfaceKit_getSensorChangeTrigger (phid, i, &triggerVal);

		printf("Sensor#: %d > Sensitivity Trigger: %d\n", i, triggerVal);
	}

	return 0;
}


void* IR_simple(void *irLeft){

int result, numSensors, i;
	const char *err;
//Declare an InterfaceKit handle
	

	//create the InterfaceKit object
	CPhidgetInterfaceKit_create(&ifKit);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)ifKit, IRAttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)ifKit, IRDetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)ifKit, IRErrorHandler, NULL);

	//Registers a callback that will run if an input changes.
	//Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetInterfaceKit_set_OnInputChange_Handler (ifKit, InputChangeHandler, NULL);

	//Registers a callback that will run if the sensor value changes by more than the OnSensorChange trig-ger.
	//Requires the handle for the IntefaceKit, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetInterfaceKit_set_OnSensorChange_Handler (ifKit, SensorChangeHandler, NULL);

	//Registers a callback that will run if an output changes.
	//Requires the handle for the Phidget, the function that will be called, and an arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetInterfaceKit_set_OnOutputChange_Handler (ifKit, OutputChangeHandler, NULL);

	//open the interfacekit for device connections
	CPhidget_open((CPhidgetHandle)ifKit, -1);

	//get the program to wait for an interface kit device to be attached
	printf("Waiting for interface kit to be attached....");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)ifKit, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		
	}

	//Display the properties of the attached interface kit device
	display_properties(ifKit);


/*	//read interface kit event data
	printf("Reading.....\n");

	//keep displaying interface kit data until user input is read
	printf("Press any key to go to next step\n");
	getchar();

	printf("Modifying sensor sensitivity triggers....\n");

	//get the number of sensors available
	CPhidgetInterfaceKit_getSensorCount(ifKit, &numSensors);

	//Change the sensitivity trigger of the sensors
	for(i = 0; i < numSensors; i++)
	{
		CPhidgetInterfaceKit_setSensorChangeTrigger(ifKit, i, 100);  //we'll just use 10 for fun
	}*/

}

void unregisterIR(){

	//since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
	printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)ifKit);
	CPhidget_delete((CPhidgetHandle)ifKit);

}
//**************************************************************************************************


int AttachHandler(CPhidgetHandle MC, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (MC, &name);
	CPhidget_getSerialNumber(MC, &serialNo);
	printf("%s %10d attached!\n", name, serialNo);

	return 0;
}

int DetachHandler(CPhidgetHandle MC, void *userptr)
{
	int serialNo;
	const char *name;

	CPhidget_getDeviceName (MC, &name);
	CPhidget_getSerialNumber(MC, &serialNo);
	printf("%s %10d detached!\n", name, serialNo);

	return 0;
}

int ErrorHandler(CPhidgetHandle MC, void *userptr, int ErrorCode, const char *Description)
{
	printf("Error handled. %d - %s\n", ErrorCode, Description);
	return 0;
}


int InputChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, int State)
{
	printf("Input %d > State: %d\n", Index, State);
	return 0;
}

int VelocityChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
	printf("Motor %d > Current Speed: %f\n", Index, Value);
	return 0;
}

int CurrentChangeHandler(CPhidgetMotorControlHandle MC, void *usrptr, int Index, double Value)
{
	printf("Motor: %d > Current Draw: %f\n", Index, Value);
	return 0;
}

int display_properties(CPhidgetMotorControlHandle phid)
{
	int serialNo, version, numInputs, numMotors;
	const char* ptr;

	CPhidget_getDeviceType((CPhidgetHandle)phid, &ptr);
	CPhidget_getSerialNumber((CPhidgetHandle)phid, &serialNo);
	CPhidget_getDeviceVersion((CPhidgetHandle)phid, &version);
	
	CPhidgetMotorControl_getInputCount(phid, &numInputs);
	CPhidgetMotorControl_getMotorCount(phid, &numMotors);

	printf("%s\n", ptr);
	printf("Serial Number: %10d\nVersion: %8d\n", serialNo, version);
	printf("# Inputs: %d\n# Motors: %d\n", numInputs, numMotors);

	return 0;
}

void go_straight()
{
	CPhidgetMotorControl_setAcceleration (motoControl, 0, 100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 0, 100.00);

	CPhidgetMotorControl_setAcceleration (motoControl, 1, - 100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 1, - 100.00);
}

void go_back()
{
	CPhidgetMotorControl_setAcceleration (motoControl, 0, - 100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 0, - 100.00);

	CPhidgetMotorControl_setAcceleration (motoControl, 1,  100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 1,  100.00);
}

void turn_left()
{
	CPhidgetMotorControl_setAcceleration (motoControl, 0,  100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 0,  100.00);

	CPhidgetMotorControl_setAcceleration (motoControl, 1,  100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 1,  100.00);
}

void turn_right()
{
	CPhidgetMotorControl_setAcceleration (motoControl, 0, - 100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 0, - 100.00);

	CPhidgetMotorControl_setAcceleration (motoControl, 1, - 100.00);
	CPhidgetMotorControl_setVelocity (motoControl, 1, - 100.00);
}

void stop()
{
	CPhidgetMotorControl_setAcceleration (motoControl, 0, 0.00);
	CPhidgetMotorControl_setVelocity (motoControl, 0, 0.00);

	CPhidgetMotorControl_setAcceleration (motoControl, 1, 0.00);
	CPhidgetMotorControl_setVelocity (motoControl, 1, 0.00);
}







int register_motors(){

	int result;
	const char *err;

	

	//create the motor control object
	CPhidgetMotorControl_create(&motoControl);

	//Set the handlers to be run when the device is plugged in or opened from software, unplugged or closed from software, or generates an error.
	CPhidget_set_OnAttach_Handler((CPhidgetHandle)motoControl, AttachHandler, NULL);
	CPhidget_set_OnDetach_Handler((CPhidgetHandle)motoControl, DetachHandler, NULL);
	CPhidget_set_OnError_Handler((CPhidgetHandle)motoControl, ErrorHandler, NULL);

	//Registers a callback that will run if an input changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnInputChange_Handler (motoControl, InputChangeHandler, NULL);

	//Registers a callback that will run if a motor changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnVelocityChange_Handler (motoControl, VelocityChangeHandler, NULL);

	//Registers a callback that will run if the current draw changes.
	//Requires the handle for the Phidget, the function that will be called, and a arbitrary pointer that will be supplied to the callback function (may be NULL).
	CPhidgetMotorControl_set_OnCurrentChange_Handler (motoControl, CurrentChangeHandler, NULL);

	//open the motor control for device connections
	CPhidget_open((CPhidgetHandle)motoControl, -1);

	//get the program to wait for a motor control device to be attached
	printf("Waiting for MotorControl to be attached....");
	if((result = CPhidget_waitForAttachment((CPhidgetHandle)motoControl, 10000)))
	{
		CPhidget_getErrorDescription(result, &err);
		printf("Problem waiting for attachment: %s\n", err);
		return 0;
	}

	//Display the properties of the attached motor control device
	display_properties(motoControl);

	//read motor control event data
	printf("Reading.....\n");

	//keep displaying motor control event data until user input is read
	printf("Press any key to continue\n");
	getchar();


}


void motorcontrol_simple()
{
	//Control the motor a bit.
	go_straight();

	printf("Press any key to continue\n");
	getchar();

	//Step 2: Set acceleration to 100, decrease target speed to 75
	turn_right();
	printf("Press any key to continue\n");
	getchar();

	//Step 3: Stop the motor by decreasing speed to 0;
	turn_left();
	printf("Press any key to end\n");
	getchar();

	//Step turn
	go_back();
	printf("Press any key to end\n");
	getchar();

	stop();
	printf("Press any key to end\n");
	getchar();


}

void unregister_motors(){
	//since user input has been read, this is a signal to terminate the program so we will close the phidget and delete the object we created
	printf("Closing...\n");
	CPhidget_close((CPhidgetHandle)motoControl);
	CPhidget_delete((CPhidgetHandle)motoControl);



}




int main(int argc, char* argv[])
{
	
    /*printf("Button pressed %i times.\n",power_button_get_value());*/
	register_motors();
    
        sleep(1); 
        /*printf("Button pressed %i times.\n",power_button_get_value());*/
	
	int irValues[2];
	
	//Run IR sensors in a separate thread
	pthread_t irThread;
	pthread_create(&irThread, NULL, IR_simple, (void *) &irValues );

	while(1){
	//Create the motor movement routine
	
	if(irLeft>200){
	turn_right();
	}
	if(irRight>200){
	turn_left();
	}
	if(irLeft && irRight >200){
	stop();
	go_back();
	}
	else{
	go_straight();
	}
	

}
    	unregisterIR();
	unregister_motors();
    //power_button_reset();
    return 0;	
}


