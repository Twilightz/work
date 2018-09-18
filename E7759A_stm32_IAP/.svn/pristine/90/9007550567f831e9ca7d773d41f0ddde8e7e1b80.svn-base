@cd %CD%

@if exist C:\Keil\ARM\ARMCC\bin\fromelf.exe (C:\Keil\ARM\ARMCC\bin\fromelf.exe --bin -o  ../bin/Temp.bin  ../bin/E9624.axf)
@if exist D:\Keil\ARM\BIN40\fromelf.exe (D:\Keil\ARM\BIN40\fromelf.exe --bin -o  ../bin/Temp.bin  ../bin/%1%.axf)
@if exist E:\Keil\ARM\BIN40\fromelf.exe (E:\Keil\ARM\BIN40\fromelf.exe --bin -o  ../bin/Temp.bin  ../bin/%1%.axf)

@binAddCrc.exe ../bin/Temp.bin  ../bin/E9624.bin  Y

%del /f/q ..\bin\Temp.bin

@cd ../bin

@dir *.bin /TW