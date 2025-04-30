.PHONY: all test example init vars clean install uninstall dist

PACKAGE  = stz
VERSION  = 0.0.1

lib_libs   = -ltree-sitter
prefix     = /usr/local
extra_dist =  Makefile README.md

CC       =  clang
CFLAGS   =  -std=c23 -Wall -pedantic -O0 -g
INCLUDES = -Iinclude
LDFLAGS  =

CFLAGS  += -DVERSION=\"$(VERSION)\"

headers    = $(wildcard include/*.h) $(wildcard include/utils/*.h)
lib_hdr    = $(wildcard src/*.h) $(wildcard src/utils/*.h)
lib_src    = $(wildcard src/*.c) $(wildcard src/utils/*.c)

test_libs  = $(lib_libs)
test_src   = $(wildcard test/*.c)

example_libs = $(lib_libs) -l$(PACKAGE)
example_src  = $(wildcard example/*.c)

dist_files = $(extra_dist) $(headers) $(lib_hdr) $(lib_src) $(test_src) $(example_src)

LIBRARY  = build/lib$(PACKAGE).a
OBJS     = $(patsubst src/%.c, build/obj/%.o, $(lib_src))
TESTS    = $(patsubst test/%.c, build/test/%, $(test_src))
EXAMPLES = $(patsubst example/%.c, build/example/%, $(example_src))

all: $(LIBRARY) test

test: $(TESTS)

example: $(EXAMPLES)

init:
	mkdir -p include
	mkdir -p src
	mkdir -p test
	mkdir -p example
	mkdir -p build/obj
	mkdir -p build/obj/utils
	mkdir -p build/test
	mkdir -p build/example

vars:
	@echo "LIBRARY  : " $(LIBRARY)
	@echo "OBJS     : " $(OBJS)
	@echo "TESTS    : " $(TESTS)
	@echo "EXAMPLES : " $(EXAMPLES)
	@echo ""
	@echo "lib_libs : " $(lib_libs)
	@echo "headers  : " $(headers)
	@echo "lib_src  : " $(lib_src)
	@echo "lib_hdr  : " $(lib_hdr)
	@echo ""
	@echo "test_libs : " $(test_libs)
	@echo "test_src  : " $(test_src)
	@echo ""
	@echo "example_libs : " $(example_libs)
	@echo "example_src  : " $(example_src)

clean:
	rm -f build/obj/*.o $(LIBRARY) $(TESTS) $(EXAMPLES)
	rm -r -f build/lib build/include

install: $(prefix)/lib/lib$(PACKAGE).a

$(prefix)/lib/lib$(PACKAGE).a: $(LIBRARY)
	mkdir -p $(prefix)/include/$(PACKAGE) && cp $(headers) $(prefix)/include/$(PACKAGE)
	mkdir -p $(prefix)/lib && cp $(LIBRARY) $(prefix)/lib

uninstall:
	rm -r $(prefix)/include/$(PACKAGE)
	rm $(prefix)/lib/$(LIBRARY)

$(LIBRARY): $(OBJS)
	ar rcs $(LIBRARY) $(OBJS)

build/obj/%.o : src/%.c include/%.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/test/% : test/%.c $(LIBRARY)
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) $^ $(test_libs) -o $@

build/example/% : example/%.c $(prefix)/lib/lib$(PACKAGE).a
	$(CC) $(CFLAGS) -I$(prefix)/include -L$(prefix)/lib $(LDFLAGS) $^ $(example_libs) -o $@

dist:
	mkdir $(PACKAGE)-$(VERSION)
	cp --parents $(dist_files) $(PACKAGE)-$(VERSION)
	tar -czvf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	rm -rf $(PACKAGE)-$(VERSION)
