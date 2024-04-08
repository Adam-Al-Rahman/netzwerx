/**
 * Copyright 2023 Netzwerx
 */

#include "netzwerx/backend/mac_address/mac_address.h"
#include "netzwerx/backend/utils/mac_address_utils.h"
#include "pybind11/pybind11.h"  // from @pybind11

// namespace py = ::pybind11;

namespace {

PYBIND11_MODULE(_pywrap_mac_address, m) {
  m.doc() =
      "MAC Address this module handle the change and get the value of mac "
      "address";
  m.def("mac_changer", &netzwerx::backend::mac_changer,
        "It is use to change mac address of a specific interface");

  m.def("get_mac_address", &netzwerx::backend::get_mac_address);
  m.def("regex_mac_address", &netzwerx::backend::regex_mac_address);
}

}  // namespace
