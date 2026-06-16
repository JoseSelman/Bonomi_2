/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#ifndef ALARM_CONTROLLER_H
#define ALARM_CONTROLLER_H

#include <stdint.h>

/**
 * @brief High-level state of the alarm controller.
 */
enum class AlarmState { Disarmed, ExitDelay, Armed, EntryDelay, Alarm };

/**
 * @brief Supported zone behavior.
 */
enum class ZoneType { EntryDelay, Instant };

/**
 * @brief Reason reported when the controller enters alarm state.
 */
enum class AlarmReason { None, InstantZone, EntryTimeout, Tamper };

/**
 * @brief Runtime configuration for the alarm controller.
 */
struct AlarmConfig {
  uint32_t exitDelayMs;
  uint32_t entryDelayMs;
  uint8_t maxInvalidAttempts;
  const char* pin;
};

/**
 * @brief Deterministic alarm state machine.
 *
 * The class contains only domain logic. Hardware adapters can call its public
 * methods from firmware code, while tests can exercise the same behavior on a
 * desktop target.
 */
class AlarmController {
public:
  /**
   * @brief Create a controller with the provided configuration.
   * @param config Delays, retry limit and access PIN.
   */
  explicit AlarmController(const AlarmConfig& config);

  /**
   * @brief Validate configuration and put the controller in a known state.
   * @return true if the configuration is usable.
   */
  bool begin();

  /**
   * @brief Request arming from the disarmed state.
   * @param pin Candidate PIN.
   * @return true if arming was accepted.
   */
  bool requestArm(const char* pin);

  /**
   * @brief Request disarming from any state.
   * @param pin Candidate PIN.
   * @return true if disarming was accepted.
   */
  bool requestDisarm(const char* pin);

  /**
   * @brief Notify that a zone detected activity.
   * @param zone Behavior configured for the zone.
   */
  void registerZoneEvent(ZoneType zone);

  /**
   * @brief Advance internal timers.
   * @param elapsedMs Elapsed milliseconds since the previous call.
   */
  void tick(uint32_t elapsedMs);

  AlarmState state() const;
  AlarmReason alarmReason() const;
  uint32_t remainingMs() const;
  uint8_t invalidAttempts() const;
  bool isConfigured() const;

private:
  bool validateConfig() const;
  bool isPinValid(const char* pin) const;
  void disarm();
  void raiseAlarm(AlarmReason reason);
  void registerInvalidAttempt();

  AlarmConfig config_;
  AlarmState state_;
  AlarmReason alarmReason_;
  uint32_t remainingMs_;
  uint8_t invalidAttempts_;
  bool configured_;
};

#endif
