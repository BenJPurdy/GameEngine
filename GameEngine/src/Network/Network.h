#include <enet/enet.h>
#include <thread>


namespace GameEngine
{
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
		void runServer();
		
	};

}