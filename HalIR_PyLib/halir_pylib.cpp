#include "../HalIR/halir.hpp"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(halir_pylib)
{
  using namespace boost::python;
  class_< HalIR >("HalIR", init<std::string>())
    .def("createCalibrationSpectra", &HalIR::createCalibrationSpectra)
    .def("calcSpectra", &HalIR::calcSpectra)
    .def("getSpectra", &HalIR::getSpectra);
}
