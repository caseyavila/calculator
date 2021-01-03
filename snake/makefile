# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= DEMO
ICON ?= icon.png
DESCRIPTION ?= "ya know... snake"
COMPRESSED ?= NO
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz -std=c89
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk

