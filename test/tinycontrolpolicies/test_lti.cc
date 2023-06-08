#include <gtest/gtest.h>
#include "TCP_controller_LTI.hpp"



// Serialize and deserialize one integer
TEST(TCPTests, TCP_LTI_Instantiate) {
  tcp::Controller_LTI<5, 6, 7> pid;
}


