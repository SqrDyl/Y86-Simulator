# Y86-Simulator

<p>This simulator can read Y86 input (ending in .ys) and assemble it (ending in .yo) into executable code.</p>

## Dependencies

<p>First Install make through a package manager, here's how to install a package manager using Windows.</p>

### Install Chocolatey

<p>Open PowerShell as administrator.</p> 

```
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

### Install make

```
choco install make -y
```
<p>Check if installation was succesful.

```
make --version
```

## How to Run

