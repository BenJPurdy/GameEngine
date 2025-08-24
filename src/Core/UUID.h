#pragma once
#include "Core.h"

namespace GameEngine
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t);
        UUID(const UUID&);

        operator uint64_t () { return uuid; }
        operator const uint64_t () const { return uuid; }
    private:
        uint64_t uuid;
    };

    class UUID32
    {
    public:
        UUID32();
        UUID32(uint32_t);
        UUID32(const UUID32&);

        operator uint32_t () { return uuid; }
        operator const uint32_t () const { return uuid; }

    private:
        uint32_t uuid;
    };
}

namespace std
{
    template<>
    struct hash<GameEngine::UUID>
    {
        std::size_t operator()(const GameEngine::UUID& uuid) const
        {
            //add hashing function in there at some point
            return uuid;
        }
    };

    template<>
    struct hash<GameEngine::UUID32>
    {
        std::size_t operator()(const GameEngine::UUID32& uuid) const
        {
            return hash<uint32_t>()((uint32_t)uuid);
        }
    };
}
