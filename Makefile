CC    ?= clang
CXX   ?= clang++

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++11

CFLAGS = -O0  $(CDEBUG) $(CSTD)
CXXFLAGS = -O0  $(CXXDEBUG) $(CXXSTD)

OBJS := serp_parse.o serp_lex.o

CLEANLIST = **/*.o \
	location.hh \
	serp_parse.hh \
	serp_parse.output \
	position.hh \
	stack.hh \
	serp_parse.cc \
	serp_lex.cc \
	serp


serp: cmd.cc $(OBJS) *.h
	$(CXX) $(CXXFLAGS) $< -o $@ $(OBJS)

serp_parse.cc serp_parse.hh: serp.yy
	bison -d -v $< -o $@

serp_lex.cc: serp.l serp_parse.hh
	flex --outfile=$@  $<

%.o: %.c
	$(CXX)  $(CXXFLAGS) -c $< -o $@

.PHONY += clean
clean:
	rm -rf $(CLEANLIST)

fmt: *.cc *.h
	clang-format-3.7 $? -i
.PHONY += fmt

test: serp
	./$< < test.srp
.PHONY += test
