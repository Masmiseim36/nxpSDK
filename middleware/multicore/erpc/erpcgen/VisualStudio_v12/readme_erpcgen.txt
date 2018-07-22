Building erpcgen for Windows using Visual Studio
================================================


Requirements
------------

1. Visual Studio Express 2013 or later.

    This version is required for the C++11 support.

2. Win flex-bison

    Direct link to the latests win flex-bison release download:
        [http://sourceforge.net/p/winflexbison]

    Extract win flex-bison zip contents directly into the erpc/erpcgen/VisualStudio_v12 directory
    (not into a subdirectory).

3. boost [http://boost.org]

    Direct link to the boost 1.61 release downloads:
        [https://sourceforge.net/projects/boost/files/boost-binaries/1.61.0/]

    Install binary for your version of visual studio (x86 architecture)
    into current directory (for Visual Studio 2013: boost_1_61_0-msvc-12.0-32.exe):
        erpc\erpcgen\VisualStudio_v12\

        whole path has to be:
        erpc\erpcgen\VisualStudio_v12\boost_1_61_0\

    For different version of Visual Studio boost library you need update project:
        in Properties\Linker\General update Additional Library Directories

Build output
------------

Visual Studio build output is available in these directories:

    erpc/erpcgen/VisualStudio_v12/Debug
    erpc/erpcgen/VisualStudio_v12/Release

