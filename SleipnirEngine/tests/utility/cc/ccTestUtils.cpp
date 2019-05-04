#include "ccTestUtils.hpp"

#include <algorithm>
#include <cassert>

namespace ccTestUtils
{

// Position

Position::Position() : x(0), y(0), z(0)
{

}


Position::Position(int32_t _x, int32_t _y, int32_t _z) : x(_x), y(_y), z(_z)
{

}

bool Position::operator==(Position const& other) const
{
    return x == other.x
        && y == other.y
        && z == other.z;
}

Position Position::operator+(Position const& other) const
{
    return Position{
        x + other.x
        , y + other.y
        , z + other.z
    };
}

Position Position::operator+(int32_t offset) const
{
    return Position{
        x + offset
        , y + offset
        , z + offset
    };
}

Position& Position::operator+=(Position const& other)
{
    x += other.x;
    y += other.y;
    z += other.z;

    return *this;
}

Position& Position::operator+=(int32_t offset)
{
    x += offset;
    y += offset;
    z += offset;

    return *this;
}

Position Position::operator*(Position const& other) const
{
    return Position{
        x * other.x
        , y * other.y
        , z * other.z
    };
}

Position Position::operator*(int32_t offset) const
{
    return Position{
        x * offset
        , y * offset
        , z * offset
    };
}

Position& Position::operator*=(Position const& other)
{
    x *= other.x;
    y *= other.y;
    z *= other.z;

    return *this;
}

Position& Position::operator*=(int32_t offset)
{
    x *= offset;
    y *= offset;
    z *= offset;

    return *this;
}

// Object

Object& Object::operator+=(ObjectMemento const& memento)
{
    if (memento.pos.first)
    {
        m_pos += memento.pos.second;
    }

    if (memento.mass.first)
    {
        m_mass += memento.mass.second;
    }

    return *this;
}

Object& Object::operator*=(ObjectMemento const& memento)
{
    if (memento.pos.first)
    {
        m_pos *= memento.pos.second;
    }

    if (memento.mass.first)
    {
        m_mass *= memento.mass.second;
    }

    return *this;
}

bool Object::operator==(Object const& other) const
{
    return m_pParent == other.m_pParent
        && m_id == other.m_id;
}

bool Object::operator!=(Object const& other) const
{
    return !operator==(other);
}

Object::Object(ObjectMemento const& memento, ObjectCollection* parent)
    : m_pParent(parent)
    , m_id(INVALID_ID)
    , m_pos(memento.pos.first ? memento.pos.second : Position())
    , m_mass(memento.mass.first ? memento.mass.second : 0)
{

}

// ObjectCollection

ObjectCollection::ObjectCollection()
    : m_idCounter(0)
{}

Object* ObjectCollection::Spawn(ObjectHandle* pHandle, ObjectMemento const& memento)
{
    assert(nullptr == Get(pHandle));

    Object* pObj = new Object(memento, this);

    pObj->SetId(m_idCounter);
    pHandle->handle = m_idCounter;

    ++m_idCounter;

    m_collection.insert({pHandle, pObj});

    return pObj;

}

Object* ObjectCollection::Get(ObjectHandle* pHandle) const
{
    auto it = m_collection.find(pHandle);

    return (it != m_collection.cend() ? it->second : nullptr);
}

bool ObjectCollection::Delete(ObjectHandle* pHandle)
{
    Object* pObj = Get(pHandle);

    if (nullptr != pObj)
    {
        m_collection.erase(pHandle);

        delete pObj;

        return true;
    }
    else
    {
        return false;
    }
}

}
