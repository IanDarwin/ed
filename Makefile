# Makefile for line editor.
# $Id$

CC =	gcc
SHAR =	bundle
MISC =	README TODO Makefile
SRCS =	bufprim.c docmd.c editcmds.c esc.c findmisc.c getlist.c \
	main.c makepat.c match.c misc.c rewr.c subst.c
OBJS =	bufprim.o docmd.o editcmds.o esc.o findmisc.o getlist.o \
	main.o makepat.o match.o misc.o rewr.o subst.o
REALH = edit.h editvars.h findcons.h
TESTS = t?

all:		ed
		for t in $(TESTS); do echo $$t; sh $$t; done

ed:		$(OBJS)
		$(CC) $(OBJS) $(LIBS) -o ed
$(SRCS):	editdefs.h
editvars.h:	editdefs.h
		sed 's/^extern //' editdefs.h > editvars.h
rcsdiff:
		rcsdiff -q RCS/*
dist:		rcsdiff $(MISC) $(SRCS) $(REALH) $(TESTS)
		$(SHAR) $(MISC) $(SRCS) $(REALH) $(TESTS) > dist
clean:
		rm -f core dist *.o ed

# DO NOT DELETE THIS LINE -- mkdep uses it.
# DO NOT PUT ANYTHING AFTER THIS LINE, IT WILL GO AWAY.

main.o : main.c /usr/include/stdio.h \
  /usr/include/stdarg.h \
  /usr/include/signal.h \
  /usr/include/sys/signal.h \
  /usr/include/sys/unistd.h /usr/include/setjmp.h \
  edit.h editvars.h 
makepat.o : makepat.c /usr/include/stdio.h \
  /usr/include/stdarg.h \
  /usr/include/ctype.h edit.h findcons.h 
match.o : match.c /usr/include/stdio.h \
  /usr/include/stdarg.h \
  edit.h findcons.h 

# IF YOU PUT ANYTHING HERE IT WILL GO AWAY
