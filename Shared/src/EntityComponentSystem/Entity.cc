#include <EntityComponentSystem/Entity.hh>

#include <Coder/Writer.hh>
#include <EntityComponentSystem/Component/Basic.hh>
#include <EntityComponentSystem/Component/Physics.hh>
#include <EntityComponentSystem/Component/Types.hh>

template <shared::ecs::component::types::component Component>
inline void DeleteComponent(shared::ecs::Entity *entity)
{
    if (entity->Has<Component>())
        delete &entity->Get<Component>();
}

namespace shared::ecs
{
    Entity::Entity(Simulation *simulation)
        : simulation(simulation)
    {
    }

    Entity::~Entity()
    {
        DeleteComponent<component::Basic>(this);
        DeleteComponent<component::Physics>(this);
    }

    void Entity::WriteBinary(coder::Writer &writer, bool isCreation)
    {
        writer.Vu(id);
        writer.U8(isCreation);

        if (isCreation)
        {
            uint32_t componentFlags = 0;
            componentFlags |= Has<component::Basic>() << component::Basic::ID;
            componentFlags |= Has<component::Physics>() << component::Physics::ID;

            writer.Vu(componentFlags);
        }

        if (Has<component::Basic>())
            Get<component::Basic>().WriteBinary(writer);
        if (Has<component::Physics>())
            Get<component::Physics>().WriteBinary(writer);
    }
}
