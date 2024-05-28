## Renai

Renai is a high fantasy adult game set in the mystical world of Silre, where 'magic' and 'metal' are synonymous. As demons begin to spill from a crack in the gates to hell, a goddess bestoys _you_ with the power to save humanity. Was her faith unfounded?

This project is under the [Mozilla General Public License v2.0](LICENSE.md).

---

### Installation

---

Renai can be run on x64 versions of Windows and most Linux distributions. It is programmed in C, and is installed via CMake. In order to install, the following steps are needed.

- 1. Download the source files. Git cloning should work fine, but so should downloading any of the releases directly from the repository.

- 2. Set up CMake. There are thousands of guides for this, follow any of them, and configure the project files to build into a "build" folder.

- 3. Run make, or whatever other build system you're making CMake use. On Windows, MinGW provides a make implementation. This will build the actual game files.

- 4. Run the app. The subfolder inside "build" should be named "Renai", which is a portable installation of the game, built for your specific machine and operating system. Move this to wherever you'd like.

The full command breakdown (and the one I use in my build.sh file) is as follows;

```bash
cmake -B Build .
cd Build
make
cd Renai
./Renai[.exe]
```