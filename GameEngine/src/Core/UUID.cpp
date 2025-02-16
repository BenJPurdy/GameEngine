#include "GameEngine_PCH.h"
#include "UUID.h"

#include <random>

namespace GameEngine
{
	static std::random_device rd;
	static std::mt19937_64 mt(rd());
	static std::uniform_int_distribution<uint64_t> dist;

	static std::mt19937 mt32(rd());
	static std::uniform_int_distribution<uint32_t> dist32;

	UUID::UUID() : uuid(dist(mt)) {}

	UUID::UUID(uint64_t id) : uuid(id) {}

	UUID::UUID(const UUID& other) : uuid(other) {}

	UUID32::UUID32() : uuid(dist(mt)) {}

	UUID32::UUID32(uint32_t id) : uuid(id) {}

	UUID32::UUID32(const UUID32& other) : uuid(other) {}
}