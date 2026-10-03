#!/bin/bash

> app.log
> direct_app.log

echo "[RUN] Компиляция всех C++ компонентов через g++..."
g++ creator.cpp -o creator
g++ writer1.cpp -o writer1
g++ reader.cpp -o reader
g++ sem_reader.cpp -o sem_reader
g++ polisman.cpp -o polisman

echo "[RUN] Инициализация IPC через creator..."
./creator

echo "[RUN] Запуск фоновых процессов..."
PIDS=()

# 1. B = 8 писателей
for (( i=0; i<8; i++ )); do
    ./writer1 $i >> app.log 2>&1 &
    PIDS+=($!)
done

# 2. 6 прямых читателей (ИЗМЕНЕНО: теперь пишут в отдельный лог)
for (( i=0; i<6; i++ )); do
    ./reader $i >> direct_app.log 2>&1 &
    PIDS+=($!)
done

# 3. C = 5 семафорных читателей
for (( i=0; i<5; i++ )); do
    ./sem_reader $i >> app.log 2>&1 &
    PIDS+=($!)
done

sleep 1

echo "[RUN] Запуск Polisman..."
./polisman

kill "${PIDS[@]}" > /dev/null 2>&1
echo "[RUN] Завершено."
