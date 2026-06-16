/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include <AlarmController.h>

namespace {
const AlarmConfig kConfig = {
    30000U,
    15000U,
    3U,
    "1234",
};

AlarmController alarm(kConfig);
} // namespace

#ifdef ARDUINO
#include <Arduino.h>

void setup() {
  (void)alarm.begin();
}

void loop() {
  alarm.tick(10U);
  delay(10U);
}
#else
int main() {
  if (!alarm.begin()) {
    return 1;
  }

  return alarm.requestArm("1234") ? 0 : 2;
}
#endif
