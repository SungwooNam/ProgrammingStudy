#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>


using namespace std;

BOOST_AUTO_TEST_SUITE( TestLog4cpp )


BOOST_AUTO_TEST_CASE( TestLogProperties ) 
{   
    try {
        
		log4cpp::PropertyConfigurator::configure("testConfig.log4cpp.properties");

    } catch(log4cpp::ConfigureFailure& f) {
        std::cout << "Configure Problem " << f.what() << std::endl;
		return;
    }

	log4cpp::Category& root = log4cpp::Category::getRoot();
	root.warn("Initialization goes weird but you can see this only in the console");
	 
    log4cpp::Category& ALL = 
        log4cpp::Category::getInstance(std::string("ALL"));
	 
    ALL.error( "During startup, serial port %s with baud rate %d can't be init!", "COM2", 9600 );
    ALL.warn( "During intial process, IO Device %d shows io %d get %d", 3 ,4, 5);

    log4cpp::Category& SEQUENCE = 
        log4cpp::Category::getInstance(std::string("ALL.SEQUENCE"));
	 
	SEQUENCE.info( "CLAMP start" );
	SEQUENCE.warn( "CLAMP io sensor %d is %d but should be %d", 24, 1, 0 );
	SEQUENCE.error( "CLAMP failed as timeout %d elapsed", 100 );
	 
    log4cpp::Category& INSPECTION = 
        log4cpp::Category::getInstance(std::string("ALL.INSPECTION"));

	INSPECTION.info( "Inspection start" );
	INSPECTION.warn( _T("검사 시작시 네트웍 상태 불안정") );
	INSPECTION.error( "Inspection failed", 100 );
 	
	log4cpp::Category::shutdown();
}


BOOST_AUTO_TEST_SUITE_END()