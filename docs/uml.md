# Diagrama de clases UML

El siguiente diagrama resume el dominio implementado. Se usa Mermaid para que el diagrama sea versionable junto con el codigo.

```mermaid
classDiagram
    class AlarmController {
        -AlarmConfig config_
        -AlarmState state_
        -AlarmReason alarmReason_
        -uint32_t remainingMs_
        -uint8_t invalidAttempts_
        -bool configured_
        +begin() bool
        +requestArm(pin) bool
        +requestDisarm(pin) bool
        +registerZoneEvent(zone) void
        +tick(elapsedMs) void
        +state() AlarmState
        +alarmReason() AlarmReason
        +remainingMs() uint32_t
        +invalidAttempts() uint8_t
        +isConfigured() bool
    }

    class AlarmConfig {
        +uint32_t exitDelayMs
        +uint32_t entryDelayMs
        +uint8_t maxInvalidAttempts
        +const char* pin
    }

    class AlarmState {
        <<enumeration>>
        Disarmed
        ExitDelay
        Armed
        EntryDelay
        Alarm
    }

    class ZoneType {
        <<enumeration>>
        EntryDelay
        Instant
    }

    class AlarmReason {
        <<enumeration>>
        None
        InstantZone
        EntryTimeout
        Tamper
    }

    AlarmController --> AlarmConfig : uses
    AlarmController --> AlarmState : reports
    AlarmController --> AlarmReason : reports
    AlarmController --> ZoneType : receives
```

## Relacion con el modelo de dominio

- `AlarmController` concentra la maquina de estados verificable.
- `AlarmConfig` representa parametros acordados durante instalacion.
- `ZoneType` abstrae el origen fisico del evento.
- `AlarmReason` conserva evidencia del motivo de disparo para diagnostico.
