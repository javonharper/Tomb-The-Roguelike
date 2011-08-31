SRCDIR=src
OBJDIR=obj
LIBDIR=lib
LIBTCODDIR=lib
INCDIR=${LIBTCODDIR}/include
FLAGS=-Wall -g
CFLAGS=$(FLAGS) -I$(INCDIR) -I$(SRCDIR)
CPP=g++
LIBS=-ltcod -ltcodxx


.SUFFIXES: .o .h .c .hpp .cpp

CPP_OBJS=\
	$(OBJDIR)/actor.o\
	$(OBJDIR)/controller.o\
	$(OBJDIR)/class.o\
	$(OBJDIR)/enemy.o\
	$(OBJDIR)/functions.o\
	$(OBJDIR)/interface.o\
	$(OBJDIR)/inventory.o\
	$(OBJDIR)/item.o\
	$(OBJDIR)/main.o\
	$(OBJDIR)/map.o\
	$(OBJDIR)/player.o\
	$(OBJDIR)/random.o\
	$(OBJDIR)/world.o\

all: tomb 

tomb: make_obj $(CPP_OBJS)
	$(CPP) $(CPP_OBJS) -o $@ -L${LIBTCODDIR} $(LIBS) -Wl,-rpath=$(LIBDIR)

make_obj:
	rm -fr obj
	mkdir obj

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

run: tomb
	./tomb

rebuild: clean tomb

clean :
	rm -f $(CPP_OBJS) tomb
	rm -fr obj
