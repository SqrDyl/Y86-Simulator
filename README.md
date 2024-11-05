# Y86-Simulator

<p>This simulator can read Y86 input (ending in .ys) and assemble it (ending in .yo) into executable code.</p>

## Dependencies

<p>Since this project was originally made on a linux machine, it's best to try to run this on linux with make installed. However, we can use WSL to help us run this on a windows.</p>

### Install WSL (Recommended)
```
wsl --install
```

### Install Make Through WSL

```
sudo apt update
sudo apt install make gcc g++ build-essential
```

### Install Chocolatey (Alternative)

<p>If you have any issues, you can install make alternatively like this. This is only if you couldn't get make to install. First open powershell as admin.</p> 

```
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
```

### Install Make (Alternative)

```
choco install make -y
```
<p>Check if installation was succesful.

```
make --version
```

## How to Run

<p>How to clean</p>

```
make clean
```

<p>Check if the simulator is working. Everything should pass after this command.</p>

```
make yess
```

