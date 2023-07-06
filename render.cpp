/*
 ____  _____ _        _
| __ )| ____| |      / \
|  _ \|  _| | |     / _ \
| |_) | |___| |___ / ___ \
|____/|_____|_____/_/   \_\
http://bela.io
*/
/**
\example Fundamentals/print/render.cpp

Printing to the console
-----------------------

This example demonstrates how to print to the console. When working within the audio thread
use the function rt_printf(). This has the same functionality as printf() but is safe to call
from the audio thread. However, make sure to not make too many calls to this function within a
render loop as this may overload the CPU and/or stall communication with the board.
In the render() function above a counter is implemented in order to only print to the console
after a specified interval has passed. A counter variable is used to keep track of the amount
of samples elapsed after starting the program.
The usage of rt_printf() is identical to printf(): http://en.cppreference.com/w/cpp/io/c/fprintf
*/

#include <Bela.h>
#include "Adafruit_MLX90393/Adafruit_MLX90393.h"
// #include "Adafruit_Lib4Bela/Bela_Wire.h"

// #include <cmath>

// float gInterval = 0.5;
// float gSecondsElapsed = 0;
// int gCount = 0;
Adafruit_MLX90393 sensor;

// #include <I2c.h>

bool setup(BelaContext *context, void *userData)
{
	// I2c i2cBus;
	// uint8_t txBuffer[1], rxBuffer[1];
	// ssize_t w, r;

	// i2cBus.initI2C_RW(1, 0x0c, 0);
	
	// txBuffer[0] = 0x80;
	// w = i2cBus.writeBytes(txBuffer, 1);
	// r = i2cBus.readBytes(rxBuffer, 1);
	// rt_printf("WriteStatus: 0x%x, ReadStatus: 0x%x, Data: 0x%x\n", w, r, rxBuffer[0]);
	
	// txBuffer[0] = 0xF0;
	// w = i2cBus.writeBytes(txBuffer, 1);
	// r = i2cBus.readBytes(rxBuffer, 1);
	// rt_printf("WriteStatus: 0x%x, ReadStatus: 0x%x, Data: 0x%x\n", w, r, rxBuffer[0]);

	// i2cBus.closeI2C();
	
	rt_printf("Getting ready\n");
	sensor = Adafruit_MLX90393();
 //   // Print a string
	// rt_printf("This Bela example has just started running!\n");
	// // Print a string containing a float
 //   rt_printf("Here's the value of pi: %f\n",M_PI);
 
	if (! sensor.begin_I2C()) {          // hardware I2C mode, can pass in address & alt Wire
	    rt_printf("No sensor found ... check your wiring?\n");
	    while (1) { }
	  }

  sensor.setGain(MLX90393_GAIN_2_5X);
  // You can check the gain too
  rt_printf("Gain set to: ");
  switch (sensor.getGain()) {
    case MLX90393_GAIN_1X: rt_printf("1 x\n"); break;
    case MLX90393_GAIN_1_33X: rt_printf("1.33 x\n"); break;
    case MLX90393_GAIN_1_67X: rt_printf("1.67 x\n"); break;
    case MLX90393_GAIN_2X: rt_printf("2 x\n"); break;
    case MLX90393_GAIN_2_5X: rt_printf("2.5 x\n"); break;
    case MLX90393_GAIN_3X: rt_printf("3 x\n"); break;
    case MLX90393_GAIN_4X: rt_printf("4 x\n"); break;
    case MLX90393_GAIN_5X: rt_printf("5 x\n"); break;
  }

  // Set resolution, per axis
  sensor.setResolution(MLX90393_X, MLX90393_RES_19);
  sensor.setResolution(MLX90393_Y, MLX90393_RES_19);
  sensor.setResolution(MLX90393_Z, MLX90393_RES_16);

  // Set oversampling
  sensor.setOversampling(MLX90393_OSR_2);

  // Set digital filtering
  sensor.setFilter(MLX90393_FILTER_6); 
	 
	return true;
}

void render(BelaContext *context, void *userData)
{
	// for(unsigned int n = 0; n < context->audioFrames; n++) {

	// 	// Increment a counter on every frame
	// 	gCount++;

	// 	// Print a message every second indicating the number of seconds elapsed
	// 	if(gCount % (int)(context->audioSampleRate*gInterval) == 0) {
	// 	    gSecondsElapsed += gInterval;
	// 	    rt_printf("Time elapsed: %f\n",gSecondsElapsed);
	// 	}

	// }

  float x, y, z;

  // get X Y and Z data at once
  if (sensor.readData(&x, &y, &z)) {
      rt_printf("X: "); rt_printf("%F", x); rt_printf(" uT\n");
      rt_printf("Y: "); rt_printf("%F", y); rt_printf(" uT\n");
      rt_printf("Z: "); rt_printf("%F", z); rt_printf(" uT\n");
  } else {
      rt_printf("Unable to read XYZ data from the sensor.");
  }

  /* Or....get a new sensor event, normalized to uTesla */
  sensors_event_t event;
  sensor.getEvent(&event);
  /* Display the results (magnetic field is measured in uTesla) */
  rt_printf("X: "); rt_printf("%F", event.magnetic.x);
  rt_printf(" \tY: "); rt_printf("%F", event.magnetic.y);
  rt_printf(" \tZ: "); rt_printf("%F", event.magnetic.z);
  rt_printf(" uTesla \n");

}

void cleanup(BelaContext *context, void *userData)
{

}
