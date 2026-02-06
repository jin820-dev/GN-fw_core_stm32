# GN-fw_core_stm32

Goldear Networks firmware core framework.

A reusable embedded firmware framework template built on STM32CubeIDE.

The project follows a clean layered architecture (app / drv / plat / utils) designed for portability, maintainability, and long-term reusability.

Note:
The architecture is hardware-agnostic.  
Only the plat layer depends on the MCU/board, so it can be ported to other STM32 or MCUs easily.

## Features
- Layered architecture
- STM32CubeIDE compatible
- HAL / FatFs ready
- third_party auto setup script
- MIT License

## Directory
Core/ : CubeMX generated core  
Drivers/ : HAL / CMSIS  
Middlewares/ : FatFs  
User/ : Application source  
User/_scripts : setup scripts  

## Getting Started (Windows)
### Clone
git clone <repo-url>  
cd GN-fw_core  

### Setup third_party
PowerShell:
```
> powershell -ExecutionPolicy Bypass -File User/_scripts/setup_third_party_ssd1306.ps1
```
### Build
STM32CubeIDE で `.project` を Import → Build

## Target Board (reference)
This repository currently includes a ready-to-build configuration for:

- STM32H723ZG Nucleo (STM32H7 series)
- STM32CubeIDE project
- HAL + FatFs enabled

## Requirements
- STM32CubeIDE
- STM32CubeMX
- Git
- Windows PowerShell

## License
This project is licensed under the MIT License.