#!/bin/bash

# Copyright (c) 2023 Alex313031

YEL='\033[1;33m' # Yellow
CYA='\033[1;96m' # Cyan
RED='\033[1;31m' # Red
GRE='\033[1;32m' # Green
c0='\033[0m' # Reset Text
bold='\033[1m' # Bold Text
underline='\033[4m' # Underline Text

# Error handling
yell() { echo "$0: $*" >&2; }
die() { yell "$*"; exit 111; }
try() { "$@" || die "${RED}Failed $*"; }

# --help
displayHelp () {
	printf "\n" &&
	printf "${bold}${GRE}Script to build mpv on Linux.${c0}\n" &&
	printf "${bold}${YEL}Use the --deps install build deps.${c0}\n" &&
	printf "${bold}${YEL}Use the --clean flag to clean configure & build artifacts.${c0}\n" &&
	printf "${bold}${YEL}Use the --build flag to build with AVX.${c0}\n" &&
	printf "${bold}${YEL}Use the --sse4 flag to build with SSE4.1.${c0}\n" &&
	printf "${bold}${YEL}Use the --help flag to show this help.${c0}\n" &&
	printf "\n"
}
case $1 in
	--help) displayHelp; exit 0;;
esac

installDeps () {

}
case $1 in
	--deps) installDeps; exit 0;;
esac

wafClean () {
export VERBOSE=1 &&
./waf clean &&
./waf distclean
}
case $1 in
	--clean) wafClean; exit 0;;
esac

buildMpv () {
export CFLAGS="-pipe -DNDEBUG -O3 -mavx -maes -s -g0 -flto" &&
export CPPFLAGS="-pipe -DNDEBUG -O3 -mavx -maes -s -g0 -flto" &&
export CXXFLAGS="-pipe -DNDEBUG -O3 -mavx -maes -s -g0 -flto" &&
export LDFLAGS="-Wl,-O3 -mavx -maes -flto" &&
export VERBOSE=1 &&

./waf configure --enable-libmpv-shared --enable-optimize --disable-debug-build --enable-vector --verbose &&
./waf -v
case $1 in
	--build) buildMpv; exit 0;;
esac

buildSSE4 () {
export CFLAGS="-pipe -DNDEBUG -O3 -sse4.1 -s -g0 -flto" &&
export CPPFLAGS="-pipe -DNDEBUG -O3 -sse4.1 -s -g0 -flto" &&
export CXXFLAGS="-pipe -DNDEBUG -O3 -sse4.1 -s -g0 -flto" &&
export LDFLAGS="-Wl,-O3 -sse4.1 -flto" &&
export VERBOSE=1 &&

./waf configure --enable-libmpv-shared --enable-optimize --disable-debug-build --verbose &&
./waf -v
case $1 in
	--sse4) buildSSE4; exit 0;;
esac

printf "\n" &&
printf "${bold}${GRE}Script to build mpv on Linux.${c0}\n" &&
printf "${bold}${YEL}Use the --deps install build deps.${c0}\n" &&
printf "${bold}${YEL}Use the --clean flag to clean configure & build artifacts.${c0}\n" &&
printf "${bold}${YEL}Use the --build flag to build with AVX.${c0}\n" &&
printf "${bold}${YEL}Use the --sse4 flag to build with SSE4.1.${c0}\n" &&
printf "${bold}${YEL}Use the --help flag to show this help.${c0}\n" &&
printf "\n"

exit 0


