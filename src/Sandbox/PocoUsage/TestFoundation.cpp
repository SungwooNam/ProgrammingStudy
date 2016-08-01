#include <boost/test/unit_test.hpp>
#include "Poco/BasicEvent.h"
#include "Poco/Delegate.h"
#include <iostream>

using Poco::BasicEvent;
using Poco::Delegate;

class Source
{
public:
    BasicEvent<int> theEvent;

    void fireEvent(int n)
    {
        theEvent(this, n);
    }
};

class Target
{
public:
    void onEvent(const void* pSender, int& arg)
    {
        std::cout << "onEvent: " << arg << std::endl;
    }
};

BOOST_AUTO_TEST_CASE( TestEvent )
{  
    Source source;
    Target target;

    source.theEvent += Delegate<Target, int>(
        &target, &Target::onEvent);

    source.fireEvent(42);

    source.theEvent -= Delegate<Target, int>(
        &target, &Target::onEvent);

}



