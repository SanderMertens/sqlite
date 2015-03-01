
TARGET = sqlite
INCLUDE = $(CORTEX_HOME)/dev/include $(CORTEX_HOME)/interface/sqlite/include
LIBS   = lang cortexdev sqliteserializer


include $(CORTEX_HOME)/build/package.makefile

