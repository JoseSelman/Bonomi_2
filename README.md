# Actividad integradora - Verificacion y Validacion de Software

Proyecto: controlador de alarma embebido con zona temporizada.

Este repositorio resuelve la actividad integradora mediante un proyecto PlatformIO escrito en C++11. La solucion separa la logica de negocio en una libreria propia, incluye pruebas unitarias y de aceptacion, documentacion de requerimientos, trazabilidad, revision por IA, reglas de formato, configuracion de analisis estatico, Doxygen y licencia.

## Alcance

El sistema modela una alarma domiciliaria simple:

- El usuario legitimo puede armar y desarmar la alarma con PIN.
- Al armar, existe una demora de salida para abandonar la zona protegida.
- Una zona de entrada temporizada permite ingresar y desarmar antes de disparar la sirena.
- Una zona instantanea dispara la alarma sin demora.
- Tres intentos fallidos de PIN disparan una alarma de sabotaje.

La logica se implementa sin depender de hardware para poder probarla en el entorno `native` de PlatformIO. El archivo `src/main.cpp` muestra una integracion minima para firmware.

## Estructura

```text
.
|-- lib/AlarmController/        # Libreria de dominio
|-- src/                        # Programa principal
|-- test/                       # Pruebas unitarias y de aceptacion
|-- docs/                       # Requerimientos, UML, trazabilidad y revision
|-- platformio.ini              # Configuracion PlatformIO
|-- .clang-format               # Formato de codigo
|-- .pre-commit-config.yaml     # Hooks de formato y analisis estatico
|-- Doxyfile                    # Documentacion de API
|-- LICENSE                     # Licencia del codigo propio
```

## Comandos sugeridos

Para generar automaticamente los commits de la rubrica:

```powershell
.\scripts\crear-commits-rubrica.ps1
```

Para generar los commits y subirlos a la cuenta GitHub `joseselman`:

```powershell
.\scripts\crear-commits-rubrica.ps1 -Push
```

Si Windows bloquea scripts de PowerShell:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\crear-commits-rubrica.ps1 -Push
```

Para verificar el proyecto:

```bash
pio test -e native
pio check -e native
pre-commit run --all-files
doxygen Doxyfile
```

## Evidencia para la rubrica

- Repositorio Git: ver `docs/git-workflow.md`.
- Rubrica resuelta: `RUBRICA_RESUELTA.md` y `docs/rubric-checklist.md`.
- Organizacion PlatformIO: `lib`, `src` y `test`.
- Formato de codigo: `.clang-format` y `docs/coding-standard.md`.
- Verificacion automatica de formato: `.vscode/settings.json` y `.pre-commit-config.yaml`.
- Verificacion estatica: `platformio.ini`, `.pre-commit-config.yaml` y `docs/static-analysis.md`.
- Revision por IA: `docs/ai-code-review.md`.
- Pruebas unitarias: `test/test_alarm_controller/test_alarm_controller.cpp`.
- Pruebas de aceptacion: `test/test_acceptance/test_acceptance.cpp` y `docs/acceptance-tests.md`.
- Documentacion del codigo: comentarios Doxygen y `Doxyfile`.
- Licencia: `LICENSE` y avisos de copyright.
- Informe de ejecucion local: `docs/execution-report.md`.
