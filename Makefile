BINDIR = bin
TARGET = Nilgiri
CPPC = g++
DEFINES = -D_REENTRANT

#################################
#### Debugging static binary ####
#################################
CPPFLAGS = -ggdb -W -Wall -Wstrict-prototypes -Wshadow -Wno-unused -static

##################################
#### Debugging dynamic binary ####
##################################
#CPPFLAGS = -ggdb -W -Wall -Wstrict-prototypes -Wshadow -Wno-unused

#################################
#### Optimized static binary ####
#################################
#CPPFLAGS = -O3 -mcpu=pentiumpro -W -Wall -Wstrict-prototypes -Wshadow -Wno-unused -static -static-libgcc $(DEFINES)

##################################
#### Optimized dynamic binary ####
##################################
#CPPFLAGS = -O3 -mcpu=pentiumpro -W -Wall -Wstrict-prototypes -Wshadow -Wno-unused $(DEFINES)

SRCS = act.comm.cc act.immortal.cc act.informative.cc act.item.cc act.modify.cc act.modify2.cc act.movement.cc act.offensive.cc act.other.cc addlog.cc bug.cc combat.cc command.cc container.cc constants.cc db.cc descriptor.cc email_conf.cc err.cc interpreter.cc item_container.cc item_note.cc library.cc main.cc player.cc spec_proc.cc spells.cc tcphost.cc time0.cc tinyworld_character.cc tinyworld_item.cc tinyworld_room.cc tinyworld_time.cc tinyworld_zone.cc utils.cc

OBJS = $(BINDIR)/act.comm.o $(BINDIR)/act.immortal.o $(BINDIR)/act.informative.o $(BINDIR)/act.item.o $(BINDIR)/act.modify.o $(BINDIR)/act.modify2.o $(BINDIR)/act.movement.o $(BINDIR)/act.offensive.o $(BINDIR)/act.other.o $(BINDIR)/addlog.o $(BINDIR)/bug.o $(BINDIR)/combat.o $(BINDIR)/command.o $(BINDIR)/container.o $(BINDIR)/constants.o $(BINDIR)/db.o $(BINDIR)/descriptor.o $(BINDIR)/email_conf.o $(BINDIR)/err.o $(BINDIR)/interpreter.o $(BINDIR)/item_container.o $(BINDIR)/item_note.o $(BINDIR)/library.o $(BINDIR)/main.o $(BINDIR)/player.o $(BINDIR)/spec_proc.o $(BINDIR)/spells.o $(BINDIR)/tcphost.o $(BINDIR)/time0.o $(BINDIR)/tinyworld_character.o $(BINDIR)/tinyworld_item.o $(BINDIR)/tinyworld_room.o $(BINDIR)/tinyworld_time.o $(BINDIR)/tinyworld_zone.o $(BINDIR)/utils.o

LIBS = -lpthread -L$(HOME)/lib -lsqlite -lcrypt

all: $(BINDIR)/$(TARGET) lanalysis

$(BINDIR)/$(TARGET): Makefile $(OBJS)
	$(CPPC) -o $(BINDIR)/$(TARGET) $(CPPFLAGS) $(OBJS) $(LIBS)

lanalysis: $(BINDIR)/lanalysis.o
	$(CPPC) -o $(BINDIR)/lanalysis $(CPPFLAGS) lanalysis.cc -lncurses $(LIBS)
	

depend:
	makedepend -p$(BINDIR)/ -- $(SRCS)
	rm -f Makefile.bak

clean:
	rm -f $(BINDIR)/$(TARGET) $(OBJS) Makefile.bak Nilgiri.exe lib/Nilgiri.exe.stackdump lib/Nilgiri.exe.core lib/gmon.out bin/lanalysis lanalysis.exe.core

wipe:
	rm -f lib/tinyworldB lib/bugdB logdB lib/email_conf_data

$(BINDIR)/%.o:%.cc
	$(CPPC) $(CPPFLAGS) -c -o $@ $<

# DO NOT DELETE

bin/act.comm.o: tinyworld_character.h /usr/include/time.h
bin/act.comm.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/act.comm.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/act.comm.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.comm.o: /usr/include/sys/lock.h
bin/act.comm.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.comm.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/act.comm.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/act.comm.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/act.comm.o: /usr/include/crypt.h container.h utils.h /usr/include/stdio.h
bin/act.comm.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.comm.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/act.comm.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/act.comm.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h
bin/act.comm.o: err.h library.h time0.h /usr/include/sys/time.h constants.h
bin/act.comm.o: email_conf.h tinyworld_zone.h tinyworld_room.h
bin/act.comm.o: tinyworld_item.h command.h tinyworld_time.h interpreter.h
bin/act.immortal.o: tinyworld_character.h /usr/include/time.h
bin/act.immortal.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/act.immortal.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/act.immortal.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.immortal.o: /usr/include/sys/lock.h
bin/act.immortal.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.immortal.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/act.immortal.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/act.immortal.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/act.immortal.o: /usr/include/crypt.h container.h utils.h
bin/act.immortal.o: /usr/include/stdio.h
bin/act.immortal.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.immortal.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/act.immortal.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/act.immortal.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h
bin/act.immortal.o: err.h library.h time0.h /usr/include/sys/time.h
bin/act.immortal.o: constants.h email_conf.h tinyworld_zone.h
bin/act.immortal.o: tinyworld_room.h tinyworld_item.h command.h
bin/act.immortal.o: tinyworld_time.h interpreter.h descriptor.h player.h
bin/act.immortal.o: bug.h addlog.h
bin/act.informative.o: descriptor.h /usr/include/crypt.h /usr/include/_ansi.h
bin/act.informative.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/act.informative.o: /usr/include/machine/ieeefp.h utils.h
bin/act.informative.o: /usr/include/stdio.h
bin/act.informative.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.informative.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.informative.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.informative.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/act.informative.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/act.informative.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/act.informative.o: /usr/include/alloca.h /usr/include/string.h
bin/act.informative.o: /usr/include/assert.h /usr/include/errno.h
bin/act.informative.o: /usr/include/sys/errno.h db.h sqlite.h err.h library.h
bin/act.informative.o: time0.h /usr/include/time.h
bin/act.informative.o: /usr/include/machine/time.h
bin/act.informative.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/act.informative.o: player.h tinyworld_character.h container.h constants.h
bin/act.informative.o: email_conf.h tinyworld_zone.h tinyworld_room.h
bin/act.informative.o: tinyworld_item.h command.h tinyworld_time.h
bin/act.informative.o: interpreter.h
bin/act.item.o: tinyworld_item.h container.h utils.h /usr/include/stdio.h
bin/act.item.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/act.item.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/act.item.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.item.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.item.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.item.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/act.item.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/act.item.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/act.item.o: /usr/include/alloca.h /usr/include/string.h
bin/act.item.o: /usr/include/assert.h /usr/include/errno.h
bin/act.item.o: /usr/include/sys/errno.h db.h sqlite.h err.h library.h
bin/act.item.o: time0.h /usr/include/time.h /usr/include/machine/time.h
bin/act.item.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/act.item.o: constants.h tinyworld_character.h /usr/include/crypt.h
bin/act.item.o: email_conf.h tinyworld_zone.h tinyworld_room.h command.h
bin/act.item.o: tinyworld_time.h interpreter.h item_note.h
bin/act.modify.o: /usr/include/stdio.h /usr/include/_ansi.h
bin/act.modify.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/act.modify.o: /usr/include/machine/ieeefp.h
bin/act.modify.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.modify.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.modify.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.modify.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/act.modify.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/act.modify.o: utils.h /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/act.modify.o: /usr/include/alloca.h /usr/include/string.h
bin/act.modify.o: /usr/include/assert.h /usr/include/errno.h
bin/act.modify.o: /usr/include/sys/errno.h db.h sqlite.h err.h library.h
bin/act.modify.o: time0.h /usr/include/time.h /usr/include/machine/time.h
bin/act.modify.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/act.modify.o: tinyworld_character.h /usr/include/crypt.h container.h
bin/act.modify.o: constants.h email_conf.h tinyworld_zone.h tinyworld_room.h
bin/act.modify.o: tinyworld_item.h command.h tinyworld_time.h interpreter.h
bin/act.modify.o: addlog.h
bin/act.modify2.o: /usr/include/stdio.h /usr/include/_ansi.h
bin/act.modify2.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/act.modify2.o: /usr/include/machine/ieeefp.h
bin/act.modify2.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.modify2.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.modify2.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.modify2.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/act.modify2.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/act.modify2.o: tinyworld_character.h /usr/include/time.h
bin/act.modify2.o: /usr/include/machine/time.h /usr/include/sys/features.h
bin/act.modify2.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/act.modify2.o: /usr/include/crypt.h container.h utils.h
bin/act.modify2.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/act.modify2.o: /usr/include/alloca.h /usr/include/string.h
bin/act.modify2.o: /usr/include/assert.h db.h sqlite.h err.h library.h
bin/act.modify2.o: time0.h /usr/include/sys/time.h constants.h email_conf.h
bin/act.modify2.o: tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/act.modify2.o: command.h tinyworld_time.h interpreter.h
bin/act.movement.o: tinyworld_character.h /usr/include/time.h
bin/act.movement.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/act.movement.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/act.movement.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.movement.o: /usr/include/sys/lock.h
bin/act.movement.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.movement.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/act.movement.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/act.movement.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/act.movement.o: /usr/include/crypt.h container.h utils.h
bin/act.movement.o: /usr/include/stdio.h
bin/act.movement.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.movement.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/act.movement.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/act.movement.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h
bin/act.movement.o: err.h library.h time0.h /usr/include/sys/time.h
bin/act.movement.o: constants.h email_conf.h tinyworld_zone.h
bin/act.movement.o: tinyworld_room.h tinyworld_item.h command.h
bin/act.movement.o: tinyworld_time.h interpreter.h
bin/act.offensive.o: tinyworld_character.h /usr/include/time.h
bin/act.offensive.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/act.offensive.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/act.offensive.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.offensive.o: /usr/include/sys/lock.h
bin/act.offensive.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.offensive.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/act.offensive.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/act.offensive.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/act.offensive.o: /usr/include/crypt.h container.h utils.h
bin/act.offensive.o: /usr/include/stdio.h
bin/act.offensive.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.offensive.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/act.offensive.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/act.offensive.o: /usr/include/string.h /usr/include/assert.h db.h
bin/act.offensive.o: sqlite.h err.h library.h time0.h /usr/include/sys/time.h
bin/act.offensive.o: constants.h email_conf.h tinyworld_zone.h
bin/act.offensive.o: tinyworld_room.h tinyworld_item.h command.h
bin/act.offensive.o: tinyworld_time.h interpreter.h combat.h addlog.h
bin/act.other.o: tinyworld_character.h /usr/include/time.h
bin/act.other.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/act.other.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/act.other.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/act.other.o: /usr/include/sys/lock.h
bin/act.other.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/act.other.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/act.other.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/act.other.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/act.other.o: /usr/include/crypt.h container.h utils.h
bin/act.other.o: /usr/include/stdio.h
bin/act.other.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/act.other.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/act.other.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/act.other.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h
bin/act.other.o: err.h library.h time0.h /usr/include/sys/time.h constants.h
bin/act.other.o: email_conf.h tinyworld_zone.h tinyworld_room.h
bin/act.other.o: tinyworld_item.h command.h tinyworld_time.h interpreter.h
bin/act.other.o: descriptor.h player.h
bin/addlog.o: err.h tinyworld_character.h /usr/include/time.h
bin/addlog.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/addlog.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/addlog.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/addlog.o: /usr/include/sys/lock.h
bin/addlog.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/addlog.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/addlog.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/addlog.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/addlog.o: /usr/include/crypt.h container.h utils.h /usr/include/stdio.h
bin/addlog.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/addlog.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/addlog.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/addlog.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h
bin/addlog.o: library.h time0.h /usr/include/sys/time.h constants.h
bin/addlog.o: email_conf.h tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/addlog.o: command.h tinyworld_time.h interpreter.h descriptor.h player.h
bin/bug.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
bin/bug.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/bug.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/bug.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/bug.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/bug.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/bug.o: /usr/include/machine/types.h /usr/include/sys/stdio.h sqlite.h
bin/bug.o: tinyworld_character.h /usr/include/time.h
bin/bug.o: /usr/include/machine/time.h /usr/include/sys/features.h
bin/bug.o: /usr/include/errno.h /usr/include/sys/errno.h /usr/include/crypt.h
bin/bug.o: container.h utils.h /usr/include/stdlib.h
bin/bug.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/bug.o: /usr/include/string.h /usr/include/assert.h db.h err.h library.h
bin/bug.o: time0.h /usr/include/sys/time.h constants.h email_conf.h
bin/bug.o: tinyworld_zone.h tinyworld_room.h tinyworld_item.h command.h
bin/bug.o: tinyworld_time.h interpreter.h bug.h
bin/combat.o: combat.h tinyworld_character.h /usr/include/time.h
bin/combat.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/combat.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/combat.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/combat.o: /usr/include/sys/lock.h
bin/combat.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/combat.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/combat.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/combat.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/combat.o: /usr/include/crypt.h container.h utils.h /usr/include/stdio.h
bin/combat.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/combat.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/combat.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/combat.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h err.h
bin/combat.o: library.h time0.h /usr/include/sys/time.h constants.h
bin/combat.o: email_conf.h tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/combat.o: command.h tinyworld_time.h interpreter.h addlog.h
bin/command.o: /usr/include/stdio.h /usr/include/_ansi.h
bin/command.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/command.o: /usr/include/machine/ieeefp.h
bin/command.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/command.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/command.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/command.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/command.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/command.o: command.h utils.h /usr/include/stdlib.h
bin/command.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/command.o: /usr/include/string.h /usr/include/assert.h
bin/command.o: /usr/include/errno.h /usr/include/sys/errno.h db.h sqlite.h
bin/command.o: err.h library.h time0.h /usr/include/time.h
bin/command.o: /usr/include/machine/time.h /usr/include/sys/features.h
bin/command.o: /usr/include/sys/time.h interpreter.h constants.h
bin/container.o: /usr/include/stdlib.h /usr/include/_ansi.h
bin/container.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/container.o: /usr/include/machine/ieeefp.h
bin/container.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/container.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/container.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
bin/container.o: /usr/include/alloca.h /usr/include/assert.h
bin/container.o: /usr/include/errno.h /usr/include/sys/errno.h container.h
bin/container.o: utils.h /usr/include/stdio.h
bin/container.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/container.o: /usr/include/sys/types.h /usr/include/machine/types.h
bin/container.o: /usr/include/sys/stdio.h /usr/include/string.h db.h sqlite.h
bin/container.o: err.h library.h time0.h /usr/include/time.h
bin/container.o: /usr/include/machine/time.h /usr/include/sys/features.h
bin/container.o: /usr/include/sys/time.h constants.h addlog.h command.h
bin/constants.o: /usr/include/stdlib.h /usr/include/_ansi.h
bin/constants.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/constants.o: /usr/include/machine/ieeefp.h
bin/constants.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/constants.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/constants.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
bin/constants.o: /usr/include/alloca.h /usr/include/errno.h
bin/constants.o: /usr/include/sys/errno.h constants.h utils.h
bin/constants.o: /usr/include/stdio.h
bin/constants.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/constants.o: /usr/include/sys/types.h /usr/include/machine/types.h
bin/constants.o: /usr/include/sys/stdio.h /usr/include/string.h
bin/constants.o: /usr/include/assert.h db.h sqlite.h err.h library.h time0.h
bin/constants.o: /usr/include/time.h /usr/include/machine/time.h
bin/constants.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/db.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
bin/db.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/db.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/db.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/db.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/db.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/db.o: /usr/include/machine/types.h /usr/include/sys/stdio.h err.h db.h
bin/db.o: sqlite.h utils.h /usr/include/stdlib.h
bin/db.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/db.o: /usr/include/string.h /usr/include/assert.h /usr/include/errno.h
bin/db.o: /usr/include/sys/errno.h library.h time0.h /usr/include/time.h
bin/db.o: /usr/include/machine/time.h /usr/include/sys/features.h
bin/db.o: /usr/include/sys/time.h addlog.h command.h container.h constants.h
bin/descriptor.o: /usr/include/unistd.h /usr/include/sys/unistd.h
bin/descriptor.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/descriptor.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/descriptor.o: /usr/include/sys/types.h /usr/include/sys/_types.h
bin/descriptor.o: /usr/include/sys/lock.h
bin/descriptor.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/descriptor.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/descriptor.o: /usr/include/stdio.h
bin/descriptor.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/descriptor.o: /usr/include/sys/reent.h /usr/include/sys/stdio.h
bin/descriptor.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/descriptor.o: /usr/include/alloca.h /usr/include/errno.h
bin/descriptor.o: /usr/include/sys/errno.h /usr/include/string.h
bin/descriptor.o: /usr/include/pthread.h /usr/include/signal.h
bin/descriptor.o: /usr/include/sys/signal.h /usr/include/sched.h
bin/descriptor.o: /usr/include/time.h /usr/include/machine/time.h
bin/descriptor.o: descriptor.h /usr/include/crypt.h utils.h
bin/descriptor.o: /usr/include/assert.h db.h sqlite.h err.h library.h time0.h
bin/descriptor.o: /usr/include/sys/time.h player.h tinyworld_character.h
bin/descriptor.o: container.h constants.h email_conf.h tinyworld_zone.h
bin/descriptor.o: tinyworld_room.h tinyworld_item.h command.h
bin/descriptor.o: tinyworld_time.h interpreter.h tcphost.h main.h addlog.h
bin/email_conf.o: /usr/include/string.h /usr/include/_ansi.h
bin/email_conf.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/email_conf.o: /usr/include/machine/ieeefp.h /usr/include/sys/reent.h
bin/email_conf.o: /usr/include/sys/_types.h /usr/include/sys/lock.h
bin/email_conf.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/email_conf.o: /usr/include/errno.h /usr/include/sys/errno.h err.h time0.h
bin/email_conf.o: /usr/include/time.h /usr/include/machine/time.h
bin/email_conf.o: /usr/include/sys/types.h /usr/include/machine/types.h
bin/email_conf.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/email_conf.o: email_conf.h utils.h /usr/include/stdio.h
bin/email_conf.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/email_conf.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/email_conf.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/email_conf.o: /usr/include/assert.h db.h sqlite.h library.h player.h
bin/email_conf.o: tinyworld_character.h /usr/include/crypt.h container.h
bin/email_conf.o: constants.h tinyworld_zone.h tinyworld_room.h
bin/email_conf.o: tinyworld_item.h command.h tinyworld_time.h interpreter.h
bin/err.o: /usr/include/stdio.h /usr/include/_ansi.h /usr/include/newlib.h
bin/err.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/err.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/err.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/err.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/err.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/err.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/err.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/err.o: /usr/include/alloca.h /usr/include/string.h /usr/include/errno.h
bin/err.o: /usr/include/sys/errno.h sqlite.h err.h time0.h
bin/err.o: /usr/include/time.h /usr/include/machine/time.h
bin/err.o: /usr/include/sys/features.h /usr/include/sys/time.h addlog.h
bin/err.o: command.h utils.h /usr/include/assert.h db.h library.h container.h
bin/err.o: constants.h
bin/interpreter.o: tinyworld_character.h /usr/include/time.h
bin/interpreter.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/interpreter.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/interpreter.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/interpreter.o: /usr/include/sys/lock.h
bin/interpreter.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/interpreter.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/interpreter.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/interpreter.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/interpreter.o: /usr/include/crypt.h container.h utils.h
bin/interpreter.o: /usr/include/stdio.h
bin/interpreter.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/interpreter.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/interpreter.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/interpreter.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h
bin/interpreter.o: err.h library.h time0.h /usr/include/sys/time.h
bin/interpreter.o: constants.h email_conf.h tinyworld_zone.h tinyworld_room.h
bin/interpreter.o: tinyworld_item.h command.h tinyworld_time.h interpreter.h
bin/item_container.o: item_container.h tinyworld_item.h container.h utils.h
bin/item_container.o: /usr/include/stdio.h /usr/include/_ansi.h
bin/item_container.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/item_container.o: /usr/include/machine/ieeefp.h
bin/item_container.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/item_container.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/item_container.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/item_container.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/item_container.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/item_container.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/item_container.o: /usr/include/alloca.h /usr/include/string.h
bin/item_container.o: /usr/include/assert.h /usr/include/errno.h
bin/item_container.o: /usr/include/sys/errno.h db.h sqlite.h err.h library.h
bin/item_container.o: time0.h /usr/include/time.h /usr/include/machine/time.h
bin/item_container.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/item_container.o: constants.h
bin/item_note.o: item_note.h tinyworld_item.h container.h utils.h
bin/item_note.o: /usr/include/stdio.h /usr/include/_ansi.h
bin/item_note.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/item_note.o: /usr/include/machine/ieeefp.h
bin/item_note.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/item_note.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/item_note.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/item_note.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/item_note.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/item_note.o: /usr/include/stdlib.h /usr/include/machine/stdlib.h
bin/item_note.o: /usr/include/alloca.h /usr/include/string.h
bin/item_note.o: /usr/include/assert.h /usr/include/errno.h
bin/item_note.o: /usr/include/sys/errno.h db.h sqlite.h err.h library.h
bin/item_note.o: time0.h /usr/include/time.h /usr/include/machine/time.h
bin/item_note.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/item_note.o: constants.h tinyworld_character.h /usr/include/crypt.h
bin/item_note.o: email_conf.h tinyworld_zone.h tinyworld_room.h command.h
bin/item_note.o: tinyworld_time.h interpreter.h
bin/library.o: /usr/include/stdio.h /usr/include/_ansi.h
bin/library.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/library.o: /usr/include/machine/ieeefp.h
bin/library.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/library.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/library.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/library.o: /usr/include/sys/lock.h /usr/include/sys/types.h
bin/library.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/library.o: /usr/include/unistd.h /usr/include/sys/unistd.h
bin/library.o: /usr/include/sys/features.h /usr/include/stdlib.h
bin/library.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/library.o: /usr/include/string.h /usr/include/errno.h
bin/library.o: /usr/include/sys/errno.h /usr/include/dirent.h
bin/library.o: /usr/include/sys/dirent.h /usr/include/assert.h library.h
bin/library.o: err.h utils.h db.h sqlite.h time0.h /usr/include/time.h
bin/library.o: /usr/include/machine/time.h /usr/include/sys/time.h
bin/library.o: descriptor.h /usr/include/crypt.h player.h
bin/library.o: tinyworld_character.h container.h constants.h email_conf.h
bin/library.o: tinyworld_zone.h tinyworld_room.h tinyworld_item.h command.h
bin/library.o: tinyworld_time.h interpreter.h
bin/main.o: /usr/include/stdlib.h /usr/include/_ansi.h /usr/include/newlib.h
bin/main.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/main.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/main.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/main.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
bin/main.o: /usr/include/alloca.h /usr/include/unistd.h
bin/main.o: /usr/include/sys/unistd.h /usr/include/sys/types.h
bin/main.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/main.o: /usr/include/sys/select.h /usr/include/sys/cdefs.h
bin/main.o: /usr/include/sys/time.h /usr/include/time.h
bin/main.o: /usr/include/machine/time.h /usr/include/errno.h
bin/main.o: /usr/include/sys/errno.h /usr/include/string.h
bin/main.o: /usr/include/pthread.h /usr/include/signal.h
bin/main.o: /usr/include/sys/signal.h /usr/include/sched.h
bin/main.o: /usr/include/sys/socket.h /usr/include/features.h
bin/main.o: /usr/include/cygwin/socket.h /usr/include/asm/socket.h
bin/main.o: /usr/include/cygwin/if.h /usr/include/cygwin/sockios.h
bin/main.o: /usr/include/cygwin/uio.h main.h err.h time0.h tcphost.h
bin/main.o: descriptor.h /usr/include/crypt.h utils.h /usr/include/stdio.h
bin/main.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/main.o: /usr/include/sys/stdio.h /usr/include/assert.h db.h sqlite.h
bin/main.o: library.h player.h tinyworld_character.h container.h constants.h
bin/main.o: email_conf.h tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/main.o: command.h tinyworld_time.h interpreter.h combat.h addlog.h bug.h
bin/player.o: player.h tinyworld_character.h /usr/include/time.h
bin/player.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/player.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/player.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/player.o: /usr/include/sys/lock.h
bin/player.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/player.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/player.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/player.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/player.o: /usr/include/crypt.h container.h utils.h /usr/include/stdio.h
bin/player.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/player.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/player.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/player.o: /usr/include/string.h /usr/include/assert.h db.h sqlite.h err.h
bin/player.o: library.h time0.h /usr/include/sys/time.h constants.h
bin/player.o: email_conf.h tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/player.o: command.h tinyworld_time.h interpreter.h descriptor.h addlog.h
bin/tcphost.o: /usr/include/stdlib.h /usr/include/_ansi.h
bin/tcphost.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/tcphost.o: /usr/include/machine/ieeefp.h
bin/tcphost.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/tcphost.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/tcphost.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
bin/tcphost.o: /usr/include/alloca.h /usr/include/unistd.h
bin/tcphost.o: /usr/include/sys/unistd.h /usr/include/sys/types.h
bin/tcphost.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/tcphost.o: /usr/include/netdb.h /usr/include/sys/socket.h
bin/tcphost.o: /usr/include/features.h /usr/include/cygwin/socket.h
bin/tcphost.o: /usr/include/asm/socket.h /usr/include/cygwin/if.h
bin/tcphost.o: /usr/include/cygwin/sockios.h /usr/include/cygwin/uio.h
bin/tcphost.o: /usr/include/sys/time.h /usr/include/netinet/in.h
bin/tcphost.o: /usr/include/cygwin/in.h /usr/include/stdint.h
bin/tcphost.o: /usr/include/asm/byteorder.h /usr/include/arpa/telnet.h
bin/tcphost.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
bin/tcphost.o: /usr/include/sys/stat.h /usr/include/time.h
bin/tcphost.o: /usr/include/machine/time.h /usr/include/sys/param.h
bin/tcphost.o: /usr/include/limits.h /usr/include/errno.h
bin/tcphost.o: /usr/include/sys/errno.h /usr/include/string.h
bin/tcphost.o: /usr/include/stdio.h
bin/tcphost.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/tcphost.o: /usr/include/sys/stdio.h err.h tcphost.h addlog.h command.h
bin/tcphost.o: utils.h /usr/include/assert.h db.h sqlite.h library.h time0.h
bin/tcphost.o: container.h constants.h
bin/time0.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/time0.o: /usr/include/sys/reent.h /usr/include/_ansi.h
bin/time0.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/time0.o: /usr/include/machine/ieeefp.h /usr/include/sys/_types.h
bin/time0.o: /usr/include/sys/lock.h
bin/time0.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/time0.o: /usr/include/string.h time0.h /usr/include/time.h
bin/time0.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/time0.o: /usr/include/machine/types.h /usr/include/sys/features.h
bin/time0.o: /usr/include/sys/time.h err.h utils.h /usr/include/stdio.h
bin/time0.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/time0.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/time0.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/time0.o: /usr/include/assert.h db.h sqlite.h library.h
bin/tinyworld_character.o: /usr/include/stdlib.h /usr/include/_ansi.h
bin/tinyworld_character.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/tinyworld_character.o: /usr/include/machine/ieeefp.h
bin/tinyworld_character.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/tinyworld_character.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/tinyworld_character.o: /usr/include/sys/lock.h
bin/tinyworld_character.o: /usr/include/machine/stdlib.h
bin/tinyworld_character.o: /usr/include/alloca.h /usr/include/string.h db.h
bin/tinyworld_character.o: sqlite.h
bin/tinyworld_character.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/tinyworld_character.o: tinyworld_character.h /usr/include/time.h
bin/tinyworld_character.o: /usr/include/machine/time.h
bin/tinyworld_character.o: /usr/include/sys/types.h
bin/tinyworld_character.o: /usr/include/machine/types.h
bin/tinyworld_character.o: /usr/include/sys/features.h /usr/include/errno.h
bin/tinyworld_character.o: /usr/include/sys/errno.h /usr/include/crypt.h
bin/tinyworld_character.o: container.h utils.h /usr/include/stdio.h
bin/tinyworld_character.o: /usr/include/sys/stdio.h /usr/include/assert.h
bin/tinyworld_character.o: err.h library.h time0.h /usr/include/sys/time.h
bin/tinyworld_character.o: constants.h email_conf.h tinyworld_zone.h
bin/tinyworld_character.o: tinyworld_room.h tinyworld_item.h command.h
bin/tinyworld_character.o: tinyworld_time.h interpreter.h descriptor.h
bin/tinyworld_character.o: player.h combat.h addlog.h
bin/tinyworld_item.o: tinyworld_character.h /usr/include/time.h
bin/tinyworld_item.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/tinyworld_item.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/tinyworld_item.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/tinyworld_item.o: /usr/include/sys/lock.h
bin/tinyworld_item.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/tinyworld_item.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/tinyworld_item.o: /usr/include/machine/types.h
bin/tinyworld_item.o: /usr/include/sys/features.h /usr/include/errno.h
bin/tinyworld_item.o: /usr/include/sys/errno.h /usr/include/crypt.h
bin/tinyworld_item.o: container.h utils.h /usr/include/stdio.h
bin/tinyworld_item.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/tinyworld_item.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/tinyworld_item.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/tinyworld_item.o: /usr/include/string.h /usr/include/assert.h db.h
bin/tinyworld_item.o: sqlite.h err.h library.h time0.h
bin/tinyworld_item.o: /usr/include/sys/time.h constants.h email_conf.h
bin/tinyworld_item.o: tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/tinyworld_item.o: command.h tinyworld_time.h interpreter.h descriptor.h
bin/tinyworld_item.o: player.h addlog.h item_container.h item_note.h
bin/tinyworld_room.o: /usr/include/string.h /usr/include/_ansi.h
bin/tinyworld_room.o: /usr/include/newlib.h /usr/include/sys/config.h
bin/tinyworld_room.o: /usr/include/machine/ieeefp.h /usr/include/sys/reent.h
bin/tinyworld_room.o: /usr/include/sys/_types.h /usr/include/sys/lock.h
bin/tinyworld_room.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/tinyworld_room.o: /usr/include/errno.h /usr/include/sys/errno.h err.h
bin/tinyworld_room.o: tinyworld_room.h container.h utils.h
bin/tinyworld_room.o: /usr/include/stdio.h
bin/tinyworld_room.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/tinyworld_room.o: /usr/include/sys/types.h /usr/include/machine/types.h
bin/tinyworld_room.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/tinyworld_room.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/tinyworld_room.o: /usr/include/assert.h db.h sqlite.h library.h time0.h
bin/tinyworld_room.o: /usr/include/time.h /usr/include/machine/time.h
bin/tinyworld_room.o: /usr/include/sys/features.h /usr/include/sys/time.h
bin/tinyworld_room.o: constants.h tinyworld_item.h
bin/tinyworld_time.o: tinyworld_time.h /usr/include/time.h
bin/tinyworld_time.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/tinyworld_time.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/tinyworld_time.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/tinyworld_time.o: /usr/include/sys/lock.h
bin/tinyworld_time.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/tinyworld_time.o: /usr/include/machine/time.h /usr/include/sys/types.h
bin/tinyworld_time.o: /usr/include/machine/types.h
bin/tinyworld_time.o: /usr/include/sys/features.h utils.h
bin/tinyworld_time.o: /usr/include/stdio.h
bin/tinyworld_time.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/tinyworld_time.o: /usr/include/sys/stdio.h /usr/include/stdlib.h
bin/tinyworld_time.o: /usr/include/machine/stdlib.h /usr/include/alloca.h
bin/tinyworld_time.o: /usr/include/string.h /usr/include/assert.h
bin/tinyworld_time.o: /usr/include/errno.h /usr/include/sys/errno.h db.h
bin/tinyworld_time.o: sqlite.h err.h library.h time0.h
bin/tinyworld_time.o: /usr/include/sys/time.h descriptor.h
bin/tinyworld_time.o: /usr/include/crypt.h player.h tinyworld_character.h
bin/tinyworld_time.o: container.h constants.h email_conf.h tinyworld_zone.h
bin/tinyworld_time.o: tinyworld_room.h tinyworld_item.h command.h
bin/tinyworld_time.o: interpreter.h
bin/tinyworld_zone.o: db.h sqlite.h
bin/tinyworld_zone.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/tinyworld_zone.o: tinyworld_zone.h /usr/include/stdlib.h
bin/tinyworld_zone.o: /usr/include/_ansi.h /usr/include/newlib.h
bin/tinyworld_zone.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/tinyworld_zone.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/tinyworld_zone.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/tinyworld_zone.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
bin/tinyworld_zone.o: /usr/include/alloca.h container.h utils.h
bin/tinyworld_zone.o: /usr/include/stdio.h /usr/include/sys/types.h
bin/tinyworld_zone.o: /usr/include/machine/types.h /usr/include/sys/stdio.h
bin/tinyworld_zone.o: /usr/include/string.h /usr/include/assert.h
bin/tinyworld_zone.o: /usr/include/errno.h /usr/include/sys/errno.h err.h
bin/tinyworld_zone.o: library.h time0.h /usr/include/time.h
bin/tinyworld_zone.o: /usr/include/machine/time.h /usr/include/sys/features.h
bin/tinyworld_zone.o: /usr/include/sys/time.h constants.h
bin/utils.o: /usr/include/stdlib.h /usr/include/_ansi.h /usr/include/newlib.h
bin/utils.o: /usr/include/sys/config.h /usr/include/machine/ieeefp.h
bin/utils.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stddef.h
bin/utils.o: /usr/include/sys/reent.h /usr/include/sys/_types.h
bin/utils.o: /usr/include/sys/lock.h /usr/include/machine/stdlib.h
bin/utils.o: /usr/include/alloca.h /usr/include/stdio.h
bin/utils.o: /usr/lib/gcc-lib/i686-pc-cygwin/3.3.3/include/stdarg.h
bin/utils.o: /usr/include/sys/types.h /usr/include/machine/types.h
bin/utils.o: /usr/include/sys/stdio.h /usr/include/string.h
bin/utils.o: /usr/include/errno.h /usr/include/sys/errno.h
bin/utils.o: /usr/include/assert.h utils.h db.h sqlite.h err.h library.h
bin/utils.o: time0.h /usr/include/time.h /usr/include/machine/time.h
bin/utils.o: /usr/include/sys/features.h /usr/include/sys/time.h container.h
bin/utils.o: constants.h tinyworld_character.h /usr/include/crypt.h
bin/utils.o: email_conf.h tinyworld_zone.h tinyworld_room.h tinyworld_item.h
bin/utils.o: command.h tinyworld_time.h interpreter.h
