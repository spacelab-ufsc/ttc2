
<h1 align="center">
    <br>
    TTC 2.0 FIRMWARE
    <br>
</h1>

<h4 align="center">Telemetry, Tracking and Command module firmware project (sources, configs and documentation).</h4>

<p align="center">
    <a href="https://github.com/spacelab-ufsc/spacelab#versioning">
        <img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/ttc2/releases">
        <img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/ttc2?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/ttc2/blob/dev_firmware/firmware/version.h">
        <img alt="GitHub commits since latest release (by date) for a branch" src="https://img.shields.io/github/commits-since/spacelab-ufsc/ttc2/latest/dev_firmware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/ttc2/commits/master">
        <img alt="GitHub last commit (branch)" src="https://img.shields.io/github/last-commit/spacelab-ufsc/ttc2/dev_firmware?style=for-the-badge">
    </a>
    <a href="">
        <img alt="GitHub issues by-label" src="https://img.shields.io/github/issues/spacelab-ufsc/ttc2/firmware?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/ttc2/blob/master/firmware/main.c">
        <img src="https://img.shields.io/badge/language-C-green?style=for-the-badge">
    </a>
    <a href="">
        <img src="https://img.shields.io/badge/IDE%20tool-CCS%20v9.0-yellow?style=for-the-badge">
    </a>
    <a href="#license">
        <img src="https://img.shields.io/badge/license-GPL3-yellow?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#overview">Overview</a> •
    <a href="#dependencies">Dependencies</a> •
    <a href="#development">Development</a> •
    <a href="#version">Version</a> •
    <a href="#license">License</a>
</p>

<br>

## Overview

The TTC 2.0 board has two MSP430 low-power microcontrollers that runs the firmware application. It manages the others sub-modules within the board, synchronizes actions, handles communication with the Earth's segment, and manages the data flow. The programming language used is C and firmware was developed in the Code Composer Studio IDE (a.k.a. CCS). 

## Dependencies

* MSP430 DriverLib v2.91.11.01

## Development

#### Toolchain setup

The Code Composer Studio (CCS) v9.0, or later, can be downloaded [here](http://software-dl.ti.com/ccs/esd/documents/ccs_downloads.html). After downloaded, start the install process. For the TTC 2.0, only the *MSP430 Ultra Low Power MCUs* package is required. More instructions for Linux environment, click [here](https://software-dl.ti.com/ccs/esd/documents/ccs_linux_host_support.html).

#### Compiling and building
	
Clone [this repository](https://github.com/spacelab-ufsc/ttc2.git). After this, import the source project into CCS:
`File -> Import -> Code Composer Studio -> CCS Projects -> Select the source folder: ttc/firmware -> Leave 'Copy projects into workspace' unchecked / Finish`. Then, click in the Build button or press "Ctrl+B" to compile and build the sources. 

> Alternatively, it is possible to use the available makefile to import, compile and build the firmware in the linux environment. It is necessary to update the directories to correspond to those in the running machine, hit "make import" in the linux terminal and hit "make".

#### Debugging

Before deploying the program into the microcontroller using the JTAG interface, follow the [power-on procedures](https://github.com/spacelab-ufsc/ttc2/tree/dev_hardware/hardware#power-on-procedure). Using the [MSP-FET](http://www.ti.com/tool/MSP-FET) is possible to program the microcontroller and start a debug session, which allows full control of the firmware in execution. For that, with the project opened in the CCS, click in the "Debug" button or press "F11" to upload the code into the TTC 2.0 board.

Besides the JTAG, there is the debugging performed through a serial UART port, using as default a baud rate of 115200, 1 stop bit, and no parity bit. The interface uses a log system standard to improve readability. Refer to the [documentation](https://github.com/spacelab-ufsc/ttc2/tree/master/doc) in the "Instructions" chapter.

> Alternatively, it is possible to use the available makefile to perform the deployment of the firmware image into the microcontroller in the linux environment using the MSP-FET. It is necessary to follow the power-on procedures, connect the programmer and hit "make flash" in the terminal.

## Version

Refer to the [releases](https://github.com/spacelab-ufsc/ttc2/releases) page and the [version](https://github.com/spacelab-ufsc/ttc2/blob/master/firmware/version.h) file.

## License

The firmware of this project is licensed under GPLv3 license.
