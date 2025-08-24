#pragma once
#include <enet/enet.h>
#include <vector>
#include <thread>
#include <glm/glm.hpp>

#define BIT(x) (1 << x)

namespace GameEngine
{
	class Entity;
	struct TransformComponent;

	class Network
	{
	public:
		~Network() = default;
		std::atomic_bool runThread = false;
		std::thread serverThread;
		ENetAddress address;
		ENetHost* localHost;
		ENetPeer* peer;
		bool amHost;
		void init();
		void create(bool isHost = false);
		void destroy();
		void connect();
		//TODO (P) make sure update can send data to differnet clients
		void update(std::vector<Entity>);
		void runServer();
		void test(Entity);
	private:
		ENetPacket* makeTransformPacket(Entity);
		void serialiseTransform(TransformComponent&, uint8_t* buffer);
		void serialiseVec3(glm::vec3&, uint8_t*);
		TransformComponent deserialiseTransform(uint8_t*);

		//packets are only to be added to the end of the enum
		enum PacketType : uint16_t
		{
			PacketType_INVALID		= 0,
			PacketType_Connect		= BIT(0),
			PacketType_Disconnect	= BIT(1),
			PacketType_ID			= BIT(2),
			PacketType_Transform	= BIT(3),
			PacketType_Misc			= BIT(4)
		};


		
	};

}