# Compiler Options
COMPILER:= gcc
CFLAGS:=-Wall

# Code Architecture
OUTDIR:=bin
OBJDIR:=obj
SRCDIR:=src
OUTFILE:=PopulationSimulation
SRCFILES:=$(wildcard $(SRCDIR)/*.c)
OBJFILES:=$(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCFILES))

all: compile clean

compile: $(OBJFILES)
	$(COMPILER) $^ -o $(OUTDIR)/$(OUTFILE) $(CFLAGS)

clean:
	rm $(OBJFILES)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(COMPILER) -I$(SRCDIR) -c $< -o $@ $(CFLAGS)