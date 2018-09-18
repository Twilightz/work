FlashErase\FlashErase.exe -s 0 -l 200000
 
ping 127.0.0.1 -n 1
 
FlashErase\FlashErase.exe -c s -s 0 -l c000 -v -f ssb.bin
pause
