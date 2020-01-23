#include <catch2/catch.hpp>

#include <database.hpp>
#include <map>
#include <memory>
#include <list>

using namespace std;
struct test_db {
  int id;
  string pizza;
};

ostream & operator<<(ostream &o, const test_db &d) {
    o<<"[" << d.id << ", " << d.pizza << "]";
    return o;
}
//nullptr, the pointer literal

TEST_CASE("Object operations on database", "[database_c][constructors]") {

  SECTION("Object can be created") {

    REQUIRE_NOTHROW([]() { database_c<test_db> db; });
  }
  SECTION("Simple injection apply to works") {
    auto container = make_shared<map<int, test_db>>();
    database_c<test_db> db(container);
    REQUIRE_FALSE(db.get_container() == nullptr);
    REQUIRE(db.get_container() == container.get());
  }
}

TEST_CASE("Getting data from the database", "[database_c][get_all]") {
    auto container = make_shared<map<int, test_db>>();
    database_c<test_db> db(container);
    SECTION("get everything is present") {
        REQUIRE_NOTHROW( db.get_all() );
    }
    SECTION("get everything is present") {
        list<test_db> ret_list = db.get_all();
        REQUIRE(ret_list.size() == 0);
    }
    SECTION("get everything is present") {
        (*container)[0] = {0,"Americana"};
        list<test_db> ret_list = db.get_all();
        REQUIRE(db.get_container() == container.get());
        REQUIRE(db.get_container()->size() ==1);
        REQUIRE(ret_list.size() == 1);
    }
}

SCENARIO("getting data from database","[database_c][bdd][get_all]") {
    auto container = make_shared<map<int, test_db>>();
    database_c<test_db> db(container);
    GIVEN("we have some data in database") {
        (*container)[0] = {0,"Americana"};
        (*container)[1] = {1,"Hawai"};
        (*container)[2] = {2,"Wiejska"};
        (*container)[3] = {3,"Peperoni"};

        INFO((*container)[0]);
        WHEN("we get them from the database") {
            list<test_db> ret_list = db.get_all();

            THEN("the list should contain 4 elements") {
                CHECK(ret_list.size() == 4);
            }

            THEN("the list should contain element with the pizza Americana") {
                test_db found;
                for (auto e:ret_list) {
                    if (e.pizza == "Americana") found = e;
                }
                REQUIRE(found.pizza =="Americana");
            }
        }
    }
}


SCENARIO("Add data to database","[database_c][bdd][create]") {
    auto container = make_shared<map<int, test_db>>();
    auto e;
    database_c<test_db> db(container);
    GIVEN("create entity") {
        e = {0,"Capriciosa"};

        INFO((*container)[0]);
        WHEN("call add method") {
            list<test_db> ret_list = db.create(e);

            THEN("the list should contain 1 elements") {
                CHECK(ret_list.size() == 1);
            }

            THEN("the list should contain element with the pizza Test Element") {
                test_db found;
                for (auto e:ret_list) {
                    if (e.pizza == "Capriciosa") found = e;
                }
                REQUIRE(found.pizza =="Capriciosa");
            }
        }
    }
}


SCENARIO("Get data by ID from database","[database_c][bdd][getById]") {
    auto container = make_shared<map<int, test_db>>();
    database_c<test_db> db(container);
    GIVEN("create some test data") {
        (*container)[0] = {0,"Margherita"};
        (*container)[1] = {1,"Parma"};
        (*container)[2] = {2,"Roma"};

        INFO((*container)[0]);
        WHEN("call method add") {
            test_db ret = db.getById(0);

            THEN("should get element different from null") {
                CHECK(ret != NULL);
            }

            THEN("should been pizza Margherita") {
                CHECK(ret.pizza == "Margherita");
            }
        }
    }
}

SCENARIO("Get data by ID from database","[database_c][bdd][update]") {
    auto container = make_shared<map<int, test_db>>();
    database_c<test_db> db(container);
    GIVEN("create some test data") {
        (*container)[0] = {0,"Margherita"};
        (*container)[1] = {1,"Parma"};
        (*container)[2] = {2,"Roma"};

        INFO((*container)[0]);
        WHEN("call add method") {
            test_db ret = db.update("Salami", 0);

            THEN("Still should be same value in element of list") {
                CHECK(ret_list.size() == 3);
            }

            THEN("should get element witch changed pizza") {
                CHECK(ret == "Salami");
            }
        }
    }
}

SCENARIO("Remove data from database","[database_c][bdd][deleteById]") {
    auto container = make_shared<map<int, test_db>>();
    database_c<test_db> db(container);
    GIVEN("create some test data") {
        (*container)[0] = {0,"Margherita"};
        (*container)[1] = {1,"Parma"};
        (*container)[2] = {2,"Roma"};

        
        INFO((*container)[0]);
        WHEN("call add method") {
            list<test_db> ret_list = db.deleteById(0);

            THEN("the list should contain 2 elements") {
                CHECK(ret_list.size() == 2);
            }

            THEN("the list should not contain element with the pizza Margherita") {
                test_db found;
                for (auto e:ret_list) {
                    if (e.pizza == "Margherita") found = e;
                }
                CHECK(found == NULL);
            }
        }
    }
}