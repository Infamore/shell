#include "And_test.hpp"
#include "Or_test.hpp"
#include "Semi_test.hpp"
#include "Operatable_test.hpp"
#include "Paren_test.hpp"
#include "Test_test.hpp"

int main(int argc, char ** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
