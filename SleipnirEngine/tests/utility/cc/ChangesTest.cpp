#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sleipnir/utility/cc/Changes.hpp>
#include "ccTestUtils.hpp"

#include <numeric>

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

                instance.Push(ObjectChanges::TimeUnit{0});

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

            instance.Modify(nullptr, memento, &Object::operator+=);

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

                instance.Push(ObjectChanges::TimeUnit{0});

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
            instance.Delete(nullptr);

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

                instance.Push(ObjectChanges::TimeUnit{0});

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
        ObjectHandle* addHandle = instance.Add(addMemento);

        ObjectChanges::ModifyOperation modOp = &Object::operator+=;

        ObjectHandle* modifyHandle1 = new ObjectHandle{1};
        ObjectMemento modifyMemento1;
        modifyMemento1.pos = {true, Position{ -1, -1, -1 } };
        modifyMemento1.mass = {true, 10};

        ObjectHandle* modifyHandle2 = new ObjectHandle{2};
        ObjectMemento modifyMemento2;
        modifyMemento2.mass = {true, 10};
        instance.Modify(modifyHandle1, modifyMemento1, modOp);
        instance.Modify(modifyHandle2, modifyMemento2, modOp);

        ObjectHandle* deleteHandle = new ObjectHandle{0};
        instance.Delete(deleteHandle);

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

                REQUIRE(adds.cend() != adds.find(addHandle));
                REQUIRE(adds[addHandle] == addMemento);

                REQUIRE(modifies.cend() != modifies.find(modifyHandle1));
                REQUIRE(std::get<1>(modifies[modifyHandle1]) == modOp);

                REQUIRE(modifies.cend() != modifies.find(modifyHandle2));
                REQUIRE(std::get<1>(modifies[modifyHandle2]) == modOp);

                REQUIRE(deletes.at(0) == deleteHandle);
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

                REQUIRE(adds.cend() != adds.find(addHandle));
                REQUIRE(adds[addHandle] == addMemento);

                REQUIRE(modifies.cend() != modifies.find(modifyHandle1));
                REQUIRE(std::get<1>(modifies[modifyHandle1]) == modOp);

                REQUIRE(modifies.cend() != modifies.find(modifyHandle2));
                REQUIRE(std::get<1>(modifies[modifyHandle2]) == modOp);

                REQUIRE(deletes.at(0) == deleteHandle);
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

            ObjectHandle* addHandle0 = instance.Add(addMemento);
            ObjectHandle* addHandle1 = instance.Add(addMemento);
            instance.Modify(addHandle1, modifyMemento, &Object::operator+=);

            REQUIRE(false == instance.IsEmpty());

            instance.Push(ObjectChanges::TimeUnit{0});

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

                Object* pObj0 = collection.Get(addHandle0);
                REQUIRE(nullptr != pObj0);

                REQUIRE(pObj0->GetPosition() == addMemento.pos.second);
                REQUIRE(pObj0->GetMass() == addMemento.mass.second);

                Object* pObj1 = collection.Get(addHandle1);

                REQUIRE(nullptr != pObj1);

                REQUIRE(pObj1->GetPosition() == (addMemento.pos.second + modifyMemento.pos.second));
                REQUIRE(pObj1->GetMass() == modifyMemento.mass.second);
            }

            THEN("every integration operates on current state of the collection")
            {
                ObjectChanges::Instance remote = changes.Pull();

                integrator.Integrate(remote);

                INFO("object is accessible before we integrate deletion");

                Object* pObj = collection.Get(addHandle0);
                REQUIRE(nullptr != pObj);

                instance.Modify(addHandle1, modifyMemento, &Object::operator*=);
                instance.Delete(addHandle0);

                instance.Push(ObjectChanges::TimeUnit{0});

                remote = changes.Pull();
                integrator.Integrate(remote);

                INFO("object is not accessible after integrating deletion");

                pObj = collection.Get(addHandle0);
                REQUIRE(nullptr == pObj);

                pObj = collection.Get(addHandle1);

                REQUIRE(nullptr != pObj);
                REQUIRE(pObj->GetPosition() == (addMemento.pos.second + modifyMemento.pos.second) * modifyMemento.pos.second);
                REQUIRE(pObj->GetMass() == modifyMemento.mass.second * modifyMemento.mass.second);
            }
        }

        WHEN("pulling operations")
        {
            ObjectMemento addMemento;
            addMemento.pos = {true, Position{ 0, 1, 0 } };

            ObjectMemento modifyMemento;
            modifyMemento.id = 1;
            modifyMemento.pos = {true, Position{ -1, -1, -1 } };
            modifyMemento.mass = {true, 10};

            std::array<std::size_t, 3> addCount = {0};
            std::array<std::size_t, 3> modifyCount = {0};
            std::array<std::size_t, 3> deleteCount = {0};

            instance.Add(addMemento);
            ++addCount[0];
            ObjectHandle* addHandle1 = instance.Add(addMemento);
            ++addCount[0];
            instance.Push(ObjectChanges::TimeUnit{0});

            REQUIRE(true == instance.IsEmpty());

            instance.Add(addMemento);
            ++addCount[1];
            instance.Modify(addHandle1, modifyMemento, &Object::operator+=);
            ++modifyCount[1];

            REQUIRE(false == instance.IsEmpty());

            instance.Push(ObjectChanges::TimeUnit{1});

            REQUIRE(true == instance.IsEmpty());

            instance.Add(addMemento);
            ++addCount[2];

            instance.Delete(addHandle1);
            ++deleteCount[2];

            instance.Push(ObjectChanges::TimeUnit{2});

            REQUIRE(true == instance.IsEmpty());

            THEN("by default all pushes are pulled")
            {
                changes.Pull().Export(adds, modifies, deletes);

                REQUIRE(std::accumulate(addCount.cbegin(), addCount.cend(), 0) == adds.size());
                REQUIRE(std::accumulate(modifyCount.cbegin(), modifyCount.cend(), 0) == modifies.size());
                REQUIRE(std::accumulate(deleteCount.cbegin(), deleteCount.cend(), 0) == deletes.size());
            }

            THEN("provided timestamp acts as a threshold")
            {
                changes.Pull(ObjectChanges::TimeUnit{1}).Export(adds, modifies, deletes);

                constexpr std::size_t iteratorOffset = 1 + 1; // point to one past desired end

                REQUIRE(std::accumulate(addCount.cbegin(), addCount.cbegin() + iteratorOffset, 0) == adds.size());
                REQUIRE(std::accumulate(modifyCount.cbegin(), modifyCount.cbegin() + iteratorOffset, 0) == modifies.size());
                REQUIRE(std::accumulate(deleteCount.cbegin(), deleteCount.cbegin() + iteratorOffset, 0) == deletes.size());
            }
        }
    }
}
