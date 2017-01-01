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

LINT := -q --enable=all -I./src/include/ -I./src/

BINSRC := $(shell find src/ ! -wholename src/lib/\* \( ! -regex '.*/\..*' \) -name \*.c -o -name \*.s)
BINS := $(addsuffix .o,$(patsubst src/%,bin/%,$(basename $(BINSRC))))

LIBSRC := $(shell find src/lib/ \( ! -regex '.*/\..*' \) -name \*.c -o -name \*.s)
LIBBINS := $(addsuffix .o,$(patsubst src/%,bin/%,$(basename $(LIBSRC))))
LIBS := $(addsuffix .a,$(patsubst src/lib/%,bin/lib/lib%,$(wildcard src/lib/*)))

KERNEL := bin/kernel.bin
BOOTCD := bin/bootcd.iso

REDIRECT := 2> >(tee -a errors.log >&2)


all: boot

boot: $(BOOTCD)

kernel: $(KERNEL)

$(KERNEL): src/linker.ld $(BINS) $(LIBS)
	@echo " LINK  " $<
	@ld -melf_i386 -o $@ -T $< -static $(BINS) -Lbin/lib/ -lstd -lpanic -lqueue $(REDIRECT)

bin/%.o: src/%.s
	@echo " ASM   " $<
	@mkdir -p $(dir $@)
	@$(ASM) $(AFLAGS) -o $@ $< $(REDIRECT)

bin/%.o: src/%.c
	@echo " CC    " $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ -c $< $(REDIRECT)

$(LIBS): $(LIBBINS)
	$(eval name := $(patsubst lib%,bin/lib/%/,$(basename $(notdir $@))))
	@echo " AR    " $(name)
	@ar rcs $@ $(filter $(name)%,$^) $(REDIRECT)

doc:
	@echo " RM     doc/html/"
	@rm -rf doc/html/
	@echo " DOC    Doxyfile"
	@doxygen Doxyfile

lint:
	@echo " LINT   "
	@rm -f lint.log
	@cppcheck $(LINT) ./src/ 2> >(tee lint.log | scripts/lintsum.pl)

$(BOOTCD): $(KERNEL)
	@rm -rf bin/isofiles
	@echo " MKDIR " bin/isofiles
	@mkdir -p bin/isofiles
	@echo " CP     src/boot/"
	@cp -r src/boot/ bin/isofiles
	@echo " CP    " $(KERNEL)
	@cp $(KERNEL) bin/isofiles/boot/
	@echo " ISO    bin/isofiles/"
	@genisoimage $(ISOFLAGS) -o $@ bin/isofiles
	@scripts/validate

.NOTPARALLEL:

clean:
	@echo " CLEAN  *.log"
	@rm -f *.log
	@echo " CLEAN  *.o *.a"
	@rm -f $(BINS) $(LIBBINS) $(LIBS)

realclean: clean
	@echo " CLEAN  bin/"
	@rm -rf bin/

