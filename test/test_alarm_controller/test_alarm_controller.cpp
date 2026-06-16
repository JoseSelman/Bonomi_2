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

void assertState(AlarmController& controller, AlarmState expected) {
  TEST_ASSERT_EQUAL(static_cast<int>(expected), static_cast<int>(controller.state()));
}
} // namespace

void setUp(void) {}

void tearDown(void) {}

void test_begin_accepts_valid_configuration(void) {
  AlarmController controller(kConfig);

  TEST_ASSERT_TRUE(controller.begin());

  TEST_ASSERT_TRUE(controller.isConfigured());
  assertState(controller, AlarmState::Disarmed);
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::None),
                    static_cast<int>(controller.alarmReason()));
}

void test_begin_rejects_invalid_pin_configuration(void) {
  const AlarmConfig invalidConfig = {
      30000U,
      15000U,
      3U,
      "12A4",
  };
  AlarmController controller(invalidConfig);

  TEST_ASSERT_FALSE(controller.begin());

  TEST_ASSERT_FALSE(controller.isConfigured());
  assertState(controller, AlarmState::Disarmed);
}

void test_req_001_disarmed_ignores_zone_events(void) {
  AlarmController controller = makeStartedController();

  controller.registerZoneEvent(ZoneType::Instant);
  controller.registerZoneEvent(ZoneType::EntryDelay);

  assertState(controller, AlarmState::Disarmed);
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::None),
                    static_cast<int>(controller.alarmReason()));
}

void test_req_002_valid_arm_starts_exit_delay(void) {
  AlarmController controller = makeStartedController();

  TEST_ASSERT_TRUE(controller.requestArm("1234"));

  assertState(controller, AlarmState::ExitDelay);
  TEST_ASSERT_EQUAL_UINT32(30000U, controller.remainingMs());
}

void test_req_002_exit_delay_finishes_in_armed_state(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));

  controller.tick(30000U);

  assertState(controller, AlarmState::Armed);
  TEST_ASSERT_EQUAL_UINT32(0U, controller.remainingMs());
}

void test_req_003_exit_delay_ignores_zone_events(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));

  controller.registerZoneEvent(ZoneType::EntryDelay);
  controller.registerZoneEvent(ZoneType::Instant);

  assertState(controller, AlarmState::ExitDelay);
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::None),
                    static_cast<int>(controller.alarmReason()));
}

void test_req_003_entry_zone_starts_entry_delay_when_armed(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);

  controller.registerZoneEvent(ZoneType::EntryDelay);

  assertState(controller, AlarmState::EntryDelay);
  TEST_ASSERT_EQUAL_UINT32(15000U, controller.remainingMs());
}

void test_req_004_valid_pin_disarms_during_entry_delay(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);
  controller.registerZoneEvent(ZoneType::EntryDelay);

  TEST_ASSERT_TRUE(controller.requestDisarm("1234"));

  assertState(controller, AlarmState::Disarmed);
  TEST_ASSERT_EQUAL_UINT32(0U, controller.remainingMs());
}

void test_req_005_entry_delay_timeout_raises_alarm(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);
  controller.registerZoneEvent(ZoneType::EntryDelay);

  controller.tick(15000U);

  assertState(controller, AlarmState::Alarm);
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::EntryTimeout),
                    static_cast<int>(controller.alarmReason()));
}

void test_req_006_instant_zone_raises_alarm_immediately(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);

  controller.registerZoneEvent(ZoneType::Instant);

  assertState(controller, AlarmState::Alarm);
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::InstantZone),
                    static_cast<int>(controller.alarmReason()));
}

void test_req_007_three_invalid_pins_raise_tamper_alarm(void) {
  AlarmController controller = makeStartedController();
  TEST_ASSERT_TRUE(controller.requestArm("1234"));
  controller.tick(30000U);

  TEST_ASSERT_FALSE(controller.requestDisarm("0000"));
  TEST_ASSERT_FALSE(controller.requestDisarm("1111"));
  TEST_ASSERT_FALSE(controller.requestDisarm("2222"));

  assertState(controller, AlarmState::Alarm);
  TEST_ASSERT_EQUAL_UINT8(3U, controller.invalidAttempts());
  TEST_ASSERT_EQUAL(static_cast<int>(AlarmReason::Tamper),
                    static_cast<int>(controller.alarmReason()));
}

int main(int argc, char** argv) {
  (void)argc;
  (void)argv;

  UNITY_BEGIN();
  RUN_TEST(test_begin_accepts_valid_configuration);
  RUN_TEST(test_begin_rejects_invalid_pin_configuration);
  RUN_TEST(test_req_001_disarmed_ignores_zone_events);
  RUN_TEST(test_req_002_valid_arm_starts_exit_delay);
  RUN_TEST(test_req_002_exit_delay_finishes_in_armed_state);
  RUN_TEST(test_req_003_exit_delay_ignores_zone_events);
  RUN_TEST(test_req_003_entry_zone_starts_entry_delay_when_armed);
  RUN_TEST(test_req_004_valid_pin_disarms_during_entry_delay);
  RUN_TEST(test_req_005_entry_delay_timeout_raises_alarm);
  RUN_TEST(test_req_006_instant_zone_raises_alarm_immediately);
  RUN_TEST(test_req_007_three_invalid_pins_raise_tamper_alarm);
  return UNITY_END();
}
