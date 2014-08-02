.PHONY: all boot clean realclean doc lint

# ISO 8601 style date/time
TIMESTAMP := $(shell date -u +%Y-%m-%dT%H:%M:%SZ)

CC := gcc
CFLAGS := -Wall -Wextra -Werror -nostdlib -nostartfiles -nodefaultlibs -nostdinc -fno-builtin -std=gnu11 -m32 \
	-DTIMESTAMP=\"$(TIMESTAMP)\" -DREVISION=\"NONE\" -I./src/include/

ASM := nasm
AFLAGS := -f elf

ISOFLAGS := -J -R -D -b boot/grub/stage2_eltorito -no-emul-boot -boot-load-size 4 -boot-info-table -quiet \
	-input-charset utf-8 -hide boot.catalog -hide-joliet boot.catalog -V "AntaresOS"

LINT := -nolib -I./src/include/ -DTIMESTAMP=\"$(TIMESTAMP)\" -booltype _Bool -booltype bool \
	+showscan +stats +gnuextensions +trytorecover

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
	@find ./ -name *.c | xargs -n 1 splint $(LINT) 2>&1 | tee lint.log

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

