# GN-fw_core

Goldear Networks firmware core framework.

STM32CubeIDE をベースにした再利用可能な組み込みファームウェア基盤テンプレートです。

レイヤ分離（app / drv / plat / utils）により、移植性・保守性・再利用性を重視しています。

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

## Requirements
- STM32CubeIDE
- STM32CubeMX
- Git
- Windows PowerShell

## License
This project is licensed under the MIT License.