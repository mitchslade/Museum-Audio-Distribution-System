# Makefile for combined files for museum application
# created by P. Cadd 25/05/2015
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'server'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS = -Wall -g `pkg-config gstreamer-0.10 --cflags`

# define any directories containing header files other than /usr/include
INCLUDES = -I/usr/include/mysql

# define library paths in addition to /usr/lib
#   If you want to include libraries not in /usr/lib then specify
#   their path using -Lpath, something like:
LFLAGS = -L/home/gbcadp/museuminfo  -L../lib

# define any libraries to link into executable:
#   If you want to link in libraries (libx.so or libx.a) Then use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -pthread `pkg-config gstreamer-0.10 --libs` -L/usr/lib/mysql -lmysqlclient

# define the C source files forthe complete application
SRCS = server.c streaming.c museumDB.c

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.c=.o)

# define the completed executable file 
MAIN = server

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
		@echo  Simple compiler named server has been compiled

$(MAIN): $(OBJS) 
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
