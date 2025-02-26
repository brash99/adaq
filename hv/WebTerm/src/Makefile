# WebTerm Makefile

# Options to pass to Java compiler.
JAVACOPTS = -O

# The name of this program.
PROGNAME = WebTerm

SRC = src
BIN = bin

# The source Java files for this program.
PROG_SRC = Parameters.java RubberTextField.java Telnet.java Terminal.java \
	Emulator.java EmulatorOptions.java VT200.java WebTerm.java

# The source Java files needed to use the ClassRegistry.
REGISTRY_SRC = ClassConnection.java ClassRegistry.java RegistryButton.java

# The main binary Java files that result from compiling.
PROG_BIN = ../bin/Parameters.class ../bin/RubberTextField.class \
	../bin/Telnet.class ../bin/Terminal.class \
	../bin/Emulator.class ../bin/EmulatorOptions.class \
	../bin/VT200.class ../bin/WebTerm.class

# The main binary Java files for the ClassRegistry.
REGISTRY_BIN = ../bin/ClassConnection.class ../bin/ClassRegistry.class \
	../bin/RegistryButton.class

# The files that go into the binary distribution of the program.
BINDIST = $(PROGNAME)/README* $(PROGNAME)/*.html \
	$(PROGNAME)/bin

# The files that go into the source distribution of the program.
SRCDIST = $(BINDIST) \
	$(PROGNAME)/src/Makefile $(PROGNAME)/src/bumprev \
	$(PROGNAME)/src/.[a-zA-Z]* $(PROGNAME)/src/*.*

SHELL = /bin/sh

BUILD_TAG = $(PROGNAME)__Build

# ------------------

# MAIN TARGET.
# Compile all of the Java files and create the build tag.

std:	$(PROG_BIN) $(REGISTRY_BIN) $(BUILD_TAG).java

# Create build tag: use bumprev to update submake and write build file,
# then compile it.

$(BUILD_TAG).java: $(PROG_BIN)
	@echo '--> Updating build tag: $(BUILD_TAG)'
	@./bumprev -m $(BUILD_TAG)
	@javac $(JAVACOPTS) -d ../bin $(BUILD_TAG).java

# ------------------

# Make the binary distribution.
bin: clean
	@( vstr=""; \
	  if [ -r ".verstring" ]; then \
	    vstr="-`cat .verstring`"; \
	  fi; \
	  tarname="$(PROGNAME)-bin$${vstr}.tar"; \
	  ( echo "Archiving: $${tarname}"; \
	    cd ../..; \
	    tar -cf $(PROGNAME)/$${tarname} $(BINDIST) \
	  ); \
	  echo "Making: $${tarname}.Z"; \
	  compress <../$${tarname} >../ftp/$${tarname}.Z; \
	  echo "Making: $${tarname}.gz"; \
	  gzip <../$${tarname} >../ftp/$${tarname}.gz \
	)

# Make the source distribution.
src: clean
	@( vstr=""; \
	  if [ -r ".verstring" ]; then \
	    vstr="-`cat .verstring`"; \
	  fi; \
	  tarname="$(PROGNAME)-src$${vstr}.tar"; \
	  ( echo "Archiving: $${tarname}"; \
	    cd ../..; \
	    tar -cf $(PROGNAME)/$${tarname} $(SRCDIST) \
	  ); \
	  echo "Making: $${tarname}.Z"; \
	  compress <../$${tarname} >../ftp/$${tarname}.Z; \
	  echo "Making: $${tarname}.gz"; \
	  gzip <../$${tarname} >../ftp/$${tarname}.gz \
	)

# Revision checkpoint.  Check all source file and then check back out.
ckp:
	ci -l Makefile $(PROG_SRC) $(VERSION_FILES)

# Clean the project.  Remove all emacs backup files and build tag file.
clean:
	rm -f *~ ../*~ $(BUILD_TAG).java

# Completely clean the project.  Clean, then remove all binaries.
realclean: clean
	rm -f ../bin/*

# Clean the registry classes.  Real clean, then delete registry source.
regclean: realclean
	rm $(REGISTRY_SRC)

registry:
	(cd ../../ClassRegistry/src; make)
	cp ../../ClassRegistry/src/ClassConnection.java .
	cp ../../ClassRegistry/src/ClassRegistry.java .
	cp ../../ClassRegistry/src/RegistryButton.java .

../bin/Parameters.class: Parameters.java
	javac $(JAVACOPTS) -d ../bin Parameters.java

../bin/RubberTextField.class: RubberTextField.java
	javac $(JAVACOPTS) -d ../bin RubberTextField.java

../bin/Telnet.class: Telnet.java
	javac $(JAVACOPTS) -d ../bin Telnet.java

../bin/Terminal.class: Terminal.java
	javac $(JAVACOPTS) -d ../bin Terminal.java

../bin/Emulator.class: Emulator.java
	javac $(JAVACOPTS) -d ../bin Emulator.java

../bin/EmulatorOptions.class: EmulatorOptions.java
	javac $(JAVACOPTS) -d ../bin EmulatorOptions.java

../bin/VT200.class: VT200.java
	javac $(JAVACOPTS) -d ../bin VT200.java

../bin/WebTerm.class: WebTerm.java
	javac $(JAVACOPTS) -d ../bin WebTerm.java

../bin/ClassConnection.class: ClassConnection.java
	javac $(JAVACOPTS) -d ../bin ClassConnection.java

../bin/ClassRegistry.class: ClassRegistry.java
	javac $(JAVACOPTS) -d ../bin ClassRegistry.java

../bin/RegistryButton.class: RegistryButton.java
	javac $(JAVACOPTS) -d ../bin RegistryButton.java
