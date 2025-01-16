# littlefs_shell

## Overview
The littlefs_shell demonstrates the capabilities of LittleFS FLASH filesystem:

After the example is loaded into the target and launched the shell prompt is printed to the console.
Type "help" to obtain list of available commands. The FLASH storage has to be formatted upon first use.
After mounting the filesystem it is possible to create/delete/list directories and read/write files using appropriate commands.
There is no concept of current directory in LittleFS, hence it is always necessary to specify full directory path.

List of supported commands:
  format       Formats the filesystem
  mount        Mounts the filesystem
  unmount      Unmounts the filesystem
  ls           Lists directory content
  rm           Removes file or directory
  mkdir        Creates a new directory
  write        Writes/appends text to a file
  cat          Prints file content

Example workflow:
To perform initial format of the storage, issue 'format yes' command.
Mount the storage by issuing 'mount' command.
Create new directory by 'mkdir mynewdir'.
Create new file in the 'mynewdir' directory by writing line of text to it using 'write mynewdir/foo.txt firstline'
Append another line to the same file using 'write mynewdir/foo.txt secondline'
Print the content of the file using 'cat mynewdir/foo.txt'. The expected output is:
  firstline
  secondline

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1060-EVKB](../../_boards/evkbmimxrt1060/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1060-EVKC](../../_boards/evkcmimxrt1060/littlefs_examples/littlefs_shell/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1024-EVK](../../_boards/evkmimxrt1024/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1040-EVK](../../_boards/evkmimxrt1040/littlefs_examples/littlefs_shell/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/littlefs_examples/littlefs_shell/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/littlefs_examples/littlefs_shell/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/littlefs_examples/littlefs_shell/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/littlefs_examples/littlefs_shell/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [LPCXpresso54S018](../../_boards/lpcxpresso54s018/littlefs_examples/littlefs_shell/example_board_readme.md)
- [LPCXpresso54S018M](../../_boards/lpcxpresso54s018m/littlefs_examples/littlefs_shell/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/littlefs_examples/littlefs_shell/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/littlefs_examples/littlefs_shell/example_board_readme.md)
