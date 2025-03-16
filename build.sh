#!/bin/bash

# Definir variáveis
SRC="src/app.cpp"
OUT_DIR="out"
OUT_FILE="$OUT_DIR/app"
INCLUDE_DIR="include"

# Criar a pasta out/ se não existir
mkdir -p "$OUT_DIR"

# Compilar o código
g++ "$SRC" -I"$INCLUDE_DIR" -o "$OUT_FILE"

# Verificar se a compilação foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "Compilação concluída com sucesso! Executável: $OUT_FILE"
else
    echo "Erro na compilação."
    exit 1
fi
