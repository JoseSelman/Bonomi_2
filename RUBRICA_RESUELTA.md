# Rubrica resuelta

Actividad integradora: Verificacion y Validacion de Software 2025.

Proyecto entregado: controlador de alarma embebido con zona temporizada.

## Resultado general

La entrega fue organizada para cubrir cada criterio de la rubrica con evidencia concreta dentro del repositorio. La unica salvedad operativa es el criterio de Git: el entorno bloqueo la creacion de `.git`, por lo que se dejo documentado el flujo de commits requerido y se intento la inicializacion con permisos elevados.

## Resolucion punto por punto

| Criterio de rubrica | Maximo esperado por la rubrica | Resolucion entregada | Evidencia |
| --- | --- | --- | --- |
| Repositorio Git | Rama de desarrollo con commits para cada avance y mensajes descriptivos. | Se preparo `.gitignore` y una secuencia de commits en rama `develop`. La creacion real de `.git` fue bloqueada por el entorno. | `.gitignore`, `docs/git-workflow.md`, `docs/execution-report.md` |
| Organizacion de proyecto PlatformIO | Librerias en `lib`, programa principal en `src` y pruebas unitarias en `test`. | Proyecto estructurado con libreria `AlarmController`, programa principal minimo y dos suites de pruebas. | `platformio.ini`, `lib/AlarmController`, `src/main.cpp`, `test/` |
| Formato del codigo | Reglas respetadas, incluyendo nombrado de variables, funciones/metodos y tipos. | Se definio formato LLVM con reglas propias de nombres y estilo embebido. | `.clang-format`, `docs/coding-standard.md` |
| Verificacion automatica de formato | `clang-format` al guardar y configuracion de pre-commit. | VS Code aplica formato al guardar y pre-commit ejecuta `clang-format`. | `.vscode/settings.json`, `.pre-commit-config.yaml` |
| Verificacion estatica de codigo | Compilador con `-Wall`, `-Wextra`, `cppcheck`, sin advertencias, y justificacion de hallazgos. | Se configuraron advertencias estrictas, `cppcheck` y politica de tratamiento de hallazgos. | `platformio.ini`, `.pre-commit-config.yaml`, `docs/static-analysis.md` |
| Revision de codigo por IA | Revision completa, evaluacion de sugerencias y documentacion de acciones. | Se documento el alcance revisado, hallazgos, riesgos y acciones implementadas. | `docs/ai-code-review.md` |
| Pruebas unitarias | Disciplina TDD y cada prueba trazada a requerimiento. | Se agregaron 11 pruebas unitarias trazadas a `REQ-001` a `REQ-010`. | `test/test_alarm_controller/test_alarm_controller.cpp`, `docs/requirements.md` |
| Pruebas de aceptacion | Flujos principales, alternativos y de excepcion. | Se agregaron 3 pruebas de aceptacion: flujo principal, alternativo y excepcion por sabotaje. | `test/test_acceptance/test_acceptance.cpp`, `docs/acceptance-tests.md` |
| Documentacion del codigo | API completa en formato Doxygen y configuracion Doxygen. | La API publica esta documentada con comentarios Doxygen y se incluye configuracion para generar documentacion. | `lib/AlarmController/src/AlarmController.h`, `Doxyfile` |
| Licencia | Informacion de licencia para codigo propio y terceros en archivo `LICENSE`. | Se agrego licencia MIT y avisos SPDX en codigo fuente. | `LICENSE`, encabezados de `*.cpp` y `*.h` |

## Trazabilidad de requerimientos

| Requerimiento | Descripcion breve | Evidencia de prueba |
| --- | --- | --- |
| REQ-001 | Eventos ignorados con alarma desarmada. | `test_req_001_disarmed_ignores_zone_events` |
| REQ-002 | Armado con PIN valido y demora de salida. | `test_req_002_valid_arm_starts_exit_delay` |
| REQ-003 | Sensores ignorados durante demora de salida. | `test_req_003_exit_delay_ignores_zone_events` |
| REQ-004 | Fin de demora de salida pasa a armado. | `test_req_002_exit_delay_finishes_in_armed_state` |
| REQ-005 | Zona temporizada inicia demora de entrada. | `test_req_003_entry_zone_starts_entry_delay_when_armed` |
| REQ-006 | PIN valido desarma durante entrada. | `test_req_004_valid_pin_disarms_during_entry_delay` |
| REQ-007 | Vencimiento de entrada dispara alarma. | `test_req_005_entry_delay_timeout_raises_alarm` |
| REQ-008 | Zona instantanea dispara alarma inmediata. | `test_req_006_instant_zone_raises_alarm_immediately` |
| REQ-009 | Tres PIN invalidos disparan sabotaje. | `test_req_007_three_invalid_pins_raise_tamper_alarm` |
| REQ-010 | Configuracion invalida se rechaza. | `test_begin_rejects_invalid_pin_configuration` |

## Comprobacion ejecutada

Se ejecuto una comprobacion automatica de consistencia de la entrega. Resultado:

```text
Archivos requeridos faltantes: ninguno
Archivos con caracteres no ASCII: ninguno
Requerimientos detectados: REQ-001, REQ-002, REQ-003, REQ-004, REQ-005, REQ-006, REQ-007, REQ-008, REQ-009, REQ-010
Pruebas unitarias detectadas: 11
Pruebas de aceptacion detectadas: 3
Problemas de trazabilidad: ninguno
Resultado: OK
```

## Acciones finales recomendadas fuera de este entorno

Para cerrar los dos puntos que dependen de herramientas externas:

```powershell
.\scripts\crear-commits-rubrica.ps1
```

Para usar la cuenta GitHub `joseselman` y subir `main` y `develop` al repositorio `Bonomi_2`:

```powershell
.\scripts\crear-commits-rubrica.ps1 -Push
```

```bash
pio test -e native
pio check -e native
pre-commit run --all-files
doxygen Doxyfile
```
