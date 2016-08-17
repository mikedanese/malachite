CC    ?= clang
CXX   ?= clang++

CDEBUG = -g -Wall

CXXDEBUG = -g -Wall

CSTD = -std=c99
CXXSTD = -std=c++11

CFLAGS = -O0  $(CDEBUG) $(CSTD)
CXXFLAGS = -O0  $(CXXDEBUG) $(CXXSTD)

OBJS :=


maizie: vm.cc $(OBJS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(OBJS)

%.o: %.c
	$(CXX)  $(CXXFLAGS) -c $< -o $@

.PHONY += clean
clean:
	rm -rf $(CLEANLIST)

fmt: *.cc *.h
	clang-format-3.8 $? -i
.PHONY += fmt

test: maizie
	./$<
.PHONY += test
