#include <enet/enet.h>

namespace GameEngine
{
	class Network
	{
	public:
		ENetAddress address;
		ENetHost* host;
		bool amHost;
		void create(bool isHost = false);
		void destroy();
		void connect();
		
	};

}