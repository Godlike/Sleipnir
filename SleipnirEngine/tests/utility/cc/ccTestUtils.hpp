#ifndef SLEIPNIR_ENGINE_TESTS_UTILITY_CC_CC_TEST_UTILS_HPP
#define SLEIPNIR_ENGINE_TESTS_UTILITY_CC_CC_TEST_UTILS_HPP

#include <sleipnir/utility/cc/Changes.hpp>

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

class Object;
class ObjectCollection;

struct ObjectMemento
{
    using Object = ccTestUtils::Object;
    constexpr static uint16_t INVALID_ID = 0xFFFF;

    uint16_t id = INVALID_ID;

    std::pair<bool, Position> pos;
    std::pair<bool, int32_t> mass;
};

class Object
{
public:
    constexpr static uint16_t INVALID_ID = ObjectMemento::INVALID_ID;

    uint16_t GetId() const { return m_id; }
    Position GetPosition() const { return m_pos; }
    int32_t GetMass() const { return m_mass; }

    Object& operator+=(ObjectMemento const& memento);
    Object& operator*=(ObjectMemento const& memento);

    bool operator==(Object const& other) const;
    bool operator!=(Object const& other) const;

private:
    friend class ObjectCollection;

    Object(ObjectMemento const& memento, ObjectCollection* parent = nullptr);

    void SetId(uint16_t id) { m_id = id; }
    void SetPos(Position const& pos) { m_pos = pos; }
    void SetMass(int32_t mass) { m_mass = mass; }

    ObjectCollection* m_pParent;
    uint16_t m_id;

    Position m_pos;
    int32_t m_mass;
};

class ObjectCollection
{
public:
    using Object = ccTestUtils::Object;

    ObjectCollection();

    Object* Spawn(ObjectMemento const& memento);
    Object* Get(ObjectMemento const& memento) const;
    bool Delete(ObjectMemento const& memento);

private:
    std::vector<Object*> m_collection;
    uint16_t m_idCounter;
};

using ObjectChanges = sleipnir::utility::cc::Changes<ObjectMemento>;
using ObjectIntegrator = ObjectChanges::Integrator<ObjectCollection>;

}

#endif // SLEIPNIR_ENGINE_TESTS_UTILITY_CC_CC_TEST_UTILS_HPP
