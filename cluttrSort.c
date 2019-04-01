#pragma config(Sensor, in1,    lightSensor,    sensorReflection)
#pragma config(Sensor, in2,    lineFollower,   sensorLineFollower)
#pragma config(Sensor, dgtl1,  encoder,        sensorQuadEncoder)
#pragma config(Motor,  port2,           flash,         tmotorVexFlashlight, openLoop, reversed)
#pragma config(Motor,  port3,           turntableMotor, tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           rodMotor,      tmotorServoContinuousRotation, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Variables for processing sorts
int currentStorageContainer = 1;
int calledStorageContainer = 0;
int waitTimer;
bool listen, callReset = true;

// Variables for referencing to base values of materials
// Nothing
int nothingLineVal = 2660;
int nothingLightVal = 55;

// Steel Vals
int steelLineVal = 2890;
int steelLightVal = 315;

// Clear Vals
int clearLineVal = 2890;
int clearLightVal = 20;

// Wood vals
int woodLineVal = 1410;
int woodLightVal = 325;

// Plastic Vals
int plasticLineVal = 300;
int plasticLightVal = 210;

// Variables for counting all the materials
int clearCount, steelCount, woodCount, plasticCount, totalCount;

// Hardware variables
int lightVal = SensorValue(lightSensor);
int lineVal = SensorValue(lineFollower);
int encoderVal = SensorValue(encoder);

// Init task
task init() {
	// Clearing debug stream
	clearDebugStream();

	// Writing a cool opening init logo
	writeDebugStreamLine("      _       _   _ ver 1.0");
	writeDebugStreamLine("  ___| |_   _| |_| |_ _ __ ");
	writeDebugStreamLine(" / __| | | | | __| __| '__|");
	writeDebugStreamLine("| (__| | |_| | |_| |_| |   ");
	writeDebugStreamLine(" \___|_|\__,_|\__|\__|_|   ");
	writeDebugStreamLine("[INIT] ===[      Initializing...     ]===");

	// Set motor to '0' power
	motor[turntableMotor] = 0;

	// Setting variables
	writeDebugStreamLine("[INIT] [VAR] ==[ Setting variables... ]==");

	// Resetting material counts
	writeDebugStreamLine("[INIT] [VAR] Resetting material counts...");
	clearCount = 0;
	steelCount = 0;
	woodCount = 0;
	plasticCount = 0;
	totalCount = 0;
	writeDebugStreamLine("[INIT] [VAR] Reset material counts!");

	writeDebugStreamLine("[INIT] [VAR] ==[    Variables set!    ]==");

	writeDebugStreamLine("[INIT] ===[ Initialization complete! ]===");
}

// Checks for around the goal amount for each material
bool checkAround(int value, int goal) {
	writeDebugStreamLine("[CHECK] Checking values...");
	if (value > 1000) {
		if (goal - 500 < value && value < goal + 500) {
			writeDebugStreamLine("[CHECK] Large Goal met!");
			return true;
			} else {
			writeDebugStreamLine("[CHECK] Large Goal not met!");
			return false;
		}
	} else {
		if (goal - 40 < value && value < goal + 40) {
			writeDebugStreamLine("[CHECK] Small Goal met!");
			return true;
			} else {
			writeDebugStreamLine("[CHECK] Small Goal not met!");
			return false;
		}
	}
}

// Detects the item in the sorting mechanism
task detectMaterial() {
	writeDebugStreamLine("[DETECT] 'detectMaterial' task started!");
	if(lightVal > 30 || lineVal < 2960) {
		writeDebugStreamLine("[DETECT] Detecting material...");

		// Neutral
		if (checkAround(lightVal, nothingLightVal) && checkAround(lineVal, nothingLineVal)){
			writeDebugStreamLine("[DETECT] Detecting nothing...");
			writeDebugStreamLine("[DETECT] [VAR] Light Sensor: %d", lightVal);
			writeDebugStreamLine("[DETECT] [VAR] Line Follower: %d", lineVal);
		}
		// Clear detection process
		if(checkAround(lightVal, clearLightVal) && checkAround(lineVal, clearLineVal)) {
			writeDebugStreamLine("[DETECT] Detecting 'clear'...");
			writeDebugStreamLine("[DETECT] [VAR] Light Sensor: %d", lightVal);
			writeDebugStreamLine("[DETECT] [VAR] Line Follower: %d", lineVal);
			clearCount++;
			totalCount++;
			calledStorageContainer = 1;
		}
		// Plastic detection process
		if(checkAround(lightVal, plasticLightVal) && checkAround(lineVal, plasticLineVal)) {
			writeDebugStreamLine("[DETECT] Detecting 'plastic'...");
			writeDebugStreamLine("[DETECT] [VAR] Light Sensor: %d", lightVal);
			writeDebugStreamLine("[DETECT] [VAR] Line Follower: %d", lineVal);
			plasticCount++;
			totalCount++;
			calledStorageContainer = 2;
		}
		// Wood detection process
		if(checkAround(lightVal, woodLightVal) && checkAround(lineVal, woodLineVal)) {
			writeDebugStreamLine("[DETECT] Detecting 'wood'...");
			writeDebugStreamLine("[DETECT] [VAR] Light Sensor: %d", lightVal);
			writeDebugStreamLine("[DETECT] [VAR] Line Follower: %d", lineVal);
			woodCount++;
			totalCount++;
			calledStorageContainer = 3;
		}
		// Steel detection process
		if(checkAround(lightVal, steelLightVal) && checkAround(lineVal, steelLineVal)) {
			writeDebugStreamLine("[DETECT] Detecting 'steel'...");
			writeDebugStreamLine("[DETECT] [VAR] Light Sensor: %d", lightVal);
			writeDebugStreamLine("[DETECT] [VAR] Line Follower: %d", lineVal);
			steelCount++;
			totalCount++;
			calledStorageContainer = 4;
		}
	}
}

// Sets the turntable turn to whatever encoder value is set
task detectStorageContainer() {
	while(true){
		// Clear Storage
		if (encoderVal == 1) {
			currentStorageContainer = 1;
			writeDebugStreamLine("[SORT] Detected storage container 1");
			wait(1);
		}
		// Plastic Storage
		if (encoderVal == 2) {
			currentStorageContainer = 2;
			writeDebugStreamLine("[SORT] Detected storage container 2");
			wait(1);
		}
		// Wood Storage
		if (encoderVal == 3) {
			currentStorageContainer = 3;
			writeDebugStreamLine("[SORT] Detected storage container 3");
			wait(1);
		}
		// Steel Storage
		if (encoderVal == 4) {
			currentStorageContainer = 4;
			writeDebugStreamLine("[SORT] Detected storage container 4");
			wait(1);
		}
	}
}

// Logic for sensor readings and how to process
task checkListen() {

	writeDebugStreamLine("[LISTEN] 'checkListen' task started!");

	// Clearing timer
	writeDebugStreamLine("[LISTEN] clearing timer 'T1'...");
	clearTimer(T1);
	writeDebugStreamLine("[LISTEN] Timer cleared!");
	while(listen) {

		writeDebugStreamLine("[LISTEN] Listening...");
		// Only activate after init
		if(calledStorageContainer > 0) {

			writeDebugStreamLine("[LISTEN] [VAR] Caught change in variable 'calledStorageContainer'!");

			// Set listen variable to false
			// This prevents any other input from the sensors
			writeDebugStreamLine("[LISTEN] Attemping to stop listening to inputs...");
			listen = false;
			writeDebugStreamLine("[LISTEN] Stopped listening!");

			// Conditionals for what storage container is there
			if(currentStorageContainer < calledStorageContainer) {
				// Task for moving motor positive
				} else if (currentStorageContainer > calledStorageContainer) {
				// Task for moving motors negative
				} else {

				// waitTimer var is assigned to T1
				waitTimer = T1;

				// Starts time for Idle process
				if (waitTimer > 20000) {
					writeDebugStreamLine("[LISTEN] Starting idle process...");
					writeDebugStreamLine("[IDLE] Total counts of all materials:");
					writeDebugStreamLine("Clear: %d", clearCount);
					writeDebugStreamLine("Plastic: %d", plasticCount);
					writeDebugStreamLine("Wood: %d", woodCount);
					writeDebugStreamLine("Steel: %d", steelCount);
					writeDebugStreamLine("TOTAL: %d", totalCount);
					calledStorageContainer = 1;
				}
			}
		}
	}
}

// Main task
task main() {

	// Starting of initialization sequence
	startTask(init);
	writeDebugStreamLine("Starting loopback...");

	startTask(detectStorageContainer);
	while(true) {

		// Flash turning on
		turnFlashlightOn(flash, -127);

		// While the callReset var is true
		if(callReset == true) {

			// Setting callReset to false
			callReset = false;

			// Detection and processing
			startTask(detectMaterial);
			startTask(checkListen);
		}
	}
}
