# CasparCG Client
> Client software primarily used with the CasparCG Server software for audio and video playout, to control graphics and recording, but it can also be used for other tasks within television broadcast.

![stable](https://img.shields.io/badge/stable-v2.0.8-lightgrey.svg?style=flat-square)
![platforms](https://img.shields.io/badge/platforms-windows%20%7C%20linux%20%7C%20osx-brightgreen.svg?style=flat-square)
[![help](https://img.shields.io/badge/help-community%20forum-green.svg?style=flat-square)](https://casparcg.com/forum)
[![irc](https://img.shields.io/badge/irc-%23casparcg-green.svg?style=flat-square)](https://kiwiirc.com/client/sinisalo.freenode.net/?nick=Guest|?#CasparCG)
[![license](https://img.shields.io/badge/license-GPLv3-blue.svg?style=flat-square)](LICENSE)

If used with CasparCG Server as intended, CasparCG Client provides a seamless and smooth way to playout audio, video and dynamic graphics. CasparCG Client can also be used to control ATEM Live Production Switchers and some TriCasters, without any requirements to use the CasparCG Server, and so much more.

<p align="center"><img src="/src/Widgets/Images/Clients.png"></p>


## Installation

#### Windows
> No installation required. Move the program to a appropriate location on your drive and start it.
> Tested on Windows 7 x64 and Windows 10 x64.

#### OS X
> Open the DMG file and drag the 'CasparCG Client' app to your /Applications to install.
> Tested on OS X 10.10 and higher.

#### Linux
> Unpack the tar file and run the run.sh. You have to pre-install *VLC* to be able to run the Linux version.
> Tested on ubuntu 14.04 LTS and 16.04 LTS.


## Branches
For the moment the project has two primary branches, `master` and `next`.  

The purpose of the `master` branch is to always have the latest stable version of CasparCG Client software, just as expected. The `next` branch contains the source code for the next upcoming version of CasparCG Client software. The version found in the next branch is considered to be unstable.  

*In the future, stable releases will be tagged for better overview and tracking.*


## Development

#### Windows
Install the pre-built Qt 5.* binaries for Windows 64-bit (MinGW SEH exception handling),
grab it from: [http://casparcg.com/build-tools/](http://casparcg.com/build-tools/).

#### Linux
VLC need to be installed on Linux: `sudo apt-get install vlc libvlc-dev libvlccore-dev`.  
The plugin cache may need to be refreshed: `sudo /usr/lib/vlc/vlc-cache-gen -f /usr/lib/vlc/plugins`.

*You can find the latest unstable development builds on [http://builds.casparcg.com](http://builds.casparcg.com).*


## Releases
Complete history of all releases and the changes can be found in the [CHANGELOG](CHANGELOG).


## License
CasparCG Client is distributed under the GNU General Public License GPLv3 or higher, see [LICENSE](LICENSE) for details.
