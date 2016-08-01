#include <boost/test/unit_test.hpp>
#include "Poco/ActiveMethod.h"
#include "Poco/ActiveResult.h"
#include <utility>
#include <iostream>

using Poco::ActiveMethod;
using Poco::ActiveResult;

class ActiveAdder
{
public:
    ActiveAdder(): add(this, &ActiveAdder::addImpl)
    {
    }

    ActiveMethod<int, std::pair<int, int>, ActiveAdder> add;

private:
    int addImpl(const std::pair<int, int>& args)
    {
        return args.first + args.second;
    }
};

BOOST_AUTO_TEST_CASE( TestAdder )
{  
    ActiveAdder adder;

    ActiveResult<int> sum = adder.add(std::make_pair(1, 2));
    // do other things
    sum.wait();
    std::cout << sum.data() << std::endl;
}
