CXXFLAGS=-std=c++11 -I../ext/gtest/include -I../ext/gtest
LDFLAGS=-pthread
PROGRAMS=format_test.out buffer_test.out reader_test.out writer_test.out

all: gtest_main.o gtest-all.o $(PROGRAMS)

test: all
	./format_test.out
	./buffer_test.out
	./reader_test.out
	./writer_test.out

buffer_test.out: ../buffer.o

reader_test.out: ../reader.o ../buffer.o

writer_test.out: ../writer.o ../buffer.o

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
