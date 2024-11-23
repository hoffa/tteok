.PHONY: build run

build:
	emcc src/app.c -s USE_SDL=2 -s USE_SDL_TTF=2 -o docs/index.html --preload-file src/assets/font.ttf --shell-file build/shell.html

run:
	emrun docs/index.html
