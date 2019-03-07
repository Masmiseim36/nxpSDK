Set wshShell = CreateObject("WScript.shell")
wshShell.run "mfgtool2.exe -c ""linux"" -l ""SDCard Erase"" -s ""6uluboot=14x14evk"" -s ""lite=l"" -s ""6uldtb=14x14-evk"" -s ""6ulldtb=14x14-evk"" -s ""ldo="" -s ""mmc=1"" "
Set wshShell = Nothing

