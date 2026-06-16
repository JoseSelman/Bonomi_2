# Formato y convenciones de codigo

## Herramienta de formato

El formato se define en `.clang-format`, basado en LLVM, con indentacion de 2 espacios y limite de 100 columnas.

## Reglas de nombres

| Elemento | Regla | Ejemplo |
| --- | --- | --- |
| Clases y estructuras | PascalCase | `AlarmController`, `AlarmConfig` |
| Enumeraciones | PascalCase | `AlarmState` |
| Valores de enumeracion | PascalCase | `EntryDelay` |
| Metodos y funciones | lowerCamelCase | `requestDisarm` |
| Variables locales | lowerCamelCase | `pinLength` |
| Miembros privados | lowerCamelCase con `_` final | `remainingMs_` |
| Constantes | `k` + PascalCase | `kMinPinLength` |
| Macros de guarda | MAYUSCULAS con `_` | `ALARM_CONTROLLER_H` |

## Reglas de estilo

- No mezclar logica de dominio con acceso a hardware.
- Evitar memoria dinamica en la libreria de dominio.
- Validar entradas publicas antes de cambiar el estado.
- Mantener una sola responsabilidad por metodo.
- Documentar la API publica con Doxygen.
- Usar `stdint.h` para tipos de ancho fijo en codigo embebido.

## Aplicacion automatica

Visual Studio Code queda configurado para aplicar formato al guardar mediante `.vscode/settings.json`. El hook `clang-format` de `.pre-commit-config.yaml` permite aplicar la misma regla antes de confirmar cambios.
