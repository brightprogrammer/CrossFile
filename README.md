# CrossFile

[![](https://img.shields.io/badge/Discord-7289DA?style=for-the-badge&logo=discord&logoColor=white)](https://discord.gg/https://discord.gg/J9b45jbAdH)
<a href="https://www.buymeacoffee.com/brightprogrammer" target="_blank"><img src="https://cdn.buymeacoffee.com/buttons/default-orange.png" alt="Buy Me A Coffee" height="28" width="105"></a>

## Intro

I've always felt the need for one single C/C++ library to handle all the file formats
I need. The search has been quite long, and I've failed hard in finding an appropriate
library. For this, I'm now writing one myself. Whatever binary file format I need to
interact with, CrossFile must and will support it.

CrossFile is a cross-platform file I/O library that aims to provide support for
reading, modifying, and creating the supported file formats from scratch.

CrossFile aims :

- to support all the file formats I need (font, images, executables, archives,
  data, etc...)
- to provide a uniform cross-platform interface for interaction with these files
- to read/modify/edit/create the files.
- to be free forever and simple to use

## Build

To build and install CrossFile as a library on your system, follow the following steps :

- Create a build directory where you want to build the library. I usually name this as
  `Build` inside the project root itself.
- Go inside `Build` directory and open a terminal, or you can follow any other method
  to go to that directory inside the terminal.
- Run the following command :

   - If you have Ninja installed on your system, then use it, it's better :
     `cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release`
   - If you don't have Ninja installed, then we'll just use GNU Make :
     `cmake .. -DCMAKE_BUILD_TYPE=Release`

- The last step will generate build files needed for building the project, and now you can run :

   - `ninja` if you have Ninja installed
   - `make -j${nproc}` if you have make installed. You can run just `make` as well.
     The only difference is number of threads provided for building the project.

- The last step will build the project, and now you can run it (for now) using `bin/main`
