/*
 * Copyright (c) 2026
 * SPDX-License-Identifier: MIT
 */

#include "AlarmController.h"

#include <ctype.h>
#include <stddef.h>
#include <string.h>

namespace {
const size_t kMinPinLength = 4U;
const size_t kMaxPinLength = 8U;
} // namespace

AlarmController::AlarmController(const AlarmConfig& config)
    : config_(config), state_(AlarmState::Disarmed), alarmReason_(AlarmReason::None),
      remainingMs_(0U), invalidAttempts_(0U), configured_(false) {}

bool AlarmController::begin() {
  configured_ = validateConfig();
  disarm();
  return configured_;
}

bool AlarmController::requestArm(const char* pin) {
  if (!configured_ || state_ != AlarmState::Disarmed || !isPinValid(pin)) {
    return false;
  }

  state_ = AlarmState::ExitDelay;
  alarmReason_ = AlarmReason::None;
  remainingMs_ = config_.exitDelayMs;
  invalidAttempts_ = 0U;
  return true;
}

bool AlarmController::requestDisarm(const char* pin) {
  if (!configured_) {
    return false;
  }

  if (isPinValid(pin)) {
    disarm();
    return true;
  }

  if (state_ != AlarmState::Disarmed) {
    registerInvalidAttempt();
  }

  return false;
}

void AlarmController::registerZoneEvent(ZoneType zone) {
  if (!configured_ || state_ == AlarmState::Disarmed || state_ == AlarmState::ExitDelay ||
      state_ == AlarmState::Alarm) {
    return;
  }

  if (zone == ZoneType::Instant) {
    raiseAlarm(AlarmReason::InstantZone);
    return;
  }

  if (state_ == AlarmState::Armed) {
    state_ = AlarmState::EntryDelay;
    remainingMs_ = config_.entryDelayMs;
  }
}

void AlarmController::tick(uint32_t elapsedMs) {
  if (state_ != AlarmState::ExitDelay && state_ != AlarmState::EntryDelay) {
    return;
  }

  if (elapsedMs < remainingMs_) {
    remainingMs_ -= elapsedMs;
    return;
  }

  remainingMs_ = 0U;

  if (state_ == AlarmState::ExitDelay) {
    state_ = AlarmState::Armed;
    return;
  }

  raiseAlarm(AlarmReason::EntryTimeout);
}

AlarmState AlarmController::state() const {
  return state_;
}

AlarmReason AlarmController::alarmReason() const {
  return alarmReason_;
}

uint32_t AlarmController::remainingMs() const {
  return remainingMs_;
}

uint8_t AlarmController::invalidAttempts() const {
  return invalidAttempts_;
}

bool AlarmController::isConfigured() const {
  return configured_;
}

bool AlarmController::validateConfig() const {
  if (config_.exitDelayMs == 0U || config_.entryDelayMs == 0U || config_.maxInvalidAttempts == 0U ||
      config_.pin == nullptr) {
    return false;
  }

  const size_t pinLength = strlen(config_.pin);
  if (pinLength < kMinPinLength || pinLength > kMaxPinLength) {
    return false;
  }

  for (size_t index = 0U; index < pinLength; ++index) {
    if (isdigit(static_cast<unsigned char>(config_.pin[index])) == 0) {
      return false;
    }
  }

  return true;
}

bool AlarmController::isPinValid(const char* pin) const {
  return pin != nullptr && strcmp(pin, config_.pin) == 0;
}

void AlarmController::disarm() {
  state_ = AlarmState::Disarmed;
  alarmReason_ = AlarmReason::None;
  remainingMs_ = 0U;
  invalidAttempts_ = 0U;
}

void AlarmController::raiseAlarm(AlarmReason reason) {
  state_ = AlarmState::Alarm;
  alarmReason_ = reason;
  remainingMs_ = 0U;
}

void AlarmController::registerInvalidAttempt() {
  if (invalidAttempts_ < config_.maxInvalidAttempts) {
    ++invalidAttempts_;
  }

  if (invalidAttempts_ >= config_.maxInvalidAttempts) {
    raiseAlarm(AlarmReason::Tamper);
  }
}
