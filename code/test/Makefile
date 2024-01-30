CC=gcc
CFLAGS=$(shell pkg-config --cflags glib-2.0 sqlite3) $(shell pkg-config --cflags gmp)
LIBS=$(shell pkg-config --libs glib-2.0 sqlite3) $(shell pkg-config --libs gmp)
BUILDDIR=../build
PROJECTLIBDIR=-L../build/lib
PROJECTLIB=-lvoting
TESTFONCTIONSBDDIR=test_Fonctions_BD
TESTFONCTIONSCOMMANDESDIR=test_Fonctions_Commandes
TESTLIBRAIRIEDIR=test_librairie

$(shell mkdir -p $(BUILDDIR))

all: $(BUILDDIR)/test_electeur $(BUILDDIR)/test_election $(BUILDDIR)/test_vote $(BUILDDIR)/test_CRUD_electeur $(BUILDDIR)/test_CRUD_election $(BUILDDIR)/test_CRUD_vote $(BUILDDIR)/test_crypto $(BUILDDIR)/test_sqlite

$(BUILDDIR)/test_electeur: $(TESTFONCTIONSBDDIR)/test_electeur.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_election: $(TESTFONCTIONSBDDIR)/test_election.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_vote: $(TESTFONCTIONSBDDIR)/test_vote.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_CRUD_electeur: $(TESTFONCTIONSCOMMANDESDIR)/test_CRUD_electeur.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_CRUD_election: $(TESTFONCTIONSCOMMANDESDIR)/test_CRUD_election.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_CRUD_vote: $(TESTFONCTIONSCOMMANDESDIR)/test_CRUD_vote.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_crypto: $(TESTLIBRAIRIEDIR)/test_crypto.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

$(BUILDDIR)/test_sqlite: $(TESTLIBRAIRIEDIR)/test_sqlite.c
	$(CC) $< $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $@

clean:
	rm -f $(BUILDDIR)/*
