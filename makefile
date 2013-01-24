CC=g++
CFLAGS=
OBJS = ./Math/Matrix.o \
	./Math/Vector.o \
	./GameWorld.o \
	./Ship/ItemsDB.o \
	./Ship/Person.o \
	./Ship/Ship.o \
	./Renderer.o \
	./auxFuncs.o \
	./StdAfx.o \
	./FileTools/RoomParser.o \
	./Generic/CString.o \
	./SGame.o \
	./GUI/TexturedWidget.o \
	./GUI/CompHoverInfo.o \
	./GUI/CompartmentButton.o \
	./GUI/ProgressBar.o \
	./GUI/Input.o \
	./GUI/TextArea.o \
	./GUI/Forms/TopPanel.o \
	./GUI/Forms/FileOpenDialog.o \
	./GUI/Forms/GameWindow.o \
	./GUI/Forms/BuildTools.o \
	./GUI/Forms/ShipEditor.o \
	./GUI/Forms/BuildInfo.o \
	./GUI/Forms/Credits.o \
	./GUI/Button.o \
	./GUI/GlobalWindow.o \
	./GUI/GUIManager.o \
	./GUI/Text.o \
	./GUI/ShipView/ShipViewGame.o \
	./GUI/ShipView/ShipView.o \
	./GUI/ShipView/ShipViewEditor.o \
	./GUI/Window.o \
	./Logger.o \

INCS = -I. -IMath -IShip -IFileTools -IGeneric -IGUI -IGUI/Forms -IGUI/ShipView
CFLAGS = $(INCS) -g -std=c++0x
LDFLAGS =-lSDL -lGL -lSDL_image

headers:
	$(CC) $(CFLAGS) -c StdAfx.h -o StdAfx.h.gch

all: headers $(OBJS)
	$(CC) $(OBJS) -o SGame $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	rm -f SGame
	find ./ -name "*.o" -exec rm -rf {} \;
	rm -f StdAfx.h.gch
