BIN = ./
HTMLFILE = src/index.html

PATH_SDL_LINUX = /usr/include/SDL2

$(BIN):
	mkdir -p $@

web-release: $(BIN)
	rm -f $(BIN)/index.html
	cp -u $(HTMLFILE) $(BIN)
	emcc --bind \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-o $(BIN)/index.js \
	src/main.cpp \
	-sALLOW_MEMORY_GROWTH \
	--preload-file assets/ \
	--use-preload-plugins \
	-s SDL2_IMAGE_FORMATS='["png", "bmp", "jpg"]' 

linux-release: $(BIN)
	gcc src/main.cpp \
	-lSDL2 \
	-lSDL2_image \
	-lm \
	-lstdc++ \
	-o main

all: $(BIN)	web-release