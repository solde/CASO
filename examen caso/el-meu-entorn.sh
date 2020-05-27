#!/bin/bash

lsb_release  -a >el-meu-entorn.txt 2>&1

uname -a >>el-meu-entorn.txt 2>&1

lscpu >>el-meu-entorn.txt 2>&1

free >>el-meu-entorn.txt 2>&1


