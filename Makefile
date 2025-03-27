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
	-s SDL2_IMAGE_FORMATS='["png"]' \
	--preload-file assets/ \
	--use-preload-plugins \

all: $(BIN)	web-release