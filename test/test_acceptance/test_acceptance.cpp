/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <AlarmController.h>
#include <unity.h>

namespace {
const AlarmConfig kConfig = {
    30000U,
    15000U,
    3U,
    "1234",
};

AlarmController makeStartedController() {
  AlarmController controller(kConfig);
  TEST_ASSERT_TRUE(controller.begin());
  return controller;
}
} // namespace

void setUp(void) {}

void tearDown(void) {}

void test_at_001_main_flow_arm_exit_and_detect_intrusion(void) {
  AlarmController controller = makeStartedController();

  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.registerZoneEvent(ZoneType::EntryDelay);
  controller.tick(30000U);
  controller.registerZoneEvent(ZoneType::Instant);

  TEST_ASSERT_EQUAL(static_cast<int>(AlarmState::Alarm), static_cast<int>(controller.state()));
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::InstantZone),
                    static_cast<int>(controller.alarmReason()));
}

void test_at_002_alternative_flow_user_enters_and_disarms(void) {
  AlarmController controller = makeStartedController();

  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);
  controller.registerZoneEvent(ZoneType::EntryDelay);
  controller.tick(7000U);
  TEST_ASSERT_TRUE(controller.requestDisarm("1234"));

  TEST_ASSERT_EQUAL(static_cast<int>(AlarmState::Disarmed), static_cast<int>(controller.state()));
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::None),
                    static_cast<int>(controller.alarmReason()));
}

void test_at_003_exception_flow_invalid_pin_attempts(void) {
  AlarmController controller = makeStartedController();

  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);
  controller.registerZoneEvent(ZoneType::EntryDelay);
  TEST_ASSERT_FALSE(controller.requestDisarm("1000"));
  TEST_ASSERT_FALSE(controller.requestDisarm("1001"));
  TEST_ASSERT_FALSE(controller.requestDisarm("1002"));

  TEST_ASSERT_EQUAL(static_cast<int>(AlarmState::Alarm), static_cast<int>(controller.state()));
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::Tamper),
                    static_cast<int>(controller.alarmReason()));
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_at_001_main_flow_arm_exit_and_detect_intrusion);
  RUN_TEST(test_at_002_alternative_flow_user_enters_and_disarms);
  RUN_TEST(test_at_003_exception_flow_invalid_pin_attempts);
  return UNITY_END();
}
