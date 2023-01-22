@echo off
:: get build target from the environment variable and make it's first letter lowercase
setlocal enabledelayedexpansion
set build_target=%xt_build_target%
set build_target=!build_target:D=d!
set build_target=!build_target:R=r!

set bin_path=../binary

set	rm_cmd=del /q

@echo pre-clean rule
if exist "%bin_path%/dsp_text_%build_target%.bin" %rm_cmd% "%bin_path%/dsp_text_%build_target%.bin"
if exist "%bin_path%/dsp_data_%build_target%.bin" %rm_cmd% "%bin_path%/dsp_data_%build_target%.bin"
if exist "%bin_path%/dsp_ncache_%build_target%.bin" %rm_cmd% "%bin_path%/dsp_reset_%build_target%.bin
