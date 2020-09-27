#include "hexedit.h"
#include <iostream>
#include <limits>
#define MAX_PATH 260

int main( )
{
    std::string path;

    std::cout << "Enter path to file: ";
    std::getline( std::cin, path );

    std::fstream file( path,
            std::ios::in | std::ios::out | std::ios::binary );

    if( !file )
    {
        std::cerr << "Couldn't open file " << path << "\n";
        return EXIT_FAILURE;
    }

    hexedit::hexedit_model model( file );
    bool update = true;

    while( true )
    {
        hexedit::hexedit_controller ctrl( model,
                new hexedit::hexedit_page_view );
        int option;
        
        if( update )
            ctrl.update_view( );

        update = true;

        do {
            std::cout << "\
Select an option:\n\
(P)revious page, (E)dit, (N)ext page, (Q)uit? ";
            if( ( option = std::cin.get( ) ) == std::cin.eof( ) )
                return EXIT_FAILURE;

            std::cin.ignore( 1, '\n' );
        } while( option != 'P'
              && option != 'p'
              && option != 'E'
              && option != 'e'
              && option != 'N'
              && option != 'n'
              && option != 'Q'
              && option != 'q' );

        switch( option )
        {
            case 'P':
            case 'p':
                if( !ctrl.previous_page( ) )
                {
                    std::cout << "Already in first page\n";
                    update = false;
                }
                break;
                
            case 'E':
            case 'e':
                {
                    unsigned long long selrow, selcol;
                    unsigned byte;

                    std::cout << "Select row (hex): ";
                    std::cin >> std::hex >> selrow;

                    std::cout << "Select column (hex): ";
                    std::cin >> std::hex >> selcol;

                    std::cout << "Overwrite byte with (hex): ";
                    std::cin >> std::hex >> byte;
                    std::cin.ignore(
                            std::numeric_limits<std::streamsize>::max( ),
                            '\n' );

                    try
                    {
                        ctrl.edit( selrow, selcol, byte );
                    }
                    catch( const std::ios_base::failure& )
                    {
                        std::cout << std::showbase << std::hex << "\
Unable to overwrite byte at row " << selrow << ", column " << selcol
<< ", ensure that the selection is valid.\n";
                        update = false;
                    }
                }
                break;

            case 'N':
            case 'n':
                if( !ctrl.next_page( ) )
                {
                    std::cout << "Already in last page\n";
                    update = false;
                }
                break;

            case 'Q':
            case 'q':
                return EXIT_SUCCESS;
                break;
        }
    }

    return EXIT_SUCCESS;
}
