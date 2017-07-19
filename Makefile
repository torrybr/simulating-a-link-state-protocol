# Tyrus Malmstrom :: Sam Kramer :: Torry Brelsford
# Makefile for P3 assignment
# CS457 -> Computer Networks and the Internet

CXX		= g++
OBJS		= manager.o router.o
OBJ1		= manager.o
OBJ2		= router.o
DEBUG		= -g
CPPFLAGS	= -Wall -std=c++11 -I. -c $(DEBUG)

LDFLAGS		= -Wall $(DEBUG) # order of targets actualy matter

EXE             = Tyrus_Malmstrom_Sam_Kramer_Torry_Brelsfold_P3

EXE1            = manager
EXE2            = router

all: clean manager router   # by default, make my makefile clean up the project directory, just to be safe.

manager: $(OBJ1)
	$(CXX) $(LDFLAGS) $(OBJ1) -o $(EXE1)
	@echo "-- Success!"

router: $(OBJ2)
	$(CXX) $(LDFLAGS) $(OBJ2) -o $(EXE2)
	@echo "-- Success!"


manager.o: manager.cpp manager.h project3.h
	@echo "-- Compiling manager..."
	$(CXX) $(CPPFLAGS) manager.cpp

router.o: router.cpp router.h project3.h
	@echo "-- Compiling router..."
	$(CXX) $(CPPFLAGS) router.cpp

clean:
	@echo "Cleaning up project directory ..."
	rm -f $(OBJS) *~ manager router

package:
	@echo "Packaging up project directory..."
	tar -cvf $(EXE).tar *.cpp *.h Makefile
