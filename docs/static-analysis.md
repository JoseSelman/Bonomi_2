# Verificacion estatica

## Compilador

`platformio.ini` configura advertencias estrictas:

```ini
-Wall
-Wextra
-Wpedantic
-Wconversion
```

El criterio de aceptacion es compilar sin advertencias. Estas banderas ayudan a detectar conversiones implicitas, codigo no portable y errores frecuentes antes de ejecutar el programa.

## Cppcheck

La configuracion de PlatformIO define `cppcheck` como herramienta de analisis:

```bash
pio check -e native
```

El hook de pre-commit ejecuta:

```bash
cppcheck --enable=warning,style,performance,portability --std=c++11 --inline-suppr --error-exitcode=1 lib src test
```

## Politica de tratamiento de hallazgos

- Hallazgos de severidad `error` o `warning`: se corrigen antes de entregar.
- Hallazgos de estilo: se corrigen salvo justificacion documentada.
- Falsos positivos: se documentan con motivo y, si hace falta, se suprimen de forma localizada.

## Justificacion actual

La libreria evita memoria dinamica y no depende de APIs de hardware. El uso de `static_cast<unsigned char>` antes de `isdigit` previene comportamiento indefinido con caracteres con signo. El contador de intentos invalidos satura en `maxInvalidAttempts` para evitar desbordes.
