#!/bin/sh
BINARY_HOME=./bin
INPUT_HOME=./input
INPUT=${INPUT_HOME}/AB_NYC_2019.csv


cat ${INPUT} | ${BINARY_HOME}/mapper | sort -k1 | ${BINARY_HOME}/reducer > output
read ALPHA < output
cat output
cat ${INPUT} | ${BINARY_HOME}/mapper_quadretic ${ALPHA} | sort -k1 | ${BINARY_HOME}/reducer > output
cat output
