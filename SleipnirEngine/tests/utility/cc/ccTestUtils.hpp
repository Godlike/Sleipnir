#ifndef SLEIPNIR_ENGINE_TESTS_UTILITY_CC_CC_TEST_UTILS_HPP
#define SLEIPNIR_ENGINE_TESTS_UTILITY_CC_CC_TEST_UTILS_HPP

#include <sleipnir/utility/cc/Changes.hpp>

#include <atomic>
#include <cstdint>
#include <vector>

namespace ccTestUtils
{

struct Position
{
    int32_t x;
    int32_t y;
    int32_t z;

    Position();
    Position(int32_t _x, int32_t _y, int32_t _z);

    Position(Position const& other) = default;
    Position& operator=(Position const& other) = default;

    Position(Position&& other) = default;
    Position& operator=(Position&& other) = default;

    ~Position() = default;

    bool operator==(Position const& other) const;

    Position operator+(Position const& other) const;
    Position operator*(Position const& other) const;

    Position operator+(int32_t offset) const;
    Position operator*(int32_t offset) const;

    Position& operator+=(Position const& other);
    Position& operator*=(Position const& other);

    Position& operator+=(int32_t offset);
    Position& operator*=(int32_t offset);
};

using Handle = uint16_t;

class Object;
class ObjectCollection;

struct ObjectMemento
{
    using Object = ccTestUtils::Object;
    constexpr static Handle INVALID_ID = 0xFFFF;

    bool operator==(ObjectMemento const& other) const
    {
        return id == other.id
            && pos.first == other.pos.first
            && pos.second == other.pos.second
            && mass.first == other.mass.first
            && mass.second == other.mass.second
        ;
    }

    Handle id = INVALID_ID;

    std::pair<bool, Position> pos;
    std::pair<bool, int32_t> mass;
};

struct ObjectHandle
{
    std::atomic<Handle> handle = ObjectMemento::INVALID_ID;
};

class Object
{
public:
    constexpr static Handle INVALID_ID = ObjectMemento::INVALID_ID;

    Handle GetId() const { return m_id; }
    Position GetPosition() const { return m_pos; }
    int32_t GetMass() const { return m_mass; }

    Object& operator+=(ObjectMemento const& memento);
    Object& operator*=(ObjectMemento const& memento);

    bool operator==(Object const& other) const;
    bool operator!=(Object const& other) const;

private:
    friend class ObjectCollection;

    Object(ObjectMemento const& memento, ObjectCollection* parent = nullptr);

    void SetId(Handle id) { m_id = id; }
    void SetPos(Position const& pos) { m_pos = pos; }
    void SetMass(int32_t mass) { m_mass = mass; }

    ObjectCollection* m_pParent;
    Handle m_id;

    Position m_pos;
    int32_t m_mass;
};

class ObjectCollection
{
public:
    using Object = ccTestUtils::Object;

    ObjectCollection();

    Object* Spawn(ObjectHandle* pHandle, ObjectMemento const& memento);
    Object* Get(ObjectHandle* pHandle) const;
    bool Delete(ObjectHandle* pHandle);

private:
    std::unordered_map<ObjectHandle*, Object*> m_collection;
    Handle m_idCounter;
};

using ObjectChanges = sleipnir::utility::cc::Changes<ObjectMemento, ObjectHandle*>;
using ObjectIntegrator = ObjectChanges::Integrator<ObjectCollection>;

}

#endif // SLEIPNIR_ENGINE_TESTS_UTILITY_CC_CC_TEST_UTILS_HPP
