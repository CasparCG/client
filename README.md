# CasparCG Client

![platforms](https://img.shields.io/badge/platforms-windows%20%7C%20linux%20%7C%20osx-brightgreen.svg?style=flat-square)
[![help](https://img.shields.io/badge/help-community%20forum-green.svg?style=flat-square)](https://casparcg.com/forum)
[![license](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat-square)](LICENSE)

<p align="center"><img src="/src/Widgets/Images/Clients.png"></p>

## Installation

#### Windows
No installation required. Unpack the zip file to a location on your drive and start it. Tested on Windows 10 x64.

#### macOS
No installation required. Open the DMG file and drag the 'CasparCG Client' app to a location on your drive and start it. Tested on macOS 11.

#### Linux
Install the deb file and launch 'CasparCG Client'. Tested on Ubuntu 22.04 64-bit, other distributions may require building from source.

## Development

#### Windows
* Install Qt 6.5 for Windows from [Qt archive](https://www.qt.io/download). You may wish to select a more minimal installation than the full 6.5 tree.
* Install [Visual Studio Community 2022](https://visualstudio.microsoft.com/vs/community/)
* Install [CMake](https://cmake.org/download/)

* Run cmake with the argument `Qt6_ROOT` with a value of `c:\Qt\6.5.3\msvc2019_64` pointing to your qt installation.
* Open the Visual Studio project file

#### macOS
* Install Qt 6.5 for macOS from [Qt archive](https://www.qt.io/download). You may wish to select a more minimal installation than the full 6.5 tree.
* Download and install Xcode from the App Store.

#### Linux
* Install Qt6, libvlc and boost from your system package manager

## Releases
Complete history of all releases and the changes can be found in the [CHANGELOG](CHANGELOG).

## License
CasparCG Client is distributed under the GNU General Public License GPLv3 or higher, see [LICENSE](LICENSE) for details.
