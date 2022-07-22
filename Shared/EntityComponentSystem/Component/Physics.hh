#pragma once

#include <cstdint>

namespace shared
{
    class Writer;
    class Reader;
}

namespace shared::ecs::component
{
    class Physics
    {
        float m_X;
        float m_Y;

    public:
        static constexpr uint32_t ID = 1;

        uint32_t ownerId;

        float X();
        float X(float);
        float Y();
        float Y(float);

        void WriteBinary(Writer &);
        void ReadBinary(Reader &);
    };
}
