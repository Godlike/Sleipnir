#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <sleipnir/utility/cc/Changes.hpp>
#include "ccTestUtils.hpp"

SCENARIO("Changes::Instance operations", "[general]")
{
    using namespace ccTestUtils;

    ObjectChanges changes;

    ObjectChanges::AddCollection adds;
    ObjectChanges::ModifyCollection modifies;
    ObjectChanges::DeleteCollection deletes;

    changes.Pull().Export(adds, modifies, deletes);

    REQUIRE(adds.empty());
    REQUIRE(modifies.empty());
    REQUIRE(deletes.empty());

    GIVEN("cloned instance")
    {
        ObjectChanges::Instance instance = changes.Clone();

        ObjectMemento addMemento;
        addMemento.pos = {true, Position{ 1, 1, 1 } };

        ObjectMemento modifyMemento;
        modifyMemento.id = 0;
        modifyMemento.pos = {true, Position{ -1, -1, -1 } };
        modifyMemento.mass = {true, 10};

        instance.Add(addMemento);
        instance.Modify(modifyMemento, &Object::operator+=);

        WHEN("pushing operations")
        {
            instance.Push();

            THEN("they are accessible through parent changes")
            {
                changes.Pull().Export(adds, modifies, deletes);

                REQUIRE(!adds.empty());
                REQUIRE(!modifies.empty());
                REQUIRE(deletes.empty());
            }
        }
    }
}
