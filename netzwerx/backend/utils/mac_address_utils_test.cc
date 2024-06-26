/**
 * Copyright 2023 Netzwerx
 */

#include "netzwerx/backend/utils/mac_address_utils.h"

#include "gtest/gtest.h"

namespace netzwerx::backend {

// TEST(Suite Name, Test name)
TEST(MacAddressUtilsTest, ExtractMacAddress) {
  EXPECT_EQ(regex_mac_address("00:1A:2B:3C:5F:4E"), "00:1A:2B:3C:5F:4E");
  EXPECT_EQ(regex_mac_address("00:1A:2B:3C:5F:4E; ls; echo "),
            "00:1A:2B:3C:5F:4E");
}
}  // namespace netzwerx::backend
