TARGET = unimain

OBJ_DIR := obj
SUFFIX := c cpp
EXCLUDE_DIRS := $(OBJ_DIR)

SRC_DIRS := $(shell find . -maxdepth 1 -type d)
SRC_DIRS := $(patsubst ./%, %, $(SRC_DIRS))
SRC_DIRS := $(filter-out $(EXCLUDE_DIRS), $(SRC_DIRS))
#$(warning  $(SRC_DIRS))

SRC := $(foreach dir, ${SRC_DIRS},			\
$(foreach fix, ${SUFFIX}, $(wildcard ${dir}/*.${fix}))	\
)
SRC := $(patsubst ./%, %, $(SRC))
#$(warning  $(SRC))

OBJS := $(foreach fix, ${SUFFIX}, $(SRC:%.${fix}=${OBJ_DIR}/%.o))
OBJS := $(filter-out $(SRC), $(OBJS))
#$(warning  $(OBJS))

CFLAGS += -I../                      \
          -I../inc/                  \

CFLAGS += $(foreach dir, ${SRC_DIRS}, -I${dir}/)


LDFLAGS += -L./                      \
           -L../out/                 \
           -lpthread                 \
           -luniapi


default: first ${OBJS} ${TARGET}
	echo "do nothing"

clean:
	rm -rf $(TARGET) obj ../out/$(TARGET)

$(TARGET): $(OBJS)
	g++ $(OBJS) -o $@ $(LDFLAGS)
	cp -af $(TARGET) ../out/

first:
	for dir in ${SRC_DIRS}; do mkdir -p ${OBJ_DIR}/$$dir; done

${OBJ_DIR}/%.o: %.cpp
	g++ $(CFLAGS) -c $< -o $@

${OBJ_DIR}/%.o: %.c
	gcc $(CFLAGS) -c $< -o $@
