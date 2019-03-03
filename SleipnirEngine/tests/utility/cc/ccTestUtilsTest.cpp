#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "ccTestUtils.hpp"

SCENARIO("Position", "[property]")
{
    using namespace ccTestUtils;

    GIVEN("two objects")
    {
        Position pos(1, 2, 3);
        Position diff(0, 1, 2);

        WHEN("object = arg1 + arg2")
        {
            Position final = pos + diff;

            THEN("arguments didn't change")
            {
                REQUIRE(1 == pos.x);
                REQUIRE(2 == pos.y);
                REQUIRE(3 == pos.z);

                REQUIRE(0 == diff.x);
                REQUIRE(1 == diff.y);
                REQUIRE(2 == diff.z);
            }

            THEN("resulting object properties are the sum of the two")
            {
                REQUIRE(pos.x + diff.x == final.x);
                REQUIRE(pos.y + diff.y == final.y);
                REQUIRE(pos.z + diff.z == final.z);
            }
        }

        WHEN("object += arg")
        {
            pos += diff;

            THEN("argument didn't change")
            {
                REQUIRE(0 == diff.x);
                REQUIRE(1 == diff.y);
                REQUIRE(2 == diff.z);
            }

            THEN("object's properties are the sum of the two")
            {
                REQUIRE(1 + diff.x == pos.x);
                REQUIRE(2 + diff.y == pos.y);
                REQUIRE(3 + diff.z == pos.z);
            }
        }

        WHEN("object = arg1 * arg2")
        {
            Position final = pos * diff;

            THEN("arguments didn't change")
            {
                REQUIRE(1 == pos.x);
                REQUIRE(2 == pos.y);
                REQUIRE(3 == pos.z);

                REQUIRE(0 == diff.x);
                REQUIRE(1 == diff.y);
                REQUIRE(2 == diff.z);
            }

            THEN("resulting object properties are the product of the two")
            {
                REQUIRE(pos.x * diff.x == final.x);
                REQUIRE(pos.y * diff.y == final.y);
                REQUIRE(pos.z * diff.z == final.z);
            }
        }

        WHEN("object *= arg")
        {
            pos *= diff;

            THEN("argument didn't change")
            {
                REQUIRE(0 == diff.x);
                REQUIRE(1 == diff.y);
                REQUIRE(2 == diff.z);
            }

            THEN("object's properties are the product of the two")
            {
                REQUIRE(1 * diff.x == pos.x);
                REQUIRE(2 * diff.y == pos.y);
                REQUIRE(3 * diff.z == pos.z);
            }
        }
    }
}

SCENARIO("Collection operations", "[collection]")
{
    using namespace ccTestUtils;

    GIVEN("one collection")
    {
        ObjectCollection collection;

        WHEN("creating objects")
        {
            ObjectMemento memento;

            Object* pObjA = collection.Spawn(memento);
            Object* pObjB = collection.Spawn(memento);

            REQUIRE(nullptr != pObjA);
            REQUIRE(nullptr != pObjB);

            THEN("created objects are different")
            {
                REQUIRE((*pObjA) != (*pObjB));
            }

            THEN("they can be accessed via Get() of their identifiable memento")
            {
                memento.id = pObjA->GetId();

                Object* pObjGet = collection.Get(memento);

                REQUIRE(nullptr != pObjGet);
                REQUIRE((*pObjA) == (*pObjGet));
            }

            THEN("they can be deleted via Delete() of their identifiable memento")
            {
                memento.id = pObjA->GetId();

                bool res = collection.Delete(memento);

                REQUIRE(true == res);
            }
        }
    }

    GIVEN("multiple collections")
    {
        ObjectCollection collectionA;
        ObjectCollection collectionB;

        WHEN("creating objects in each")
        {
            ObjectMemento memento;

            Object* pObjA = collectionA.Spawn(memento);
            Object* pObjB = collectionB.Spawn(memento);

            REQUIRE(nullptr != pObjA);
            REQUIRE(nullptr != pObjB);

            THEN("created objects independently increment id")
            {
                REQUIRE(pObjA->GetId() == pObjB->GetId());
            }

            THEN("created objects are different")
            {
                REQUIRE((*pObjA) != (*pObjB));
            }
        }
    }
}

SCENARIO("Object-memento operations", "[object]")
{
    using namespace ccTestUtils;

    GIVEN("collection and memento")
    {
        ObjectCollection collection;
        ObjectMemento memento;
        memento.pos = {true, Position{ 1, 2, 3 } };
        memento.mass = {true, 4};

        WHEN("we create an object")
        {
            Object* pObj = collection.Spawn(memento);
            REQUIRE(nullptr != pObj);

            THEN("it inherits memento properties")
            {
                REQUIRE(pObj->GetPosition() == memento.pos.second);
                REQUIRE(pObj->GetMass() == memento.mass.second);
            }
        }

        WHEN("we create an object and add a memento to it")
        {
            Object* pObj = collection.Spawn(memento);
            REQUIRE(nullptr != pObj);

            *pObj += memento;

            THEN("object inherits initial memento and adds given memento to itself")
            {
                REQUIRE((memento.pos.second + memento.pos.second) == pObj->GetPosition());
                REQUIRE((memento.mass.second + memento.mass.second) == pObj->GetMass());
            }
        }

        WHEN("we create an object and multiply it by a memento")
        {
            Object* pObj = collection.Spawn(memento);
            REQUIRE(nullptr != pObj);

            *pObj *= memento;

            THEN("object inherits initial memento and multiplies its properties by given memento")
            {
                REQUIRE((memento.pos.second * memento.pos.second) == pObj->GetPosition());
                REQUIRE((memento.mass.second * memento.mass.second) == pObj->GetMass());
            }
        }
    }
}
