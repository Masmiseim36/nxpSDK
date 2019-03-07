Set wshShell = CreateObject("WScript.shell")
wshShell.run "mfgtool2.exe -c ""linux"" -l ""Quad Nor Flash Erase"" -s ""6uluboot=14x14evk"" -s ""6ulnor=qspi1"" -s ""6uldtb=14x14-evk"" -s ""norconfig=qspi-nor-micron-n25q256a-config"" "
Set wshShell = Nothing

'norconfig depends on the QSPI chip mounted on board, for i.MX6SX TO1.1 board, norconfig=qspi-nor-spansion-s25fl128s-config
