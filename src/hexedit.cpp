#include "hexedit.h"
#include <iostream>
#include <iomanip>

hexedit::hexedit_model::hexedit_model( std::fstream& file )
    : file( file )
    , current_page_idx( 0 )
{
    load_page( );
}

void hexedit::hexedit_model::load_page( )
{
    unsigned char raw_page[ PAGESIZE ] = { 0 };
    file.read( reinterpret_cast<char*>( raw_page ), PAGESIZE );

    int bytes_read = file.gcount( );

    if( file.eof( ) )
        bytes_read--;

    _page = std::vector<unsigned char>( raw_page, raw_page + bytes_read );
}

const std::vector<unsigned char>& hexedit::hexedit_model::page( ) const&
{
    return _page;
}

void hexedit::hexedit_page_view::update(
        const hexedit::hexedit_model& model )
{
    std::cout << "Page number: " << std::dec
        << model.current_page_idx + 1 << "\n";
    std::cout << std::setfill( ' ' ) << std::setw( 9 ) << "";
    std::cout << std::hex << std::uppercase << std::setfill( '0' );

    for( int i = 0; i < ROWSIZE; i++ )
    {
        std::cout << " " << std::setw( 2 ) << i;
    }

    std::cout << "   ";

    for( int i = 0; i < ROWSIZE; i++ )
    {
        std::cout << i;
    }

    std::cout << "\n";

    unsigned int rownum = 16 * model.current_page_idx * MAXROWS;
    unsigned long idx = 0;

    while( idx < model._page.size( ) )
    {
        int old_idx = idx, colnum = 0;

        std::cout << std::hex << std::uppercase << std::setfill( '0' );
        std::cout << std::setw( 8 ) << rownum << ":";

        while( colnum < ROWSIZE && idx < model._page.size( ) )
        {
            std::cout << " " << std::setw( 2 )
                << ( 0xFF & model._page[ idx ] );
            colnum++;
            idx++;
        }

        for( int i = 0; i < ROWSIZE - colnum; i++ )
        {
            std::cout << "   ";
        }

        std::cout << " | ";
        idx = old_idx;
        colnum = 0;

        while( colnum < ROWSIZE && idx < model._page.size( ) )
        {
            if( std::isprint( model._page[ idx ] )
                    && !std::isblank( model._page[ idx ] ) )
            {
                std::cout << model._page[ idx ];
            }
            else
            {
                std::cout << ".";
            }

            colnum++;
            idx++;
        }

        rownum += ROWSIZE;
        std::cout << std::dec << std::nouppercase << std::setfill( ' ' );
        std::cout << "\n";
    }
}

hexedit::hexedit_controller::hexedit_controller(
        hexedit::hexedit_model& model,
        hexedit::hexedit_view *view )
    : model( model )
    , view( view )
{
}

bool hexedit::hexedit_controller::previous_page( )
{
    if( model.current_page_idx == 0 )
        return false;

    if( model.file.eof( ) )
        model.file.clear( );

    model.current_page_idx--;
    model.file.seekg( model.current_page_idx * PAGESIZE );
    model.load_page( );

    return true;
}

bool hexedit::hexedit_controller::edit( const unsigned long long& row
                                      , const unsigned long long& col
                                      , const unsigned char& chr
                                      )
{
    if( model.file.eof( ) )
        model.file.clear( );

    model.file.seekp( row + col );
    model.file.write( reinterpret_cast<const char*>( &chr )
                    , sizeof( chr )
                    );

    /* reload contents of the page */
    model.file.seekg( model.current_page_idx * PAGESIZE );
    model.load_page( );

    return true;
}

bool hexedit::hexedit_controller::next_page( )
{
    if( model.file.eof( ) )
        return false;

    model.current_page_idx++;
    model.load_page( );

    return true;
}

void hexedit::hexedit_controller::update_view( )
{
    view->update( model );
}
