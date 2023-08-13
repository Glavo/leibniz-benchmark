#!/usr/bin/env bash

BASE_DIR=$(realpath "$(dirname $0)")

BUILD_DIR="$BASE_DIR/build"
SRC_DIR="$BASE_DIR/src"

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

cp "$SRC_DIR/rounds.txt" $BUILD_DIR

CXX="${CXX:-g++}"

echo '========= C++ ========='
$CXX -O3 -march=native -o leibniz_cxx $SRC_DIR/leibniz.cpp
./leibniz_cxx
echo

echo '========= C++ (AVX2) ========='
$CXX -O3 -march=native -o leibniz_cxx_avx2 $SRC_DIR/leibniz_avx2.cpp
./leibniz_cxx_avx2
echo

echo '========= Java ========='
java $SRC_DIR/leibniz.java
echo

echo '========= Java (AVX2) ========='
java --source 20 --enable-preview --add-modules=jdk.incubator.vector $SRC_DIR/leibniz_avx2.java
echo

echo '========= Golang ========='
go run $SRC_DIR/leibniz.go
echo

echo '========= Python (CPython) ========='
python3 $SRC_DIR/leibniz.py
echo

echo '========= Python (GraalPython) ========='
$GRAALVM_HOME/bin/python3 --jvm $SRC_DIR/leibniz.py
echo

echo '========= Python (PyPy) ========='
pypy3 $SRC_DIR/leibniz.py
echo

echo '========= JavaScript (V8) ========='
js $SRC_DIR/leibniz.js
echo

echo '========= JavaScript (GraalJS) ========='
$GRAALVM_HOME/bin/node --jvm $SRC_DIR/leibniz.js
echo
