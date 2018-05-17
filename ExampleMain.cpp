#include "EndpointIterator.h"
#include <iostream>

int main()
{
	// object that manages and iteracts with audio endpoints.
	auto enumerator = Endpoints::EndpointIterator();
	
	// array of audio endpoints.
	auto endpoints = enumerator.GetEndpoints();
	
	// go over all speakers, print out their names
	for (size_t i = 0; i < endpoints.size(); ++i)
	{
		std::cout << "Endpoint: " << endpoints[i].GetName() << std::endl;
	}

	// Unmute headphones and set vol to 74
	enumerator.GetEndpointByName("Headphones").SetVolume(74);
	enumerator.GetEndpointByName("Headphones").Unmute();

	// you get the gist
	enumerator.GetEndpointByName("Room").SetVolume(5);
	enumerator.GetEndpointByName("Headphones").SetDefaultMain();

	// ye you do.
	enumerator.GetEndpointByName("Vive").SetDefaultComm();



	while (true);
}