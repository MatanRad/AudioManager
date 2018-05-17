#ifndef VOLUME_H
#define VOLUME_H

#include <Mmdeviceapi.h>
#include <Endpointvolume.h>
#include <string>

namespace Endpoints
{

	enum class EndpointDefaultType
	{
		Main,
		Comm
	};

	class Endpoint
	{
		IMMDevice *device;

		std::string name;
		std::string controller;

		IAudioEndpointVolume* GetAudioEndpointVolume() const;

	public:
		LPWSTR GetID();

		const char* GetName() const;
		const char* GetControllerName() const;

		void Mute();
		void Unmute();
		void SetMute(bool mute);

		bool GetMute() const;

		// Get/set volume of type int between 0-100
		int GetVolume() const;
		void SetVolume(int vol);

		// Get/set volume of type float between 0-1
		float GetVolumeFloat() const;
		void SetVolumeFloat(float vol);

		bool IsDefault(EndpointDefaultType defType) const;
		void SetDefault(EndpointDefaultType defType);
		void SetDefaultMain();
		void SetDefaultComm();

		Endpoint(IMMDevice *device);
		Endpoint(const Endpoint& endpoint);
		~Endpoint();
	};
}
#endif // !VOLUME_H