# Flujo Git recomendado

La rubrica evalua el uso de repositorio, rama de desarrollo y commits descriptivos. Para presentar esta actividad, se recomienda usar:

- Rama principal: `main`.
- Rama de trabajo: `develop`.
- Commits pequenos y trazables.

## Secuencia de commits sugerida

La forma automatica recomendada es ejecutar el script incluido:

```powershell
.\scripts\crear-commits-rubrica.ps1
```

El script ya queda preparado con identidad local `joseselman` y correo `joseselman@users.noreply.github.com`.

Para crear los commits y empujar a `https://github.com/joseselman/Bonomi_2.git`:

```powershell
.\scripts\crear-commits-rubrica.ps1 -Push
```

Antes de usar `-Push`, el repositorio `Bonomi_2` debe existir en GitHub bajo la cuenta `joseselman`, o se debe indicar otro nombre:

```powershell
.\scripts\crear-commits-rubrica.ps1 -Push -RepositoryName "nombre-del-repositorio"
```

Si PowerShell bloquea la ejecucion de scripts, usar:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\scripts\crear-commits-rubrica.ps1 -Push
```

Si se desea configurar otra identidad local de Git para esta entrega:

```powershell
.\scripts\crear-commits-rubrica.ps1 -UserName "Nombre Apellido" -UserEmail "correo@example.com"
```

El script inicializa Git si hace falta, crea la rama `develop`, registra commits por avance, integra `develop` en `main` y, con `-Push`, configura `origin` y empuja `main` y `develop`.

La secuencia manual equivalente es:

```bash
git init -b main
git add README.md ENTREGA.md platformio.ini .gitignore
git commit -m "chore: create PlatformIO project structure"

git checkout -b develop

git add lib src
git commit -m "feat: implement alarm controller state machine"

git add test
git commit -m "test: add unit and acceptance tests"

git add docs scripts Doxyfile .clang-format .pre-commit-config.yaml .vscode/settings.json LICENSE RUBRICA_RESUELTA.md
git commit -m "docs: add verification evidence and project documentation"

git checkout main
git merge --no-ff develop
```

## Criterio de calidad de mensajes

Cada mensaje debe indicar que cambio se hizo y por que tipo de evidencia aporta:

- `feat`: funcionalidad nueva.
- `test`: pruebas nuevas o corregidas.
- `docs`: documentacion, trazabilidad o evidencia.
- `chore`: configuracion del proyecto.
- `fix`: correccion de defecto.

## Recomendacion para la entrega

Antes de entregar, ejecutar:

```bash
pre-commit run --all-files
pio test -e native
pio check -e native
```
