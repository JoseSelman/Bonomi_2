# Checklist de rubrica resuelta

| Criterio | Puntaje | Nivel maximo de la rubrica | Evidencia incluida |
| --- | ---: | --- | --- |
| Repositorio Git | 0,5 | Rama de desarrollo con commits descriptivos para cada avance. | `.gitignore`, `docs/git-workflow.md`, `docs/execution-report.md`. |
| Organizacion PlatformIO | 0,5 | Librerias en `lib`, programa en `src`, pruebas en `test`. | `platformio.ini`, `lib/AlarmController`, `src/main.cpp`, `test/`. |
| Formato del codigo | 1,2 | Reglas respetadas y reglas de nombrado. | `.clang-format`, `docs/coding-standard.md`. |
| Verificacion automatica de formato | 1,2 | `clang-format` al guardar y pre-commit. | `.vscode/settings.json`, `.pre-commit-config.yaml`. |
| Verificacion estatica de codigo | 1,2 | Compilador con advertencias, `cppcheck`, justificacion de hallazgos. | `platformio.ini`, `.pre-commit-config.yaml`, `docs/static-analysis.md`. |
| Revision de codigo por IA | 1,2 | Revision completa, sugerencias evaluadas y acciones documentadas. | `docs/ai-code-review.md`. |
| Pruebas unitarias | 1,2 | Disciplina TDD y pruebas trazadas a requerimientos. | `test/test_alarm_controller/test_alarm_controller.cpp`, `docs/requirements.md`. |
| Pruebas de aceptacion | 1,2 | Flujos principales, alternativos y de excepcion. | `test/test_acceptance/test_acceptance.cpp`, `docs/acceptance-tests.md`. |
| Documentacion del codigo | 1,2 | API completa en Doxygen y configuracion Doxygen. | `lib/AlarmController/src/AlarmController.h`, `Doxyfile`. |
| Licencia | 0,6 | Licencia para codigo propio y archivo `LICENSE`. | `LICENSE`, avisos SPDX en codigo fuente. |

## Observacion

La resolucion extendida esta en `RUBRICA_RESUELTA.md`. El unico punto no materializado por el entorno es la creacion real de `.git`; se intento con permisos elevados y quedo documentado en `docs/execution-report.md`.
