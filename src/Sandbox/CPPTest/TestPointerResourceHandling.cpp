#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <boost\shared_ptr.hpp>
#include <windows.h>

using namespace std;

BOOST_AUTO_TEST_SUITE( TestResourceHandling )

HANDLE CreateTempFile()
{
    return CreateFile( "test.tmp", GENERIC_READ | GENERIC_WRITE, 0,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
}

BOOST_AUTO_TEST_CASE( TestOpenCloseMatched ) 
{  
    HANDLE hFile = CreateTempFile();
	BOOST_CHECK( hFile != INVALID_HANDLE_VALUE );

	HANDLE hOpenAgain = CreateTempFile();
	BOOST_CHECK_EQUAL( hOpenAgain, INVALID_HANDLE_VALUE );
    CloseHandle( hFile );

    hOpenAgain = CreateTempFile();
	BOOST_CHECK( hOpenAgain != INVALID_HANDLE_VALUE );
    CloseHandle( hOpenAgain );
}

typedef boost::shared_ptr<void> HandlePtr;
HandlePtr CreateTempFilePtr()
{
	return HandlePtr( CreateTempFile(), CloseHandle );
}

BOOST_AUTO_TEST_CASE( TestOpenCloseWithSharedPtr ) 
{  
	{
		HandlePtr file = CreateTempFilePtr();
		BOOST_CHECK( file.get() != INVALID_HANDLE_VALUE );

		HandlePtr fileAgain = file;
		// use the file
	}
}




BOOST_AUTO_TEST_SUITE_END() 