# makefile VS-PLUS lib
# AHKeli
# 24.02.2014 PHe
# 06.03.2014 juc

# Definitionen für das mkmf (/opt) Skript
# mkmf produziert makedepend.dep
# -> Abhängigkeiten und Regeln zur Kompilation der Object-Dateien
SHELL		= /bin/sh
MKMF		= /opt/mkmf
MAKEFILE	= makedepend
TARGET		= dep
SOURCES		= -p src/vsplus -p src/oevmod -p src/adapt

# Quelldateien in den Verzeichnissen unter src auslesen
# Object-Datei Ziele definieren
SRCDIRS		= src/vsplus src/oevmod src/adapt
OBJDIR		= obj
CFILES		= $(foreach source, $(SRCDIRS), \
			$(notdir $(wildcard $(source)/*.c)))
OFILES		:= $(CFILES:.c=.o)
OBJS		= $(patsubst %,$(OBJDIR)/%,$(OFILES))

# Gesamtziel
TARGETNAME	= vsp_core
PROGRAM		= lib/$(TARGETNAME).lib

# Kompilereinstellungen
CROSS		= 
CC		= $(CROSS)gcc
CFLAGS		= -Os -fPIC -Wall

# Includes und spezielle Flaggen
INCLUDES	= -I. -I./src/adapt -I./src/oevmod -I./src/vsplus
VSP_FLAGS	= -D_ADVANCED_MEMORY_ -D_HELPER_MACROS_

# Programm:
# make clean -> Objects, Error-Log und VSP lib löschen
# make depend -> Dependencies und Kompilationsregeln nach makedepend.dep schreiben
# make -> Object-Dateien kompilieren und lib Archiv packen
all:$(PROGRAM)

$(PROGRAM): $(OBJS)
		@echo "Erstelle Archiv $(PROGRAM) ..."
		ar -rsv $(PROGRAM) $?
		@echo "Archiv erstellt"

# Beispiel-Regel: $(CC) $(CFLAGS) $(INCLUDES) $(VSP_FLAGS) -c src/adapt/vsa_schnitt_general.c -o $@ 2>> $(OBJDIR)/error.txt
depend:
	@$(SHELL) -c "export TARGET='$(TARGET)' ; export CFLAGS='$(CFLAGS)'; export INCLUDES='$(INCLUDES)' ; export VSP_FLAGS='$(VSP_FLAGS)'; $(MKMF) -f $(MAKEFILE) $(SOURCES) -o $(OBJDIR)"

clean:
	@echo "Loesche Object-Dateien ..."
	rm -f $(OBJDIR)/*.o
	@echo "Loesche Error-Log ..."
	rm -f $(OBJDIR)/error.txt
	@echo "Loesche Archiv ..."
	rm -f $(PROGRAM)

# Abhängikeiten und Regeln einbinden
-include $(MAKEFILE).dep
