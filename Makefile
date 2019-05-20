# This makefile is for Linux building.
# In Windows and MacOS, please use QCreator.

# Unix installation
ifdef DESTDIR
  # Debian uses this
  INSTALLPATH = $(DESTDIR)/usr
  SHAREPATH = $(DESTDIR)/usr/share
else
  INSTALLPATH = /usr/local
  SHAREPATH = /usr/local/share
endif

UNAME=$(shell uname -s)

ifeq (, $(shell which qmake-qt5))
  QMAKE=qmake
else
  QMAKE=qmake-qt5
endif

ifeq ($(UNAME),Linux)
  # Linux
  SELECTQT=QT_SELECT=5
else
  $(error Unknown operating system. This Makefile is for Mac or Linux.)
endif

all: src

clean:
	+rm -rf build

src: prep
	+make -C build

prep:
	mkdir -p build
	( cd build; $(SELECTQT) $(QMAKE) ../src/gpix.pro )

# Unix installation
install: all
	install -d $(INSTALLPATH)/bin
	install build/eln $(INSTALLPATH)/bin/eln

# Tar preparation
tar: all
	git archive -o ../gpix.tar.gz --prefix=gpix/ HEAD

.PHONY: src all clean tar install prep

