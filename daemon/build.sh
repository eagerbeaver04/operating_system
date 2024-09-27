#!/bin/bash

if ! command -v cmake &> /dev/null; then
    echo "cmake не установлен. Установите cmake и повторите попытку."
    exit 1
fi

mkdir -p build
cd build || { echo "Не удалось перейти в директорию сборки."; exit 1; }

cmake ..
if [ $? -ne 0 ]; then
    echo "Ошибка при выполнении cmake."
    exit 1
fi

make
if [ $? -ne 0 ]; then
    echo "Ошибка при выполнении make."
    exit 1
fi

sudo ./daemon config.txt
