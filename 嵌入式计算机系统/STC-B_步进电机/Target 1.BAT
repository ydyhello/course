SET C51INC=E:\工训\keil\C51\Inc;E:\工训\keil\C51\Inc\STC
SET C51LIB=E:\工训\keil\C51\Lib
SET CPU_TYPE=STC15F2K60S2 Series
SET CPU_VENDOR=STC
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x02160EC0
"E:\工训\keil\C51\BIN\C51.EXE" @.\output\main.__i
"E:\工训\keil\C51\BIN\C51.EXE" @.\output\SelfStepmotor.__i
"E:\工训\keil\C51\BIN\BL51.EXE" @.\output\output.lnp
"E:\工训\keil\C51\BIN\OH51.EXE" ".\output\output" 
