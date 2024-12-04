CXXFLAGS	+= -I./sources/vendors/mosquittopp/exam

VPATH += sources/vendors/mosquittopp/exam

OBJ += $(OBJ_DIR)/mosqsupscribe.o
OBJ += $(OBJ_DIR)/mosqpublish.o
