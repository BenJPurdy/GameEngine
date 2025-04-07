#include "GameEngine_PCH.h"
#include "Network.h"
#include "Logging/Log.h"
#include "Scene/Components.h"

#define PORT_NUMBER 8888

namespace GameEngine
{

	void Network::init()
	{
		if (enet_initialize() != 0)
		{
			LOG_FATAL("Failed to initialise enet");
		}
	}
	void Network::create(bool isHost)
	{

		
		address.host = ENET_HOST_ANY;
		address.port = PORT_NUMBER;

		if (isHost)
		{
			localHost = enet_host_create(&address,
				32, //max clients
				2,	//max channels
				0,	//max incoming bandwidth (0 = no limit)
				0	//max outgoing bandwidth (0 = no limit)
			);
			amHost = true;
		}
		else
		{
			localHost = enet_host_create(NULL, 1, 2, 0, 0);
		}
		
		if (localHost == nullptr)
		{
			std::string type = isHost ? "Host" : "Client";
			LOG_ERROR("Failed to create {0}", type);
		}

	}

	void Network::runServer()
	{
		while (runThread)
		{
			//LOG_TRACE("Update server");
			ENetEvent event;

			while (enet_host_service(localHost, &event, 16) > 0)
			{
				switch (event.type)
				{
				case ENET_EVENT_TYPE_CONNECT:
					LOG_TRACE("new client from: {0}:{1}", event.peer->address.host, event.peer->address.port);
					break;
				case ENET_EVENT_TYPE_DISCONNECT:
					LOG_TRACE("{0} disconnected", event.peer->data);
					break;
				}
			}
			LOG_TRACE("No packet recived");
		}
		LOG_TRACE("exiting thread");
		return;
	}

	

	void Network::destroy()
	{
		enet_host_destroy(localHost);
	}

	void Network::connect()
	{
		ENetAddress addr;
		ENetEvent event;
		ENetPeer* peer;

		enet_address_set_host(&addr, "127.0.0.1");
		addr.port = PORT_NUMBER;

		peer = enet_host_connect(localHost, &addr, 2, 0);

		if (peer == nullptr)
		{
			LOG_FATAL("No available peers for initiating a connection");
			return;
		}
		
		if (enet_host_service(localHost, &event, 5000) > 0
			&& event.type == ENET_EVENT_TYPE_CONNECT)
		{
			LOG_TRACE("connected to server");
		}
		else
		{
			LOG_ERROR("Connection to server timed out");
		}





	}
}