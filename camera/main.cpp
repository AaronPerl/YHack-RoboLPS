#include <ctime>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <raspicam/raspicam.h>
#include <RF24/RF24.h>

#define LOCAL_ADDR "camPi"
#define ROBOT_ADDR "robot"

int main(int argc, char** argv)
{
	uint8_t addresses[][6] = {LOCAL_ADDR, ROBOT_ADDR};
	raspicam::RaspiCam camera;
	RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);
	int curInput;
	std::cout << "opening camera" << std::endl;
	if (!camera.open())
	{
		throw std::runtime_error("Could not open camera");
	}
	radio.begin();
	radio.openWritingPipe(addresses[1]);
	radio.openReadingPipe(1, addresses[0]);
	std::cout << "camera width: " << camera.getWidth() << std::endl;
	std::cout << "camera height: " << camera.getHeight() << std::endl;
	while ((curInput = getchar()) != EOF)
	{
		if (curInput != '\n')
		{
			if (!radio.write((char*)&curInput, sizeof(char)))
				std::cerr << "error writing to pipe" << std::endl;
		}
	}
}
