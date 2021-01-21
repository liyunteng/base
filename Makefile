PROJECT_ROOT ?= $(abspath .)
include $(PROJECT_ROOT)/build/def.mk

SUBDIRS = list log

include $(PROJECT_ROOT)/build/subdir.mk
