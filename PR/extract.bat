@echo off
if not exist dat\%1 goto error
pr dat\%1 -x ext >%1.txt
dir ext\res*.* >realres.txt
goto end
:error
echo The file '%1' does not exist in dat folder
echo:
:end