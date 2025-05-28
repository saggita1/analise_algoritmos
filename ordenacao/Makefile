CC = gcc
CFLAGS = -Wall -O2 -std=c99
SRCDIR = algoritmos
UTILDIR = utils
TARGETS = bubble insertion merge quick gerador benchmark

all: $(TARGETS)

# Compilar algoritmos
bubble: $(SRCDIR)/bubble_sort.c
	$(CC) $(CFLAGS) -o bubble $(SRCDIR)/bubble_sort.c

insertion: $(SRCDIR)/insertion_sort.c
	$(CC) $(CFLAGS) -o insertion $(SRCDIR)/insertion_sort.c

merge: $(SRCDIR)/merge_sort.c
	$(CC) $(CFLAGS) -o merge $(SRCDIR)/merge_sort.c

quick: $(SRCDIR)/quick_sort.c
	$(CC) $(CFLAGS) -o quick $(SRCDIR)/quick_sort.c

# Utilitários
gerador: $(UTILDIR)/gerar_entradas.c
	$(CC) $(CFLAGS) -o gerador $(UTILDIR)/gerar_entradas.c

benchmark: $(UTILDIR)/benchmark.c
	$(CC) $(CFLAGS) -o benchmark $(UTILDIR)/benchmark.c

# Executar testes completos
test: all
	./benchmark

# Gerar apenas entradas
entradas: gerador
	./gerador

# Limpar arquivos compilados
clean:
	rm -f $(TARGETS)
	rm -f entradas/*.txt
	rm -f resultados/*.csv
	rm -f graficos/*.png

.PHONY: all test entradas graficos clean
