#!/bin/sh

# script to patch version number into source. Called by gitpkgtool during
# build, is passed these arguments: $project $majorVersion $minorVersion.

PROJECT=$1
MAJORVERSION=$2
MINORVERSION=$3

echo " - Patching Makefile with PROJECT=${PROJECT} MAJORVERSION=${MAJORVERSION} MINORVERSION=${MINORVERSION}"

sed -i -e "s/^PROJECT=.*/PROJECT=${PROJECT}/" Makefile
sed -i -e "s/^MAJORVER=.*/MAJORVER=${MAJORVERSION}/" Makefile
sed -i -e "s/^MINORVER=.*/MINORVER=${MINORVERSION}/" Makefile

sed-i -e "s/^MAJORVERSION \".*\"/MAJORVERSION "${MAJORVERSION}\"/" src/piXPlaneFMCCDU/Version.h 
sed-i -e "s/^MINORVERSION \".*\"/MINORVERSION "${MAJORVERSION}\"/" src/piXPlaneFMCCDU/Version.h
