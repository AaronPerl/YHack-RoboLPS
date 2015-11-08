#include <ctime>
#include <iostream>
#include <stdexcept>
#include <raspicam/raspicam.h>

int main(int argc, char** argv)
{
	raspicam::RaspiCam camera;
	std::cout << "opening camera" << std::endl;
	if (!camera.open())
	{
		throw std::runtime_error("Could not open camera");
	}
	std::cout << "camera width: " << camera.getWidth() << std::endl;
	std::cout << "camera height: " << camera.getHeight() << std::endl;
}
