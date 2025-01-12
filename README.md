# Eye Saver

Blacks your screen out to remind you to take breaks and rest your eyes

# Installation Instructions

Download and install [GLFW](https://www.glfw.org/download) either from their
website or from your package manager. This project uses the version
corresponding to OpenGL version 3.3.

Download [GLAD](https://glad.dav1d.de/). Place `glad.c` into the lib directory
and `glad.h` into `inc/glad`

Install FreeType (I just used apt) and copy the file `ft2build.h` (mine was at
`/usr/include/freetype2/ft2build.h`) into the root of the `inc` directory.

run `make` in the main directory of the project and an executable should be
generated in the bin directory.
