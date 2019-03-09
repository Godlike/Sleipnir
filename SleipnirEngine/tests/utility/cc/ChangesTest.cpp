#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sleipnir/utility/cc/Changes.hpp>
#include "ccTestUtils.hpp"

SCENARIO("Changes::Instance operations", "[general]")
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

    GIVEN("cloned instance")
    {
        ObjectChanges::Instance instance = changes.Clone();

        ObjectMemento addMemento;
        addMemento.pos = {true, Position{ 0, 1, 0 } };

        ObjectMemento modifyMemento;
        modifyMemento.id = 1;
        modifyMemento.pos = {true, Position{ -1, -1, -1 } };
        modifyMemento.mass = {true, 10};

        instance.Add(addMemento);
        instance.Add(addMemento);
        instance.Modify(modifyMemento, &Object::operator+=);

        WHEN("pushing operations")
        {
            instance.Push();

            REQUIRE(instance.IsEmpty());

            THEN("they are accessible through parent changes")
            {
                changes.Pull().Export(adds, modifies, deletes);

                REQUIRE(!adds.empty());
                REQUIRE(!modifies.empty());
                REQUIRE(deletes.empty());
            }

            THEN("they can be integrated")
            {
                ObjectChanges::Instance remote = changes.Pull();

                integrator.Integrate(remote);

                Object* pObj = collection.Get(modifyMemento);

                REQUIRE(nullptr != pObj);
                REQUIRE(pObj->GetPosition() == (addMemento.pos.second + modifyMemento.pos.second));
                REQUIRE(pObj->GetMass() == modifyMemento.mass.second);
            }

            THEN("integrations can be stacked")
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
