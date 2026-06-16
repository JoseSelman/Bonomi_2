# Pruebas de aceptacion

Las pruebas de aceptacion se implementan de forma automatica en `test/test_acceptance/test_acceptance.cpp`. Cubren flujo principal, flujo alternativo y flujo de excepcion.

## AT-001 - Flujo principal: armado, salida e intrusion

Objetivo: validar que el usuario pueda armar, salir durante la demora y que una intrusion posterior dispare la alarma.

| Paso | Accion | Resultado esperado |
| --- | --- | --- |
| 1 | Iniciar el controlador con configuracion valida. | Estado `Disarmed`. |
| 2 | Solicitar armado con PIN valido. | Estado `ExitDelay`. |
| 3 | Generar evento en zona temporizada durante salida. | No se dispara alarma. |
| 4 | Avanzar `exitDelayMs`. | Estado `Armed`. |
| 5 | Generar evento en zona instantanea. | Estado `Alarm`, motivo `InstantZone`. |

## AT-002 - Flujo alternativo: ingreso legitimo

Objetivo: validar que el usuario pueda ingresar por zona temporizada y desarmar antes del vencimiento.

| Paso | Accion | Resultado esperado |
| --- | --- | --- |
| 1 | Armar y esperar fin de demora de salida. | Estado `Armed`. |
| 2 | Generar evento en zona de entrada. | Estado `EntryDelay`. |
| 3 | Avanzar menos que `entryDelayMs`. | Sigue en `EntryDelay`. |
| 4 | Ingresar PIN valido. | Estado `Disarmed`, motivo `None`. |

## AT-003 - Flujo de excepcion: PIN incorrecto reiterado

Objetivo: validar la respuesta ante intento de sabotaje por PIN incorrecto.

| Paso | Accion | Resultado esperado |
| --- | --- | --- |
| 1 | Armar y entrar por zona temporizada. | Estado `EntryDelay`. |
| 2 | Ingresar tres PIN invalidos. | Estado `Alarm`, motivo `Tamper`. |

## Criterio de aprobacion

La suite de aceptacion se considera aprobada si todos los casos anteriores pasan en el entorno `native`:

```bash
pio test -e native -f test_acceptance
```
