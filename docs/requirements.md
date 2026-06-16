# Requerimientos del sistema

## Sistema

Controlador de alarma embebido para una vivienda o laboratorio pequeno. El sistema se enfoca en la logica verificable: estados, temporizaciones, validacion de PIN y disparo de alarmas. La lectura real de sensores, teclado, sirena y comunicacion externa queda fuera del alcance y se considera adaptador de hardware.

## Partes interesadas

| Interesado | Necesidad |
| --- | --- |
| Usuario legitimo | Armar y desarmar la alarma sin disparos accidentales al entrar o salir. |
| Instalador | Configurar tiempos y PIN de forma simple y verificable. |
| Mantenedor | Probar la logica sin depender del hardware final. |
| Vecinos o responsables del lugar | Recibir alarma solo ante eventos relevantes. |

## Supuestos

- El PIN es numerico y tiene entre 4 y 8 digitos.
- Existen dos tipos de zona: entrada temporizada e instantanea.
- El tiempo se informa al controlador mediante llamadas periodicas a `tick`.
- La sirena, indicadores y sensores se conectan mediante adaptadores externos.

## Requerimientos funcionales

| ID | Requerimiento | Criterio de aceptacion |
| --- | --- | --- |
| REQ-001 | En estado desarmado, los eventos de zona no deben generar alarma. | Al registrar eventos de zona, el estado permanece `Disarmed` y el motivo es `None`. |
| REQ-002 | Con PIN valido, el usuario debe poder armar el sistema e iniciar una demora de salida. | `requestArm` devuelve verdadero, el estado pasa a `ExitDelay` y el contador queda en `exitDelayMs`. |
| REQ-003 | Durante la demora de salida, los eventos de zona deben ser ignorados para permitir salir. | Un evento en zona temporizada durante `ExitDelay` no cambia a alarma. |
| REQ-004 | Al terminar la demora de salida, el sistema debe quedar armado. | Al ejecutar `tick(exitDelayMs)`, el estado pasa a `Armed`. |
| REQ-005 | Una zona de entrada temporizada en estado armado debe iniciar una demora de entrada. | Un evento `EntryDelay` en estado `Armed` pasa a `EntryDelay` y carga `entryDelayMs`. |
| REQ-006 | Durante la demora de entrada, un PIN valido debe desarmar el sistema. | `requestDisarm` con PIN valido pasa a `Disarmed` y borra temporizadores. |
| REQ-007 | Si la demora de entrada vence sin PIN valido, debe dispararse alarma. | Al ejecutar `tick(entryDelayMs)`, el estado pasa a `Alarm` con motivo `EntryTimeout`. |
| REQ-008 | Una zona instantanea en estado armado debe disparar alarma inmediata. | Un evento `Instant` en `Armed` pasa a `Alarm` con motivo `InstantZone`. |
| REQ-009 | Tres intentos invalidos de PIN con el sistema protegido deben disparar alarma de sabotaje. | Luego de tres intentos fallidos, el estado es `Alarm` y el motivo `Tamper`. |
| REQ-010 | Una configuracion invalida debe ser rechazada antes de operar. | `begin` devuelve falso y el sistema no acepta armado. |

## Requerimientos no funcionales

| ID | Requerimiento | Evidencia |
| --- | --- | --- |
| RNF-001 | La logica de dominio debe ser independiente del hardware. | `lib/AlarmController` no incluye Arduino ni APIs de placa. |
| RNF-002 | El codigo debe compilar con advertencias estrictas. | `platformio.ini` usa `-Wall`, `-Wextra`, `-Wpedantic` y `-Wconversion`. |
| RNF-003 | El formato debe ser automatico y reproducible. | `.clang-format`, `.vscode/settings.json` y `.pre-commit-config.yaml`. |
| RNF-004 | La API publica debe documentarse con Doxygen. | Comentarios en `AlarmController.h` y configuracion `Doxyfile`. |

## Matriz de trazabilidad

| Requerimiento | Prueba unitaria | Prueba de aceptacion |
| --- | --- | --- |
| REQ-001 | `test_req_001_disarmed_ignores_zone_events` | - |
| REQ-002 | `test_req_002_valid_arm_starts_exit_delay` | `test_at_001_main_flow_arm_exit_and_detect_intrusion` |
| REQ-003 | `test_req_003_exit_delay_ignores_zone_events` | `test_at_001_main_flow_arm_exit_and_detect_intrusion` |
| REQ-004 | `test_req_002_exit_delay_finishes_in_armed_state` | `test_at_001_main_flow_arm_exit_and_detect_intrusion` |
| REQ-005 | `test_req_003_entry_zone_starts_entry_delay_when_armed` | `test_at_002_alternative_flow_user_enters_and_disarms` |
| REQ-006 | `test_req_004_valid_pin_disarms_during_entry_delay` | `test_at_002_alternative_flow_user_enters_and_disarms` |
| REQ-007 | `test_req_005_entry_delay_timeout_raises_alarm` | - |
| REQ-008 | `test_req_006_instant_zone_raises_alarm_immediately` | `test_at_001_main_flow_arm_exit_and_detect_intrusion` |
| REQ-009 | `test_req_007_three_invalid_pins_raise_tamper_alarm` | `test_at_003_exception_flow_invalid_pin_attempts` |
| REQ-010 | `test_begin_rejects_invalid_pin_configuration` | - |
