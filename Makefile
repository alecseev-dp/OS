CC = gcc
CFLAGS = -Wall

all: callPr_O0_g callPr_O1 callPr_O2 callPr_O3 callPr_Os callPr_asm

callPr_O0_g: callPr.c addCalc.c
	$(CC) $(CFLAGS) -g -O0 -o callPr_O0_g callPr.c addCalc.c -lm

callPr_O1: callPr.c addCalc.c
	$(CC) $(CFLAGS) -O1 -o callPr_O1 callPr.c addCalc.c -lm

callPr_O2: callPr.c addCalc.c
	$(CC) $(CFLAGS) -O2 -o callPr_O2 callPr.c addCalc.c -lm

callPr_O3: callPr.c addCalc.c
	$(CC) $(CFLAGS) -O3 -o callPr_O3 callPr.c addCalc.c -lm

callPr_Os: callPr.c addCalc.c
	$(CC) $(CFLAGS) -Os -o callPr_Os callPr.c addCalc.c -lm

callPr_asm: callPr.c addCalcMod.s
	$(CC) $(CFLAGS) -g -o callPr_asm callPr.c addCalcMod.s -lm

clean:
	rm -f callPr_O0_g callPr_O1 callPr_O2 callPr_O3 callPr_Os callPr_asm *.o