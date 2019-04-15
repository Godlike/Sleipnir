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

        REQUIRE(true == instance.IsEmpty());

        WHEN("adding")
        {
            ObjectMemento memento;

            instance.Add(memento);

            THEN("instance is no longer empty")
            {
                REQUIRE(false == instance.IsEmpty());
            }

            THEN("added operation is visible through Export()")
            {
                instance.Export(adds, modifies, deletes);

                REQUIRE(false == adds.empty());
                REQUIRE(true == modifies.empty());
                REQUIRE(true == deletes.empty());

                REQUIRE(true == instance.IsEmpty());

            }

            THEN("Export() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Export(adds, modifies, deletes);

                REQUIRE(true == instance.IsEmpty());
            }

            THEN("Push() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Push();

                REQUIRE(true == instance.IsEmpty());
            }

            THEN("Reset() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Reset();

                REQUIRE(true == instance.IsEmpty());
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

            THEN("added operation is visible through Export()")
            {
                instance.Export(adds, modifies, deletes);

                REQUIRE(true == adds.empty());
                REQUIRE(false == modifies.empty());
                REQUIRE(true == deletes.empty());
            }

            THEN("Export() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Export(adds, modifies, deletes);

                REQUIRE(true == instance.IsEmpty());
            }

            THEN("Push() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Push();

                REQUIRE(true == instance.IsEmpty());
            }

            THEN("Reset() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Reset();

                REQUIRE(true == instance.IsEmpty());
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

            THEN("added operation is visible through Export()")
            {
                instance.Export(adds, modifies, deletes);

                REQUIRE(true == adds.empty());
                REQUIRE(true == modifies.empty());
                REQUIRE(false == deletes.empty());
            }

            THEN("Export() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Export(adds, modifies, deletes);

                REQUIRE(true == instance.IsEmpty());
            }

            THEN("Push() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Push();

                REQUIRE(true == instance.IsEmpty());
            }

            THEN("Reset() gets rid of queued operations")
            {
                REQUIRE(false == instance.IsEmpty());

                instance.Reset();

                REQUIRE(true == instance.IsEmpty());
            }
        }
    }

    GIVEN("Instance with several operations")
    {
        ObjectChanges::AddCollection adds;
        ObjectChanges::ModifyCollection modifies;
        ObjectChanges::DeleteCollection deletes;

        changes.Pull().Export(adds, modifies, deletes);

        REQUIRE(adds.empty());
        REQUIRE(modifies.empty());
        REQUIRE(deletes.empty());

        ObjectChanges::Instance instance = changes.Clone();

        REQUIRE(true == instance.IsEmpty());

        ObjectMemento addMemento;
        addMemento.pos = {true, Position{ 0, 1, 0 } };
        instance.Add(addMemento);

        ObjectChanges::ModifyOperation modOp = &Object::operator+=;

        ObjectMemento modifyMemento1;
        modifyMemento1.id = 1;
        modifyMemento1.pos = {true, Position{ -1, -1, -1 } };
        modifyMemento1.mass = {true, 10};

        ObjectMemento modifyMemento2;
        modifyMemento2.id = 2;
        modifyMemento2.mass = {true, 10};
        instance.Modify(modifyMemento1, modOp);
        instance.Modify(modifyMemento2, modOp);

        ObjectMemento deleteMemento;
        deleteMemento.id = 0;
        instance.Delete(deleteMemento);

        REQUIRE(false == instance.IsEmpty());

        WHEN("copying")
        {
            ObjectChanges::AddCollection addsCopy;
            ObjectChanges::ModifyCollection modifiesCopy;
            ObjectChanges::DeleteCollection deletesCopy;

            ObjectChanges::Instance instanceCopy(instance);

            REQUIRE(false == instanceCopy.IsEmpty());

            THEN("Exported operations are equal")
            {
                instance.Export(adds, modifies, deletes);
                instanceCopy.Export(addsCopy, modifiesCopy, deletesCopy);

                REQUIRE(adds == addsCopy);
                REQUIRE(modifies == modifiesCopy);
                REQUIRE(deletes == deletesCopy);
            }
        }

        WHEN("assign-copying")
        {
            ObjectChanges::AddCollection addsCopy;
            ObjectChanges::ModifyCollection modifiesCopy;
            ObjectChanges::DeleteCollection deletesCopy;

            ObjectChanges::Instance instanceCopy = changes.Clone();

            REQUIRE(true == instanceCopy.IsEmpty());

            instanceCopy = instance;

            REQUIRE(false == instanceCopy.IsEmpty());

            THEN("Exported operations are equal")
            {
                instance.Export(adds, modifies, deletes);
                instanceCopy.Export(addsCopy, modifiesCopy, deletesCopy);

                REQUIRE(adds == addsCopy);
                REQUIRE(modifies == modifiesCopy);
                REQUIRE(deletes == deletesCopy);
            }
        }

        WHEN("moving")
        {
            REQUIRE(false == instance.IsEmpty());

            ObjectChanges::Instance instanceMove(std::move(instance));

            THEN("Source becomes empty, destination contains queued operations")
            {
                REQUIRE(true == instance.IsEmpty());
                REQUIRE(false == instanceMove.IsEmpty());

                instanceMove.Export(adds, modifies, deletes);

                REQUIRE(adds.at(0) == addMemento);

                REQUIRE(modifies.at(0).first == modifyMemento1);
                REQUIRE(modifies.at(0).second == modOp);

                REQUIRE(modifies.at(1).first == modifyMemento2);
                REQUIRE(modifies.at(1).second == modOp);

                REQUIRE(deletes.at(0) == deleteMemento);
            }
        }

        WHEN("assign-moving")
        {
            REQUIRE(false == instance.IsEmpty());

            ObjectChanges::Instance instanceMove = changes.Clone();

            REQUIRE(true == instanceMove.IsEmpty());

            instanceMove = std::move(instance);

            THEN("Source becomes empty, destination contains queued operations")
            {
                REQUIRE(true == instance.IsEmpty());
                REQUIRE(false == instanceMove.IsEmpty());

                instanceMove.Export(adds, modifies, deletes);

                REQUIRE(adds.at(0) == addMemento);

                REQUIRE(modifies.at(0).first == modifyMemento1);
                REQUIRE(modifies.at(0).second == modOp);

                REQUIRE(modifies.at(1).first == modifyMemento2);
                REQUIRE(modifies.at(1).second == modOp);

                REQUIRE(deletes.at(0) == deleteMemento);
            }
        }

        WHEN("assign-copying")
        {
            ObjectChanges::AddCollection addsCopy;
            ObjectChanges::ModifyCollection modifiesCopy;
            ObjectChanges::DeleteCollection deletesCopy;

            ObjectChanges::Instance instanceCopy = changes.Clone();

            REQUIRE(true == instanceCopy.IsEmpty());

            instanceCopy = instance;

            REQUIRE(false == instanceCopy.IsEmpty());

            THEN("Exported operations are equal")
            {
                instance.Export(adds, modifies, deletes);
                instanceCopy.Export(addsCopy, modifiesCopy, deletesCopy);

                REQUIRE(adds == addsCopy);
                REQUIRE(modifies == modifiesCopy);
                REQUIRE(deletes == deletesCopy);
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

        REQUIRE(true == instance.IsEmpty());

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

            REQUIRE(false == instance.IsEmpty());

            instance.Push();

            THEN("instance becomes empty")
            {
                REQUIRE(true == instance.IsEmpty());
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
