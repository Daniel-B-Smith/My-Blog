#include <boost/python.hpp>

#include "skip_list.h"

using namespace boost::python;

typedef SkipList<int> SkipInt;
typedef SkipList<double> SkipFloat;
BOOST_PYTHON_MODULE(cpp_skip_lists)
{
  class_<SkipInt>("SkipInt")
    .def("insert", &SkipInt::insert)
    .def("delete", &SkipInt::remove)
    .def("reset", &SkipInt::reset)
    .def_readonly("n_elem", &SkipInt::n_elem)
    ;

  class_<SkipFloat>("SkipFloat")
    .def("insert", &SkipFloat::insert)
    .def("delete", &SkipFloat::remove)
    .def("reset", &SkipFloat::reset)
    .def_readonly("n_elem", &SkipFloat::n_elem)
    ;
}
