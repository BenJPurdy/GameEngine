#include "GameEngine_PCH.h"
#include "Network.h"
#include "Logging/Log.h"

namespace GameEngine
{
	void Network::create(bool isHost)
	{

		if (enet_initialize() != 0)
		{
			LOG_FATAL("Failed to initialise enet");
		}
		address.host = ENET_HOST_ANY;
		address.port = 11671;

		if (isHost)
		{
			host = enet_host_create(&address,
				32, //max clients
				2,	//max channels
				0,	//max incoming bandwidth (0 = no limit)
				0	//max outgoing bandwidth (0 = no limit)
			);
			amHost = true;
		}
		else
		{
			host = enet_host_create(NULL, 1, 2, 0, 0);
		}
		
		if (host == nullptr)
		{
			std::string type = isHost ? "Host" : "Client";
			LOG_ERROR("Failed to create {0}", type);
		}
	}

	void Network::destroy()
	{
		enet_host_destroy(host);
	}

	void Network::connect()
	{
		if (amHost) return;

	}
}