param(
  [string]$UserName = "joseselman",
  [string]$UserEmail = "joseselman@users.noreply.github.com",
  [string]$GitHubUser = "joseselman",
  [string]$RepositoryName = "Bonomi_2",
  [string]$RemoteName = "origin",
  [switch]$SkipMerge,
  [switch]$Push,
  [switch]$UseSshRemote
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"
if (Get-Variable -Name PSNativeCommandUseErrorActionPreference -ErrorAction SilentlyContinue) {
  $PSNativeCommandUseErrorActionPreference = $false
}

function Invoke-Git {
  param(
    [Parameter(Mandatory = $true)]
    [string[]]$Arguments
  )

  & git @Arguments
  if ($LASTEXITCODE -ne 0) {
    throw "Fallo el comando: git $($Arguments -join ' ')"
  }
}

function Get-GitExitCode {
  param(
    [Parameter(Mandatory = $true)]
    [string[]]$Arguments
  )

  try {
    & git @Arguments *> $null
    return $LASTEXITCODE
  } catch {
    return 1
  }
}

function Test-GitCommand {
  & git --version | Out-Null
  if ($LASTEXITCODE -ne 0) {
    throw "Git no esta disponible en la terminal."
  }
}

function Test-RepositoryHasCommits {
  return (Get-GitExitCode @("rev-parse", "--verify", "HEAD")) -eq 0
}

function Ensure-GitIdentity {
  $currentName = & git config --get user.name
  if ([string]::IsNullOrWhiteSpace($currentName)) {
    Invoke-Git @("config", "user.name", $UserName)
  }

  $currentEmail = & git config --get user.email
  if ([string]::IsNullOrWhiteSpace($currentEmail)) {
    Invoke-Git @("config", "user.email", $UserEmail)
  }
}

function Commit-Group {
  param(
    [Parameter(Mandatory = $true)]
    [string]$Message,

    [Parameter(Mandatory = $true)]
    [string[]]$Paths
  )

  [void](Get-GitExitCode @("reset", "--quiet"))

  $existingPaths = @()
  foreach ($path in $Paths) {
    if (Test-Path -Path $path) {
      $existingPaths += $path
    } else {
      Write-Warning "No existe y se omite: $path"
    }
  }

  if ($existingPaths.Count -eq 0) {
    Write-Warning "No hay archivos para el commit: $Message"
    return
  }

  & git add -- $existingPaths
  if ($LASTEXITCODE -ne 0) {
    throw "No se pudieron agregar archivos para: $Message"
  }

  $diffExitCode = Get-GitExitCode @("diff", "--cached", "--quiet")
  if ($diffExitCode -eq 0) {
    Write-Host "Sin cambios para commit: $Message"
    return
  }

  Invoke-Git @("commit", "-m", $Message)
}

$scriptDirectory = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Resolve-Path (Join-Path $scriptDirectory "..")
Set-Location $repoRoot

Test-GitCommand

if (-not (Test-Path -Path ".git")) {
  & git init -b main
  if ($LASTEXITCODE -ne 0) {
    Invoke-Git @("init")
    Invoke-Git @("checkout", "-B", "main")
  }
}

if (Test-RepositoryHasCommits) {
  throw "El repositorio ya tiene commits. Este script esta pensado para ejecutarse una sola vez sobre una entrega sin historial."
}

Ensure-GitIdentity

Commit-Group `
  -Message "chore: create PlatformIO project structure" `
  -Paths @(
    "README.md",
    "ENTREGA.md",
    "platformio.ini",
    ".gitignore"
  )

Invoke-Git @("checkout", "-b", "develop")

Commit-Group `
  -Message "feat: implement alarm controller state machine" `
  -Paths @(
    "lib/AlarmController/library.json",
    "lib/AlarmController/src/AlarmController.h",
    "lib/AlarmController/src/AlarmController.cpp",
    "src/main.cpp"
  )

Commit-Group `
  -Message "test: add unit and acceptance tests" `
  -Paths @(
    "test/test_alarm_controller/test_alarm_controller.cpp",
    "test/test_acceptance/test_acceptance.cpp"
  )

Commit-Group `
  -Message "docs: add verification evidence and project documentation" `
  -Paths @(
    ".clang-format",
    ".pre-commit-config.yaml",
    ".vscode/settings.json",
    "Doxyfile",
    "LICENSE",
    "RUBRICA_RESUELTA.md",
    "docs",
    "scripts/crear-commits-rubrica.ps1"
  )

if (-not $SkipMerge) {
  Invoke-Git @("checkout", "main")
  Invoke-Git @("merge", "--no-ff", "develop", "-m", "merge: integrate development branch")
}

if ($Push) {
  if ($UseSshRemote) {
    $remoteUrl = "git@github.com:$GitHubUser/$RepositoryName.git"
  } else {
    $remoteUrl = "https://github.com/$GitHubUser/$RepositoryName.git"
  }

  if ((Get-GitExitCode @("remote", "get-url", $RemoteName)) -eq 0) {
    Invoke-Git @("remote", "set-url", $RemoteName, $remoteUrl)
  } else {
    Invoke-Git @("remote", "add", $RemoteName, $remoteUrl)
  }

  Invoke-Git @("push", "-u", $RemoteName, "main")
  Invoke-Git @("push", "-u", $RemoteName, "develop")
}

Write-Host ""
Write-Host "Commits generados correctamente."
if ($Push) {
  Write-Host "Repositorio remoto: $remoteUrl"
}
Write-Host "Historial:"
Invoke-Git @("log", "--oneline", "--decorate", "--graph", "--all")
