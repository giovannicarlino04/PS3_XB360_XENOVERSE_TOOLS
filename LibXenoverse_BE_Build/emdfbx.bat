@echo off
set program="%~dp0%~n0.exe"
if not exist %program% (
    @echo on
    @echo %program% does not exist
    @pause
    @exit 1
)
if [%1]==[] goto :end
:loop
@echo on
%program% %1
@echo off
shift
if not [%1]==[] goto loop
