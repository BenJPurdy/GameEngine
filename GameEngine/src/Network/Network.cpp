#include "GameEngine_PCH.h"
#include "Network.h"
#include "Logging/Log.h"
#include "Scene/Components.h"
#include "Scene/Entity.h"

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

	void Network::update(std::vector<Entity> networkObjects)
	{
		ENetEvent e;
		ENetPacket* packet;
		//we can send all the packets by calling enet_host_service
		//or enet_peer_send & enet_host_flushto send a specific packet
		while (enet_host_service(localHost, &e, 0) > 0)
		{
			
		}
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

		while (enet_host_service(localHost, &event, 0) > 0)
		{
			//call this to make sure the server registers that we have connected.
			//it works so I won't be questioning it
		}
	}

	void Network::test(Entity e)
	{
		ENetPacket* t = makeTransformPacket(e);
	}

	//packet
	// byte 0-3 = id
	// byte 4 = packet Type
	// 
	//

	ENetPacket* Network::makeTransformPacket(Entity e)
	{
		//size of a transform (assuming glm::vec3 for pos, rot, scl)
		const size_t packetSize = (3 * sizeof(glm::vec3));
		if (!e.hasComponent<TransformComponent>()) return nullptr;
		auto& tc = e.getComponent<TransformComponent>();
		uint8_t data[36];
		serialiseTransform(tc, data);
		TransformComponent test = deserialiseTransform(data);
		LOG_TRACE("finished");
	}

	void Network::serialiseTransform(TransformComponent& tc, uint8_t* buffer)
	{
		float* b = reinterpret_cast<float*>(buffer);
		b[0] = tc.transform.x;	b[1] = tc.transform.y;	b[2] = tc.transform.z;
		b[3] = tc.rotation.x;	b[4] = tc.rotation.y;	b[5] = tc.rotation.z;
		b[6] = tc.scale.x;		b[7] = tc.scale.y;		b[8] = tc.scale.z;
	}

	TransformComponent Network::deserialiseTransform(uint8_t* buffer)
	{
		const float* b = reinterpret_cast<const float*>(buffer);
		TransformComponent tc;
		tc.transform = glm::vec3(b[0], b[1], b[2]);
		tc.rotation = glm::vec3(b[3], b[4], b[5]);
		tc.scale = glm::vec3(b[6], b[7], b[8]);
		tc.mod = true;
		//we set true on the assumption that this will change the transform and we
		//should update the physics world.
		//this might not always be the case but it's better to do this than not
		return tc;

	}

	

	void Network::serialiseVec3(glm::vec3& v, uint8_t* buffer)
	{
		float* b = reinterpret_cast<float*>(buffer);
		b[0] = v.x;
		b[1] = v.y;
		b[2] = v.z;
	}
}