#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <raspicam/raspicam.h>
#include <RF24/RF24.h>
#include <fstream>
#include <unistd.h>

#define LOCAL_ADDR "camPi"
#define ROBOT_ADDR "robot"

int main(int argc, char** argv)
{
	uint8_t addresses[][6] = {LOCAL_ADDR, ROBOT_ADDR};
	uint8_t *buf1;
	uint8_t *buf2;
	raspicam::RaspiCam camera;
	RF24 radio(RPI_V2_GPIO_P1_22, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_4MHZ);
	int curInput;
	std::cout << "opening camera" << std::endl;
	if (!camera.open())
	{
		throw std::runtime_error("Could not open camera");
	}
	camera.setHorizontalFlip(true);
	camera.setVerticalFlip(true);
	radio.begin();
	radio.openWritingPipe(addresses[1]);
	radio.openReadingPipe(1, addresses[0]);
	std::cout << "camera width: " << camera.getWidth() << std::endl;
	std::cout << "camera height: " << camera.getHeight() << std::endl;
	buf1 = new uint8_t[camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
	buf2 = new uint8_t[camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB)];
	std::cout << "image size" << camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB) << std::endl;
	std::ofstream file1("image1.ppm",std::ios::binary);
	std::ofstream file2("image2.ppm",std::ios::binary);
	while ((curInput = getchar()) != EOF)
	{
		if (curInput == 'p')
		{
			std::cout << "grabbing frames" << std::endl;
			camera.grab();
			camera.retrieve(buf1, raspicam::RASPICAM_FORMAT_RGB);
			radio.write("n", sizeof(char));
			usleep(100000);
			camera.grab();
			camera.retrieve(buf2, raspicam::RASPICAM_FORMAT_RGB);
			radio.write("o", sizeof(char));
			file1 << "P6\n" << camera.getWidth() << " " << camera.getHeight() << " 255\n";
			file1.write((char*)buf1, camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
			unsigned int maxDiff = 0;
			short x = 0;
			short y = 0;
			for (unsigned int i = 0; i < camera.getWidth() * camera.getHeight(); i++)
			{
				unsigned int totalDiff = 0;
				for (unsigned int j = 0; j < 3; j++)
				{
					short diff = (short)buf2[i*3+j] - buf1[i*3+j];
					diff = std::abs(diff);
					buf2[i*3+j] = (uint8_t) diff;
					totalDiff += diff;
				}
				if (totalDiff > maxDiff)
				{
					x = i % camera.getWidth();
					y = i / camera.getWidth();
					maxDiff = totalDiff;
				}
			}
			std::cout << "x:"<< x << ",y:" << y << std::endl;
			std::cout << "maxDiff: " << maxDiff << std::endl;
			file2 << "P6\n" << camera.getWidth() << " " << camera.getHeight() << " 255\n";
			file2.write((char*)buf2, camera.getImageTypeSize(raspicam::RASPICAM_FORMAT_RGB));
			file1.close();
			file2.close();
			std::cout << "frames grabbed" << std::endl;
		}
		else if (curInput != '\n')
		{
			if (!radio.write((char*)&curInput, sizeof(char)))
				std::cerr << "error writing to pipe" << std::endl;
		}
	}
	delete[] buf1;
	delete[] buf2;
}
