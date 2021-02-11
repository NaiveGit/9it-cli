## 9it - Version Control System

Totally not git! 9it is actually what we like to call a 'single-player' version control system. It has most of the basic functionality of git minus pushing and pulling. Thus, it is lightweight and intended for individual use. Keep in mind that this project was developed as a learning experience and not intended for everyday use, thus bugs and potential data loss are a possibility.

### USAGE

Please read the man page that comes installed or pass in the help flag for usage.

### FEATURES / NON-FEATURES

Basic staging, committing, reverting and branching is implemented. There are also a couple of commands for inspecting the raw binary objects that 9it produces.

Pushing and fetching from remotes is not built in, merging has also not been implemented.

### INSTALLATION

#### arch (and artix, manjaro etc)

Not in AUR yet (and not sure if it will ever be), but you can get the pkgbuild from [here](https://files.danieliu.xyz/repos/arch/9it-cli/PKGBUILD). The entire install process is as follows:
```
mkdir 9it-cli
cd 9it-cli
wget https://files.danieliu.xyz/repos/arch/9it-cli/PKGBUILD
makepkg -si
```

#### debian (and ubuntu, mint etc)

The .deb file is available [here](https://files.danieliu.xyz/repos/debian/9it-cli/9it-cli_1.0-1.deb). The full install is quite simple:
```
wget https://files.danieliu.xyz/repos/debian/9it-cli/9it-cli_1.0-1.deb
sudo dpkg -i 9it-cli_1.0-1.deb
sudo apt-get install -f
```

#### gentoo

The ebuild can be found [here](https://files.danieliu.xyz/repos/gentoo/9it-cli/9it-cli-1.0.ebuild). Currently there are no USE flags (and there may never be). To install external ebuilds on gentoo, you need your own local repository. If you don't have one watch [this](https://www.youtube.com/watch?v=wVQIhZPMDlU) or read [this](https://wiki.gentoo.org/wiki/Custom_ebuild_repository). Once you are ready, download the ebuild and place it in the dev-vcs category in the local repo. In the root of your local repo, run:
```
sudo mkdir -p dev-vcs/9it-cli/
sudo wget -P dev-vcs/9it-cli/ https://files.danieliu.xyz/repos/gentoo/9it-cli/9it-cli-1.0.ebuild
sudo chown -R portage:portage dev-vcs
cd dev-vcs/9it-cli
sudo repoman manifest
```
Now we can emerge it:
```
sudo emerge --ask dev-vcs/9it-cli::local-repo-name
```

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

