SOURCES = ./src/main.cpp

# Icon resource
ICON_RC = icon.rc
ICON_RES = icon.o

# Default target: build & run
default: $(ICON_RES) game.exe
	./game.exe

# Compile icon resource
$(ICON_RES): $(ICON_RC) assets/images/icon.ico
	windres $(ICON_RC) -O coff -o $(ICON_RES)

# Build executable
game.exe: $(SOURCES) $(ICON_RES)
	g++ $(SOURCES) $(ICON_RES) -o game.exe -g -O1 -Wall -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows

# Clean
clean:
	rm -f game.exe $(ICON_RES)
