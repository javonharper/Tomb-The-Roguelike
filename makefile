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
	$(OBJDIR)/enemy.o\
	$(OBJDIR)/functions.o\
	$(OBJDIR)/interface.o\
	$(OBJDIR)/item.o\
	$(OBJDIR)/main.o\
	$(OBJDIR)/map.o\
	$(OBJDIR)/player.o\
	$(OBJDIR)/random.o\
	$(OBJDIR)/world.o\

all : The_Tomb

The_Tomb: $(CPP_OBJS)
	$(CPP) $(CPP_OBJS) -o $@ -L${LIBTCODDIR} $(LIBS) -Wl,-rpath=$(LIBDIR)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CPP) $(CFLAGS) -o $@ -c $<

run:
	./The_Tomb

rebuild: clean The_Tomb

clean :
	rm -f $(CPP_OBJS) The_Tomb
	rm -fr obj/*