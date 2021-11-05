#!/bin/bash

read -p "Static Linker [y/N]: " linker
if [ "${linker}" = "y" ]; then
    aarch64-linux-gnu-gcc main.c -I install/usr/include install/usr/lib/libapduteec.a install/usr/lib/libteec.a -o get_version    
else
    aarch64-linux-gnu-gcc main.c -I install/usr/include -L install/usr/lib -l apduteec -l teec -o get_version
fi
