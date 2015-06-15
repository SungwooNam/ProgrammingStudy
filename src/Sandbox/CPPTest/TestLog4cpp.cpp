#include "StdAfx.h"

#include <stdexcept>
#include <boost\test\unit_test.hpp>
#include <log4cpp/Portability.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/NDC.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/PropertyConfigurator.hh>

double calcPi()
{
    double denominator = 3.0;
    double retVal = 4.0;
    long i;
    for (i = 0; i < 50000000l; i++)
    {
        retVal = retVal - (4.0 / denominator);
        denominator += 2.0;
        retVal = retVal + (4.0 /denominator);
        denominator += 2.0;
    }
    return retVal;
}


using namespace std;

BOOST_AUTO_TEST_SUITE( TestLog4cpp )

BOOST_AUTO_TEST_CASE( TestSimple ) 
{  
    try {
        
		log4cpp::PropertyConfigurator::configure("testConfig.log4cpp.properties");

    } catch(log4cpp::ConfigureFailure& f) {
        std::cout << "Configure Problem " << f.what() << std::endl;
		return;
    }

    log4cpp::Category& root = log4cpp::Category::getRoot();

    log4cpp::Category& sub1 = 
        log4cpp::Category::getInstance(std::string("sub1"));

    log4cpp::Category& sub2 = 
        log4cpp::Category::getInstance(std::string("sub1.sub2"));

    root.error("root error");
    root.warn("root warn");
    sub1.error("sub1 error");
    sub1.warn("sub1 warn");

    calcPi();

    sub2.error("sub2 error");
    sub2.warn("sub2 warn");

    root.error("root error");
    root.warn("root warn");
    sub1.error("sub1 error");
    sub1.warn("sub1 warn");

#ifdef WIN32
    Sleep(100);
#else
    sleep(3);
#endif

    sub2.error("sub2 error");
    sub2.warn("sub2 warn");
    sub2.error("%s %s %d", "test", "vform", 123);
    sub2.warnStream() << "streamed warn";

    sub2 << log4cpp::Priority::WARN << "warn2.." << "..warn3..value=" << 0 
         << log4cpp::eol << "..warn4";

    log4cpp::Category::shutdown();
}


BOOST_AUTO_TEST_SUITE_END()