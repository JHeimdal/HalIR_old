#include "halir_pylib.hpp"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(halir_pylib)
{
  using namespace boost::python;
  class_< HalIR >("HalIR", init<std::std::string())
}
