# To build for GKrellM v2, just type 'make' and 'make install'
# For GKrellM v1, type 'make i8krellm1' and 'make install1'
# 

LFLAGS = -shared


## Support for GKrellM 2.0
#
GTK2_INCLUDE = `pkg-config gtk+-2.0 --cflags`
GTK2_LIB = `pkg-config gtk+-2.0 --libs`
SINSTALLDIR2 = /usr/share/gkrellm2/plugins
UINSTALLDIR2 = $(HOME)/.gkrellm2/plugins
FLAGS2 = -O2 -Wall -fPIC -I. $(GTK2_INCLUDE)
LIBS2 = $(GTK2_LIB)
CC2 = gcc $(FLAGS2) -DGKRELLM2
CC2F = gcc $(FLAGS2) -DFAKE -DGKRELLM2
OBJS2 = i8krellm.o

# seems to barf without the @echo....
i8krellm: i8krellm.so
	@echo ""

i8krellm.so: $(OBJS2)
	$(CC2) $(OBJS2) -o i8krellm.so $(LFLAGS) $(LIBS2)

fake: i8krellm.c prop-anim.xpm
	$(CC2F) -c -o i8krellm.o i8krellm.c
	$(CC2F) $(OBJS2) -o i8krellm.so $(LFLAGS) $(LIBS2)

i8krellm.o: i8krellm.c prop-anim.xpm
	$(CC2) -c -o i8krellm.o i8krellm.c

site_install: i8krellm.so
	install -c -s -m 644 i8krellm.so $(SINSTALLDIR2)

user_install: i8krellm.so
	install -c -s -m 644 i8krellm.so $(UINSTALLDIR2)

install: i8krellm.so
	install -c -s -m 644 i8krellm.so $(UINSTALLDIR2)

clean:
	rm -f *.o core *.so


## Support for GKrellM 1.2
#
GTK_INCLUDE = `gtk-config --cflags`
GTK_LIB = `gtk-config --libs` -lpthread
IMLIB_INCLUDE = `imlib-config --cflags-gdk`
IMLIB_LIB = `imlib-config --libs-gdk`
SINSTALLDIR = /usr/share/gkrellm/plugins
UINSTALLDIR = $(HOME)/.gkrellm/plugins
FLAGS = -O2 -Wall -fPIC -I. $(GTK_INCLUDE) $(IMLIB_INCLUDE)
LIBS = $(GTK_LIB) $(IMLIB_LIB)
CC = gcc $(FLAGS)
CCF = gcc $(FLAGS) -DFAKE
OBJS = i8krellm.o

i8krellm1.so: $(OBJS)
#	@echo "========> i8krellm1.so"
	$(CC) $(OBJS) -o i8krellm1.so $(LFLAGS) $(LIBS)

i8krellm1.o: i8krellm.c prop-anim.xpm
#	@echo "========> i8krellm1.o"
	$(CC) -c -o i8krellm1.o i8krellm.c

fake1: i8krellm.c prop-anim.xpm
	$(CCF) -c -o i8krellm1.o i8krellm.c
	$(CCF) $(OBJS) -o i8krellm1.so $(LFLAGS) $(LIBS)

site_install1: i8krellm1.so
	install -c -s -m 644 i8krellm1.so $(SINSTALLDIR)

user_install1: i8krellm1.so
	install -c -s -m 644 i8krellm1.so $(UINSTALLDIR)

install1: i8krellm1.so
	install -c -s -m 644 i8krellm1.so $(UINSTALLDIR)

