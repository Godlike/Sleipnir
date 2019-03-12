#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sleipnir/utility/cc/Changes.hpp>
#include "ccTestUtils.hpp"

SCENARIO("Basic Changes::Instance", "[general]")
{
    using namespace ccTestUtils;

    ObjectCollection collection;
    ObjectChanges changes;

    GIVEN("empty cloned instance")
    {
        ObjectChanges::AddCollection adds;
        ObjectChanges::ModifyCollection modifies;
        ObjectChanges::DeleteCollection deletes;

        changes.Pull().Export(adds, modifies, deletes);

        REQUIRE(adds.empty());
        REQUIRE(modifies.empty());
        REQUIRE(deletes.empty());

        ObjectChanges::Instance instance = changes.Clone();

        REQUIRE(instance.IsEmpty());

        WHEN("adding")
        {
            ObjectMemento memento;

            instance.Add(memento);

            THEN("instance is no longer empty")
            {
                REQUIRE(false == instance.IsEmpty());
            }

            THEN("added operation is visible through Export")
            {
                instance.Export(adds, modifies, deletes);

                REQUIRE(false == adds.empty());
                REQUIRE(true == modifies.empty());
                REQUIRE(true == deletes.empty());
            }
        }

        WHEN("modifying")
        {
            ObjectMemento memento;

            instance.Modify(memento, &Object::operator+=);

            THEN("instance is no longer empty")
            {
                REQUIRE(false == instance.IsEmpty());
            }

            THEN("added operation is visible through Export")
            {
                instance.Export(adds, modifies, deletes);

                REQUIRE(true == adds.empty());
                REQUIRE(false == modifies.empty());
                REQUIRE(true == deletes.empty());
            }
        }

        WHEN("deleting")
        {
            ObjectMemento memento;

            instance.Delete(memento);

            THEN("instance is no longer empty")
            {
                REQUIRE(false == instance.IsEmpty());
            }

            THEN("added operation is visible through Export")
            {
                instance.Export(adds, modifies, deletes);

                REQUIRE(true == adds.empty());
                REQUIRE(true == modifies.empty());
                REQUIRE(false == deletes.empty());
            }
        }
    }
}

SCENARIO("Integration of Changes::Instance", "[general]")
{
    using namespace ccTestUtils;

    ObjectCollection collection;
    ObjectChanges changes;

    ObjectChanges::AddCollection adds;
    ObjectChanges::ModifyCollection modifies;
    ObjectChanges::DeleteCollection deletes;

    ObjectIntegrator integrator(collection);

    changes.Pull().Export(adds, modifies, deletes);

    REQUIRE(adds.empty());
    REQUIRE(modifies.empty());
    REQUIRE(deletes.empty());

    GIVEN("empty cloned instance")
    {
        ObjectChanges::Instance instance = changes.Clone();

        REQUIRE(instance.IsEmpty());

        WHEN("pushing operations")
        {
            ObjectMemento addMemento;
            addMemento.pos = {true, Position{ 0, 1, 0 } };

            ObjectMemento modifyMemento;
            modifyMemento.id = 1;
            modifyMemento.pos = {true, Position{ -1, -1, -1 } };
            modifyMemento.mass = {true, 10};

            instance.Add(addMemento);
            instance.Add(addMemento);
            instance.Modify(modifyMemento, &Object::operator+=);

            instance.Push();

            THEN("instance becomes empty")
            {
                REQUIRE(instance.IsEmpty());
            }

            THEN("pushed operations are visible through parent's Pull")
            {
                changes.Pull().Export(adds, modifies, deletes);

                REQUIRE(false == adds.empty());
                REQUIRE(false == modifies.empty());
                REQUIRE(true == deletes.empty());
            }

            THEN("pushed operations can be integrated")
            {
                ObjectChanges::Instance remote = changes.Pull();

                integrator.Integrate(remote);

                Object* pObj = collection.Get(modifyMemento);

                REQUIRE(nullptr != pObj);

                REQUIRE(pObj->GetPosition() == (addMemento.pos.second + modifyMemento.pos.second));
                REQUIRE(pObj->GetMass() == modifyMemento.mass.second);
            }

            THEN("every integration operates on current state of the collection")
            {
                ObjectChanges::Instance remote = changes.Pull();

                integrator.Integrate(remote);

                ObjectMemento deleteMemento;
                deleteMemento.id = 0;

                INFO("object is accessible before we integrate deletion");

                Object* pObj = collection.Get(deleteMemento);
                REQUIRE(nullptr != pObj);

                instance.Modify(modifyMemento, &Object::operator*=);
                instance.Delete(deleteMemento);

                instance.Push();

                remote = changes.Pull();
                integrator.Integrate(remote);

                INFO("object is not accessible after integrating deletion");

                pObj = collection.Get(deleteMemento);
                REQUIRE(nullptr == pObj);

                pObj = collection.Get(modifyMemento);

                REQUIRE(nullptr != pObj);
                REQUIRE(pObj->GetPosition() == (addMemento.pos.second + modifyMemento.pos.second) * modifyMemento.pos.second);
                REQUIRE(pObj->GetMass() == modifyMemento.mass.second * modifyMemento.mass.second);
            }
        }
    }
}
