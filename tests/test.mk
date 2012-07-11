
OBJS = $(PROG).o
SRCS = $(PROG).asm

all: $(PROG)

$(PROG): $(OBJS)
	ld -o ${.TARGET} $(OBJS)

$(OBJS): $(SRCS)
	nasm -f elf -o ${.TARGET} $(SRCS)

clean:
	rm -f $(PROG) $(OBJS)
