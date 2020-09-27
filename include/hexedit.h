/*! \file hexedit.h
 *  \brief Contains declarations for the MVC architecture of hexedit.
 *  \author Rimack Zelnick
 *  \version 0.1.0
 *  \date 2020-09-27
 *
 *  The hexedit project makes heavy use of the MVC architecture. The model
 *  holds information pertaining to an open file descriptor passed by the
 *  driver (i.e main() function), the controller operates on the model, and
 *  the view also has access to the model's protected/private members. The
 *  access of the controller/view to the model is done by means of class
 *  friendship.
 */
#ifndef HEXEDIT_H
#define HEXEDIT_H
#include <fstream>
#include <memory>
#include <vector>

#define ROWSIZE 16
#define MAXROWS 24
#define PAGESIZE (ROWSIZE*MAXROWS)

#ifndef NDEBUG
/*! \def DPRINTF(fmt,...)
 *  \brief Prints formatted debugging information.
 *
 *  When compiling this program with GCC's -g flag, this macro prints
 *  debugging information about the program including what filename, which
 *  line, and what function is it being called, plus a user-defined
 *  formatted printf() style message.
 */
#define DPRINTF(fmt,...)                                                   \
    do {                                                                   \
        fprintf(stderr, "[DEBUG] %s:%d:%s(): " fmt, __FILE__, __LINE__,    \
                __func__, __VA_ARGS__);                                    \
    } while( 0 )
#else
#define DPRINTF(fmt,...)
#endif /* !NDEBUG */

/*! \namespace hexedit
 *  \brief The namespace that contains all the classes, definitions, and
 *  objects of hexedit.
 */
namespace hexedit
{
    /*! \class hexedit_model
     *  \brief The model used by hexedit.
     *
     *  The model class holds a file descriptor, a STL vector that holds
     *  information about a "page", and the current page index. The term
     *  "page" refers to a chunk of bytes read from the file at any given
     *  moment in the program's execution, a page may contain up to 24 rows,
     *  and each row may contain up to 16 bytes.
     */
    class hexedit_model
    {
        public:
            /*! \fn hexedit_model( std::fstream& file )
             *  \brief Constructor for hexedit_model.
             *  \param[in,out] file A opened and valid file descriptor the
             *  file to be read or edited by hexedit.
             *  \exception std::ios_base::failure The file descriptor isn't
             *  properly opened.
             *
             *  Initialiazes all attrbutes. In order for the model properly
             *  work it must be opened with `std::ios_base::in`,
             *  `std::ios_base::out`, and `std::ios_base::binary`, a single
             *  page (first bytes of the file) is then loaded.
             */
            hexedit_model( std::fstream& file );

            /*! \fn void load_page( )
             *  \brief Loads a single page into memory.
             *  \exception std::ios_base::failure The file descriptor isn't
             *  properly opened.
             *
             *  The load page function loads a single page into memory
             *  the number of the page loaded is stored in
             *  `current_page_idx` variable.
             */
            void load_page( );

            /*! \fn const std::vector<unsigned char>& page( )
             *  \brief Gets the page.
             */
            const std::vector<unsigned char>& page( ) const&;

        protected:
             /*! Stores file descriptor. */
            std::fstream& file;

            /*! Stores chunks of bytes referred to as a "page". */
            std::vector<unsigned char> _page; 

            /*! Stores current page index number starting from zero. */
            unsigned long long current_page_idx; 

            friend class hexedit_controller;
            friend class hexedit_view;
            friend class hexedit_page_view;
    };

    /*! \class hexedit_view
     *  \brief Abstract view class.
     *
     *  The abstract view class is a template used by other views that
     *  specifies only one pure virtual member function, which is then
     *  specialiazed to describe a view.
     */
    class hexedit_view
    {
        public:
            /*! \fn void update( const hexedit_model& model )
             *  \brief Update the view.
             *  \param[in] model A reference of the model.
             *
             *  The update function is a pure virtual function inhereted by
             *  subclasses to create a unique view.
             */
            virtual void update( const hexedit_model& model ) = 0;
    };

    /*! \class hexedit_page_view
     *  \brief A view class specialized to view a page.
     *
     *  The page view class inherits and implements the update function from
     *  its parent to create a table view of the page.
     */
    class hexedit_page_view: public hexedit_view
    {
        public:
            /*! \fn void update( const hexedit_model& model )
             *  \brief Updates the view to show a page in table format.
             *  \param[in] model A reference of the model.
             *
             *  The update function creates a table format view of a page.
             */
            void update( const hexedit_model& model );
    };

    /*! \class hexedit_controller
     *  \brief The controller used by hexedit.
     *
     *  The controller class is used in the context of MVC to manipulate the
     *  application model, this isn't necessarily a driver of the program
     *  itself.
     */
    class hexedit_controller
    {
        public:
            /*! \fn hexedit_controller( hexedit_model& m, hexedit_view *v )
             *  \brief Constructs a controller.
             *  \param[in,out] m The application model.
             *  \param[in,out] v A reference to a view.
             */
            hexedit_controller( hexedit_model& model, hexedit_view *view );

            /*! \fn void update_view( )
             *  \brief Updates the view.
             *  
             *  The update view function makes a function call to
             *  `hexedit_view::update()` in order to create a view using
             *  the controller from the driver. The driver must never
             *  operate on the view.
             */
            void update_view( );

            /*! \fn bool previous_page( )
             *  \brief Gets the previous page.
             *  \return Whether or not the previous page was obtained.
             *
             *  The previous page function loads and decrements the current
             *  page index and loads a full page into memory. If the current
             *  page index is zero (i.e first page) then it must return
             *  false.
             */
            bool previous_page( );

            /*! \fn bool edit( )
             *  \brief Edits a single byte in the file.
             *  \param[in] row The row where the byte is located.
             *  \param[in] col The column where the byte is located.
             *  \param[in] chr The byte used to replace the old value.
             *  \return Whether or not the files has been edited.
             *  \exception std::ios_base::failure The model's file
             *  descriptor isn't properly opened.
             *
             *  The edit function allows for the replacement of a single
             *  byte in a file, if it cannot be written then an exception
             *  (which must be caught) is throw.
             */
            bool edit( const unsigned long long& row
                     , const unsigned long long& col
                     , const unsigned char& chr );

            /*! \fn bool next_page( )
             *  \brief Gets the next page.
             *  \return Whether or not the next page was obtained.
             *
             *  The next page function loads and inecrements the current
             *  page index and loads a full page into memory. If the current
             *  page index is zero (i.e first page) then it must return
             *  false.
             */
            bool next_page( );

        protected:
            /*! Stores a reference to the application model. */
            hexedit_model& model;

            /*! Stores a reference to an application view. */
            std::shared_ptr<hexedit_view> view;
    };
}

#endif /* HEXEDIT_H */
