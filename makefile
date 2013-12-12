include Makefile_public

TARGET	= $(DEBUG_TARGET)

DEBUG_TARGET = $(LIB_DIR)/libnetevent.a

SRC = $(wildcard *.cpp)
OBJS = $(addprefix $(OBJ_DIR)/,$(subst .cpp,.o,$(SRC)))

INC = -I$(NETEVENT_INCLUDE_DIR)

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $?

$(OBJ_DIR)/%.o : %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJS) $(TARGET)
