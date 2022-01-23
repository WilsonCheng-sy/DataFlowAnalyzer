LLVM_CONFIG=/home/sycheng/llvm_pass/llvm_build/bin/llvm-config

CXX=`$(LLVM_CONFIG) --bindir`/clang
CXXFLAGS=`$(LLVM_CONFIG) --cppflags` -fPIC -fno-rtti
LDFLAGS=`$(LLVM_CONFIG) --ldflags`

all: hw2.so Test hello_pass.so Demo.so
# all: hw2.so Test

hw2.o: hw2.cpp
	$(CXX) -c hw2.cpp $(CXXFLAGS) -o hw2.o

hw2.so: hw2.o
	$(CXX) -shared hw2.o $(LDFLAGS) -fPIC -o hw2.so

hello_pass.o: hello_pass.cpp
	$(CXX) -c hello_pass.cpp $(CXXFLAGS) -o hello_pass.o

hello_pass.so: hello_pass.o
	$(CXX) -shared hello_pass.o $(LDFLAGS) -fPIC -o hello_pass.so

Demo.o: Demo.cpp
	$(CXX) -c Demo.cpp $(CXXFLAGS) -o Demo.o

Demo.so: Demo.o
	$(CXX) -shared Demo.o $(LDFLAGS) -fPIC -o Demo.so

# Test: test1.c test2.c test3.c
# 	$(CXX) -S -fno-discard-value-names -emit-llvm test1.c -o test1.ll
# 	$(CXX) -S -fno-discard-value-names -emit-llvm test2.c -o test2.ll
# 	$(CXX) -S -fno-discard-value-names -emit-llvm test3.c -o test3.ll

Test: foo.c icpp.c icpp2.c icpp3.c
	$(CXX) -S -fno-discard-value-names -emit-llvm foo.c -o foo.ll
	$(CXX) -S -fno-discard-value-names -emit-llvm icpp.c -o icpp.ll
	$(CXX) -S -fno-discard-value-names -emit-llvm icpp2.c -o icpp2.ll
	$(CXX) -S -fno-discard-value-names -emit-llvm icpp3.c -o icpp3.ll

clean:
	rm -f *.o *.ll *.so *.bc
