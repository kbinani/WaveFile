CXXFLAGS=-std=c++1y -stdlib=libc++ -I../ext/gtest/include -I../ext/gtest
LDFLAGS=-lc++
CXX=g++
PROGRAMS=format_test.out buffer_test.out

all: gtest_main.o gtest-all.o $(PROGRAMS)

buffer_test.out: ../buffer.o

gtest_main.o: ../ext/gtest/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

gtest-all.o: ../ext/gtest/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $^ -o $@

.o.out:
	$(CXX) $(LDFLAGS) $^ gtest_main.o gtest-all.o -o $@

clean:
	$(RM) $(PROGRAMS)
