# Makefile

# プログラム名とオブジェクトファイル名
PROGRAM = sample
OBJS = sample.o can_lib.o

# コンパイラの定義
CC = gcc
CFLAGS = -Wall -O2 -std=c11

# 拡張子の定義
.SUFFIXES: .c .o

# プライマリターゲット
$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS)

# サフィックスルール
.c.o:
	$(CC) $(CFLAGS) -c $<

# 依存ヘッダファイルの設定
sample.o: can_lib.h
can_lib.o: can_lib.h

# ファイル削除ターゲット
.PHONY: clean
clean:
	$(RM) $(PROGRAM) $(OBJS)
