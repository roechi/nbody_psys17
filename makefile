# Generic makefile for C projects with src/ obj/ bin/ folder structure
# Original version: http://stackoverflow.com/questions/7004702/how-can-i-create-a-makefile-for-c-projects-with-src-obj-and-bin-subdirectories
# Modified to use -fopenmp and a gcc6 compiler

# project name (generate executable with this name)
TARGET   = nbody

#  ---- CHANGE THIS TO THE PATH OF THE SYSTEMS GCC-6 COMPILER! ----
PATH_TO_GCC6 = /usr/local/bin/gcc-6

# C compiler and flags
CC       = $(PATH_TO_GCC6)
CFLAGS   = -std=c99 -Wall -I. -fopenmp

# Linker and flags
LINKER   = $(PATH_TO_GCC6)
LFLAGS   = -Wall -I. -lm -fopenmp
LDLIBS	 = -lm

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"

.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
