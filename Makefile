# Project Name
TARGET = Chord

# Sources
CPP_SOURCES = Chord.cpp

# Library Locations
LIBDAISY_DIR = ../../Daisy/libDaisy/
DAISYSP_DIR = ../../Daisy/DaisySP/

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
