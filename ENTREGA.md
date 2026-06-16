# Resolucion de la actividad integradora

## Proyecto elegido

Se desarrollo un controlador de alarma embebido con zona temporizada. El dominio fue elegido porque permite aplicar verificacion y validacion sobre una maquina de estados pequena, realista y totalmente testeable sin depender de hardware.

## Que se entrega

- Proyecto PlatformIO con estructura `src`, `lib` y `test`.
- Libreria `AlarmController` con la logica de negocio aislada.
- Programa principal minimo para integrar la libreria.
- Pruebas unitarias trazadas a requerimientos.
- Pruebas de aceptacion para flujo principal, alternativo y de excepcion.
- Documentacion de requerimientos, UML, formato, analisis estatico, revision por IA y flujo Git.
- Configuracion de clang-format, cppcheck, pre-commit y Doxygen.
- Licencia MIT y avisos SPDX en codigo fuente.

## Requerimientos principales cubiertos

1. El sistema ignora sensores cuando esta desarmado.
2. El usuario arma con PIN valido y entra en demora de salida.
3. Durante la demora de salida, los sensores no disparan alarma.
4. Al finalizar la demora, el sistema queda armado.
5. La zona temporizada inicia demora de entrada.
6. El PIN valido durante entrada desarma el sistema.
7. El vencimiento de entrada dispara alarma.
8. La zona instantanea dispara alarma inmediata.
9. Tres PIN invalidos disparan alarma de sabotaje.
10. Una configuracion invalida se rechaza al iniciar.

## Evidencia principal

| Rubrica | Archivo |
| --- | --- |
| Organizacion PlatformIO | `platformio.ini`, `src`, `lib`, `test` |
| Requerimientos y trazabilidad | `docs/requirements.md` |
| Rubrica resuelta | `RUBRICA_RESUELTA.md`, `docs/rubric-checklist.md` |
| Pruebas unitarias | `test/test_alarm_controller/test_alarm_controller.cpp` |
| Pruebas de aceptacion | `test/test_acceptance/test_acceptance.cpp`, `docs/acceptance-tests.md` |
| Diagrama UML | `docs/uml.md` |
| Formato de codigo | `.clang-format`, `docs/coding-standard.md` |
| Verificacion automatica | `.vscode/settings.json`, `.pre-commit-config.yaml` |
| Analisis estatico | `platformio.ini`, `docs/static-analysis.md` |
| Revision por IA | `docs/ai-code-review.md` |
| Documentacion API | `Doxyfile`, comentarios Doxygen |
| Licencia | `LICENSE` |
| Informe de ejecucion | `docs/execution-report.md` |

## Comandos de verificacion

```bash
pio test -e native
pio check -e native
pre-commit run --all-files
doxygen Doxyfile
```

## Nota para la entrega final

Para obtener el puntaje completo de repositorio Git, confirmar los cambios en una rama `develop` con mensajes descriptivos. La secuencia sugerida esta en `docs/git-workflow.md`.
