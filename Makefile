default: build

BIN = ./
HTMLFILE = src/index.html

PATH_SDL_LINUX = /usr/include/SDL2

$(BIN):
	mkdir -p $@

build: $(BIN)
	rm -f $(BIN)/index.*
	cp -u $(HTMLFILE) $(BIN)
	emcc --bind \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-o $(BIN)/index.js \
	src/main.cpp \
	-sEXIT_RUNTIME \
	-s SDL2_IMAGE_FORMATS='["png", "bmp", "jpg"]' \
	--preload-file assets/ \
	--use-preload-plugins 

all: $(BIN)	build