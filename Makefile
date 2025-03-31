default: build

BIN = ./
HTMLFILE = src/index.html

PATH_SDL_LINUX = /usr/include/SDL2

$(BIN):
	mkdir -p $@

ASSET_FILES := $(shell find assets -name '*.png')

regen-assets:
	rm -r cache_assets
	mkdir -p cache_assets
	for i in $(ASSET_FILES); do \
		convert -flip $$i cache_$$i; \
		convert -flop cache_$$i cache_$$i; \
		convert -resize 320x320 cache_$$i cache_$$i; \
	done

build: $(BIN) regen-assets
	rm -f $(BIN)/index.*
	cp -u $(HTMLFILE) $(BIN)
	emcc --bind \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-o $(BIN)/index.js \
	src/main.cpp \
	-sEXIT_RUNTIME \
	-sEXPORTED_FUNCTIONS=_main,_close_embed \
	-sEXPORTED_RUNTIME_METHODS=ccall,cwrap \
	-s SDL2_IMAGE_FORMATS='["png"]' \
	--preload-file cache_assets/ \
	--use-preload-plugins 

all: $(BIN)	build