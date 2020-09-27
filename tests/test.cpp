#include <hexedit.h>
#include <cstdio>
#include <cstring>
#include <criterion/criterion.h>

Test( hexedit_suite, controller_test )
{
    std::fstream file( "lorem_ipsum.txt" );

    cr_assert( file, "Failed to open file" );

    hexedit::hexedit_model model( file );
    hexedit::hexedit_controller ctrl( model,
            new hexedit::hexedit_page_view );

    const char page1[ ] = "\
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n\
incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis\n\
nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\n\
eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, s";
    const char page2[ ] = "\
unt\nin culpa qui officia deserunt mollit anim id est laborum.";

    cr_assert( strncmp( reinterpret_cast<const char*>(
                    model.page( ).data( ) ), page1, strlen( page1 ) ) == 0,
            "Page #1 is different" );

    ctrl.next_page( );

    cr_assert( strncmp( reinterpret_cast<const char*>(
                    model.page( ).data( ) ), page2, strlen( page2 ) ) == 0,
            "Page #2 is different" );

    ctrl.previous_page( );

    cr_assert( strncmp( reinterpret_cast<const char*>(
                    model.page( ).data( ) ), page1, strlen( page1 ) ) == 0,
            "Page #1 is different" );
}
