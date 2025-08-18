COMPIL = gcc
CFLAGS = -O3 -mavx2 -msse2 -Iinc
LFLAGS = -lpthread
SOURCE = vi_core.c vi_line.c vi.c
OBJECS = $(SOURCE:.c=.o)
OUTPUT = vi

all: $(OUTPUT)

$(OUTPUT): $(OBJECS)
	$(COMPIL) $(OBJECS) -o $(OUTPUT) $(LFLAGS)

%.o: %.c
	$(COMPIL) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECS) $(OUTPUT)