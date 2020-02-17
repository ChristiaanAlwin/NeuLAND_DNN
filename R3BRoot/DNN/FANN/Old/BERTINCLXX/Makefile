GCC=g++
CFLAGS=-I ../src/include
LDFLAGS=-L ../src/

TARGETS = Training_BERT Training_INCLXX Validation_BERT Validation_INCLXX
DEBUG_TARGETS = 

all: $(TARGETS)

%: %.c Makefile
	$(GCC) $(CFLAGS) $(LDFLAGS) -O3 $< -o $@ -lfann -lm

%_fixed: %.c Makefile
	$(GCC) $(CFLAGS) $(LDFLAGS) -O3 -DFIXEDFANN $< -o $@ -lfixedfann -lm

clean:
	rm -f $(TARGETS) $(DEBUG_TARGETS) xor_fixed.data *.net *~ *.obj *.exe *.tds noscale.txt withscale.txt scale_test_results.txt
