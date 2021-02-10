## 9it - Version Control System

Totally not git! 9it is actually what we like to call a 'single-player' version control system. It has most of the basic functionality of git minus pushing and pulling. Thus, it is lightweight and intended for individual use. Keep in mind that this project was developed as a learning experience and not intended for everyday use, thus bugs and potential data loss are a possibility.

### USAGE

Please read the man page that comes installed or pass in the help flag for usage.

### FEATURES / NON-FEATURES

Basic staging, committing, reverting and branching is implemented. There are also a couple of commands for inspecting the raw binary objects that 9it produces.

Pushing and fetching from remotes is not built in, merging has also not been implemented.

### INSTALLATION

#### pkgbuild (arch, artix, manjaro etc)

Not in AUR yet (and not sure if it will ever be), but you can get the pkgbuild from [here](https://files.danieliu.xyz/repos/makepkg/9it-cli/PKGBUILD). The entire install process is as follows:
```
mkdir 9it-cli
cd 9it-cli
wget https://files.danieliu.xyz/repos/makepkg/9it-cli/PKGBUILD
makepkg -si
```

#### deb (deb, ubuntu, mint etc)

The .deb file is available [here](https://files.danieliu.xyz/repos/apt/9it-cli/9it-cli_1.0-1.deb). The full install is quite simple:
```
wget https://files.danieliu.xyz/repos/apt/9it-cli/9it-cli_1.0-1.deb
sudo dpkg -i 9it-cli_1.0-1.deb
sudo apt-get install -f
```

#### ebuild (gentoo) - coming soon

#### Build from Source
Simply clone the repository:
```
git clone https://github.com/NaiveGit/9it-cli
```
And install using make:
```
make && sudo make clean install
```
The binary is installed to /usr/bin in addition to man pages at /usr/share/man.

