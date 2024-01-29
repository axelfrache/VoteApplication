CC=gcc
CFLAGS=`pkg-config --cflags glib-2.0 sqlite3`
LIBS=`pkg-config --libs glib-2.0 sqlite3` -lsqlite3 
AR=ar
ARFLAGS=rcs
BUILDDIR=../build
SRCDIR=.
LIBDIR=$(BUILDDIR)/lib
OBJDIR=$(BUILDDIR)/obj
LIBNAME=libvoting.a
DEPS=main_Serveur.c bd.c serveur_vote.c util.c

# Liste de tous les fichiers source
SRC := $(wildcard $(SRCDIR)/*.c)

# Conversion des fichiers source en .o
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Crée les répertoires de build si nécessaire
$(shell mkdir -p $(LIBDIR) $(OBJDIR))

# Cible par défaut
all: $(LIBDIR)/$(LIBNAME)
	
main: $(DEPS)
	gcc -o $(BUILDDIR)/main main_Serveur.c bd.c serveur_vote.c util.c `pkg-config --cflags --libs glib-2.0` `pkg-config --libs sqlite3`

# Règle pour construire la bibliothèque
$(LIBDIR)/$(LIBNAME): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

# Règle pour construire les fichiers objet
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour nettoyer le projet
clean:
	rm -rf $(BUILDDIR)

.PHONY: all clean
