# Cross toolchain variables
XT_PRG_PREFIX=mipsel-linux-gnu-
CC=$(XT_PRG_PREFIX)gcc
LD=$(XT_PRG_PREFIX)ld

# uMPS3-related paths

# search for the umps3 installation prefix.
ifneq ($(wildcard /usr/bin/umps3),)
	UMPS3_DIR_PREFIX = /usr
else
	UMPS3_DIR_PREFIX = /usr/local
endif

UMPS3_DATA_DIR=$(UMPS3_DIR_PREFIX)/share/umps3
UMPS3_INCLUDE_DIR=$(UMPS3_DIR_PREFIX)/include/umps3

# Compiler options
CFLAGS_LANG=-ffreestanding 
CFLAGS_MIPS=-mips1 -mabi=32 -mno-gpopt -EL -G 0 -mno-abicalls -fno-pic -mfp32
CFLAGS=$(CFLAGS_LANG) $(CFLAGS_MIPS) -I$(UMPS3_INCLUDE_DIR) -I/$(CURDIR) -I$(CURDIR)/phase2 -I$(CURDIR)/phase2/include -Wall -O0

# Linker options
LDFLAGS=-G 0 -T $(UMPS3_DATA_DIR)/umpscore.ldscript -m elf32ltsmip

# Add the location of crt*.S to the search path
VPATH=$(UMPS3_DATA_DIR)

.PHONY:	all clean

all:	kernel.core.umps              
                                    
kernel.core.umps:	kernel			
	umps3-elf2umps -k $<			                                
																	
kernel:	phase2/include/p2test.04.o phase2/src/pcb.o phase2/src/ash.o phase2/src/ns.o phase2/src/exceptions.o phase2/src/initial.o phase2/src/interrupts.o phase2/src/scheduler.o phase2/src/syscall.o  crtso.o libumps.o
	$(LD) -o $@ $^ $(LDFLAGS)

clean:
	rm -f phase2/src/*.o *.o phase2/include/*.o kernel kernel.*.umps

# Pattern rule for assembly modules
%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<
