WX_CONFIG := wx-config
WX_CXXFLAGS := $(shell $(WX_CONFIG) --cxxflags)
WX_LIBS := $(shell $(WX_CONFIG) --libs)
SQLITE := -lsqlite3

APPLICATION := cashRegister
TESTS := taxCalculation_test
TESTS_OBJECTS := $(addsuffix .o,TESTS)
APPLICATION_OBJECTS:= MainFrame.o App.o SQLiteDB.o TaxCalculation.o
OBJECTS := $(APPLICATION_OBJECTS) catch_amalgamated.o

all: $(APPLICATION) $(TESTS)

$(APPLICATION): $(APPLICATION_OBJECTS)
	$(CXX) -o $@ $(APPLICATION_OBJECTS) $(LDFLAGS) $(WX_LIBS) $(LIBS) $(SQLITE)

$(OBJECTS): %.o: %.cpp %.hpp
	$(CXX) -c -o $@ $(WX_CXXFLAGS) $(CXXFLAGS) $<

$(TESTS_OBJECTS): %.o: %.cpp
	$(CXX) -c -o $@ $(WX_CXXFLAGS) $(CXXFLAGS) $<

# missing dependency issue
taxCalculation_test: taxCalculation_test.o TaxCalculation.o catch_amalgamated.o
	$(CXX) -o $@ $(LDFLAGS) $^

.PHONY: clean
clean:
	find . -name '*~' -o -name '*.o' -o -name $(APPLICATION) | xargs rm


