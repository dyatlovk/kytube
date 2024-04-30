ifneq (,$(wildcard ./.env))
    include .env
    export
endif

# Mute all `make` specific output. Comment this out to get some debug information.
.SILENT:

# .DEFAULT: If the command does not exist in this makefile
.DEFAULT: help

help:
	@echo "Usage:"
	@echo "     make [command]"
	@echo
	@echo "Available commands:"
	@grep '^[^#[:space:]].*:' Makefile | grep -v '^default' | grep -v '^\.' | grep -v '=' | grep -v '^_' | sed 's/://' | xargs -n 1 echo ' -'

########################################################################################################################

cmake-debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -S . -B ./cmake-build-debug
cmake-release:
	cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -S . -B ./cmake-build-release
compile:
	cmake --build ./build --target YouTubeQt -- -j 6
compile-test:
	cmake --build ./build --target qtest -- -j 6
run-test:
	./build/test/qtest
run:
	./build/YouTubeQt
