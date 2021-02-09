## 9it

Totally not git! 9it is actually what we like to call a 'single-player' version control system. It has most of the basic functionality of git minus pushing and pulling. Thus, it is lightweight and intended for individual use. Keep in mind that this project was developed as a learning experience and not intended for everyday use, thus bugs and potential data loss are a possibility.

### USAGE
Please read the man page that comes installed or pass in the help flag for usage.

### FEATURES / NON-FEATURES

Basic staging, committing, reverting and branching is implemented. There are also a couple of commands for inspecting the raw binary objects that 9it produces.

Pushing and fetching from remotes is not built in, merging has also not been implemented.

### INSTALLATION

#### AUR (coming soon)

#### apt (coming soon)

#### ebuild (coming soon)

#### Build from Source
Simply clone the repository:
```
git clone https://github.com/NaiveGit/9it-cli
```
And install using make:
```
make && sudo make clean install
```
The binary is installed to /usr/local in addition to man pages at /usr/local/man.

