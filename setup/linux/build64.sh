#!/bin/bash

for i in `git tag --list --sort=committerdate`; do BUILDTAG=$i; done

for i in `git rev-parse HEAD`; do BUILDCOMMIT=$i; done
BUILDCOMMIT=${BUILDCOMMIT:0:8}

for i in `git rev-parse --abbrev-ref HEAD`; do BUILDBRANCH=$i; done

echo $BUILDTAG $BUILDCOMMIT $BUILDBRANCH

VERSION=${BUILDTAG:1}

echo "#pragma once" > ../../UnicornExpansion/UnicornExpansion/version.h
echo "const char * VERSION = \"$VERSION\" ;" >> ../../UnicornExpansion/UnicornExpansion/version.h

pushd ../..
./build_gcc.sh
popd

appdir=/tmp/UnicornExpansion.AppDir

rm -rf $appdir

mkdir $appdir
cp appruns/AppRun-x86_64 $appdir/AppRun
chmod 777 $appdir/AppRun
cp ../../graphics/main.png $appdir/UnicornExpansion.png
pushd $appdir
ln -s UnicornExpansion.png .DirIcon
popd

cp UnicornExpansion.desktop $appdir
mkdir $appdir/usr
mkdir $appdir/usr/bin
mkdir $appdir/usr/lib

cp /usr/lib64/libicui18n.so* $appdir/usr/lib
cp /usr/lib64/libicuuc.so* $appdir/usr/lib
cp /usr/lib64/libicudata.so* $appdir/usr/lib
cp /usr/lib64/libvorbis* $appdir/usr/lib
cp /usr/lib64/libopenal.so* $appdir/usr/lib
cp /usr/lib64/libatomic.so* $appdir/usr/lib
cp /usr/lib64/libGLU.so* $appdir/usr/lib
cp /usr/lib64/libogg.so* $appdir/usr/lib
cp /usr/lib64/libFLAC.so* $appdir/usr/lib

cp ../../bin/UnicornExpansion $appdir/usr/bin
cp -r ../../data $appdir/usr

export ARCH=x86_64

appimagetool-x86_64.AppImage $appdir /tmp/UnicornExpansion-$VERSION-x86_64.AppImage
