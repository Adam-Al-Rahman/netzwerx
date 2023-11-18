#include "gtest/gtest.h"
#include "netzwerx/backend/mac_address/mac_address_mock.h"

namespace netzwerx::backend {

TEST(MacAddressLinuxTest, CorrectMacAddress) {
  MockMacLinuxInnerMethod mock_method;

  EXPECT_CALL(mock_method, super_user())
      .Times(testing::AtLeast(1))
      .WillRepeatedly(testing::Return("success"));

  EXPECT_CALL(mock_method, get_linux_network_interfaces())
      .Times(testing::AtLeast(1))
      .WillRepeatedly(
          testing::Return(std::vector<std::string>{"eth0", "lo", "wlan0"}));

  // Set expectations for linux_execute_command
  EXPECT_CALL(mock_method, linux_execute_command(testing::_))
      .Times(3)
      .WillRepeatedly(
          testing::Return(0));  // Simulate successful command execution

  std::string os = netzwerx::core::platform();
  if (os == "debian") {
    EXPECT_EQ((mac_linux_debian("eth0", "00:1A:2B:3C:5F:4E", mock_method)),
              "success");

    // TODO(Adam-Al-Rahman): Implement for non-sucess
    // EXPECT_EQ((mac_linux_debian("eth0", "00:2A:3B-AA:5F:4E", mock_method)),
    //           "!00:2A:3B-AA:5F:4E");
  }

  if (os == "arch") {
    EXPECT_EQ((mac_linux_arch("eth0", "00:1A:2B:3C:5F:4E", mock_method)), "success");

    // TODO(Adam-Al-Rahman): Implement for non-sucess
    // EXPECT_EQ((mac_linux_arch("eth0", "00:2A:3B-AA:5F:4E")),
    //           "!00:2A:3B-AA:5F:4E");
  }
}

// TEST(MacAddressTest, PosStartWith00) {
//   EXPECT_EQ(std::get<1>(mac_linux_debian(interface, "00:1A:2B:3C:5F:4E")),
//             "success");
//   EXPECT_EQ(std::get<1>(mac_linux_debian(interface, "00:2A:3B-AA:5F:4E")),
//             "!" + interface);

//   // Arch Linux
//   EXPECT_EQ(std::get<1>(mac_linux_arch(interface, "00:1A:2B:3C:5F:4E")),
//             "success");
//   EXPECT_EQ(std::get<1>(mac_linux_arch(interface, "00:2A:3B-AA:5F:4E")),
//             "!" + interface);
// }

// TEST(MacAddressTest, GetMacAddress) {
//   EXPECT_EQ(mac_changer("eth0", "00:1A:2B:3C:5F:4E"), 0);
//   EXPECT_EQ(mac_changer("eth0", "00:2A:3B-AA:5F:4E"), 1);
// }

// TEST(MacAddressTest, StartWith00) {
//   EXPECT_EQ(mac_changer("eth0", "00:1A:2B:3C:5F:4E"), 0);
//   EXPECT_EQ(mac_changer("eth0", "00:2A:3B-AA:5F:4E"), 1);
// }

}  // namespace netzwerx::backend