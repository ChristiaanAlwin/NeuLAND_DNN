CPPFLAGS:=
CFLAGS:=$(shell root-config --cflags) -I ../src/include
LDFLAGS:=-L ../src/
LIBS:=$(shell root-config --libs) $(shell pkg-config freetype2 --libs)

######################################################################
MACRO1:=Design_Network

.PHONY: all
all: $(MACRO1)

$(MACRO1): $(MACRO1).o
	g++ -o $@ $^ $(LIBS) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -lfann -lm

$(MACRO1).o: $(MACRO1).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS) $(LIBS) $(LDFLAGS) -lfann -lm
	
MACRO2:=Train_Network

.PHONY: all
all: $(MACRO2)

$(MACRO2): $(MACRO2).o
	g++ -o $@ $^ $(LIBS) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -lfann -lm

$(MACRO2).o: $(MACRO2).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS) $(LIBS) $(LDFLAGS) -lfann -lm
	
MACRO3:=Validate_Network

.PHONY: all
all: $(MACRO3)

$(MACRO3): $(MACRO3).o
	g++ -o $@ $^ $(LIBS) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -lfann -lm

$(MACRO3).o: $(MACRO3).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS) $(LIBS) $(LDFLAGS) -lfann -lm
	
MACRO4:=SimpleTrain

.PHONY: all
all: $(MACRO4)

$(MACRO4): $(MACRO4).o
	g++ -o $@ $^ $(LIBS) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -lfann -lm

$(MACRO4).o: $(MACRO4).cpp Makefile
	g++ -c -o $@ $< $(CPPFLAGS) $(CFLAGS) $(LIBS) $(LDFLAGS) -lfann -lm

.PHONY: clean
clean:
	rm -f *.o *_C.* $(MACRO1) $(MACRO2) $(MACRO3) $(MACRO4)
