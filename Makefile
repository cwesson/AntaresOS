.PHONY: all boot clean realclean doc lint
SHELL := /bin/bash

# ISO 8601 style date/time
TIMESTAMP := $(shell date -u +%Y-%m-%dT%H:%M:%SZ)
REVISION := $(shell git rev-parse --short HEAD)$(if $(shell git diff --exit-code),"M",)

CC := gcc
CWARN := -Wall -Wextra -Wunused -Werror -Wformat -Wswitch-default -Wswitch-enum -Wuninitialized \
	-Wshadow -Wpointer-arith -Wcast-align -Wdate-time -Wlogical-op -Wredundant-decls \
	-Wnested-externs -Winline -Wvolatile-register-var -Woverlength-strings
CFLAGS := $(CWARN) -nostdlib -nostartfiles -nodefaultlibs -nostdinc -fno-builtin -std=gnu11 -m32 \
	-DTIMESTAMP=\"$(TIMESTAMP)\" -DREVISION=\"$(REVISION)\" -I./src/include/ -iquote./src/

ASM := nasm
AFLAGS := -f elf

ISOFLAGS := -J -R -D -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -quiet \
	-input-charset utf-8 -hide boot.catalog -hide-joliet boot.catalog -V "AntaresOS"

LINT := -q --enable=all -I./src/include/

SOURCES := $(shell find src/ \( ! -regex '.*/\..*' \) -name \*.c -o -name \*.s)

INCLUDE := $(shell find src/ \( ! -regex '.*/\..*' \) \( ! -regex '.*/include.*' \) -type d)
CFLAGS += $(addprefix -Iquote,$(INCLUDE))

BINS := $(addsuffix .o,$(patsubst src/%,bin/%,$(SOURCES)))
KERNEL := bin/kernel.bin


all: kernel

kernel: $(KERNEL)

$(KERNEL): src/linker.ld bin $(BINS)
	@echo " LINK  " $<
	@ld -melf_i386 -o $@ -T $< $(BINS)

bin/%.s.o: src/%.s
	@echo " ASM   " $<
	@$(ASM) $(AFLAGS) -o $@ $<

bin/%.c.o: src/%.c
	@echo " CC    " $<
	@$(CC) $(CFLAGS) -o $@ -c $<

doc:
	@echo " RM     doc/html/"
	@rm -rf doc/html/
	@echo " DOC    Doxyfile"
	@doxygen Doxyfile

lint:
	@echo " LINT   "
	@rm -f lint.log
	@cppcheck $(LINT) ./src/ 2> >(tee lint.log | scripts/lintsum.pl)

.NOTPARALLEL:

boot: bin/bootcd.iso

bin/bootcd.iso: all
	@rm -rf bin/isofiles
	@echo " MKDIR " bin/isofiles
	@mkdir -p bin/isofiles
	@echo " CP     src/boot/"
	@cp -r src/boot/ bin/isofiles
	@echo " CP    " $(KERNEL)
	@cp $(KERNEL) bin/isofiles/boot/
	@echo " ISO   " $@
	@genisoimage $(ISOFLAGS) -o $@ bin/isofiles
	@scripts/validate
	
bin:
	@echo " MKDIR  bin"
	@find -type d \( ! -regex '.*/\..*' \) | grep ./src/ | sed -e 's/\/src\//\/bin\//' | xargs -l mkdir -p

clean:
	@echo " CLEAN  *.o"
	@rm -f $(BINS)

realclean: clean
	@echo " CLEAN  bin/"
	@rm -rf bin/

