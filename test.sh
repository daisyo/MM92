#!/bin/zsh

# ファイルの初期化(logをとる)
cp -p out.txt log/`date +%Y%m%d_%H-%M-%S`.log
:> out.txt

g++ -std=c++11 -W -Wall -Wno-sign-compare -O2 -s -pipe -mmmx -msse -msse2 -msse3 Lighting.cpp -o a
java -jar tester.jar -exec './a' -seed $1

# 10回回す
i=1
m=101

while [ $i -lt $m ]; do
    # 実行
    run=$(java -jar tester.jar -exec './a' -seed $i -novis)

    # split
    IFS_ORIGINAL="$IFS"
    IFS=' '
    arr=($run)
    IFS="$IFS_ORIGINAL"
    t=0

    echo $i
    for v in "${arr[@]}"; do
        # Scoreの点数の部分だけを抜き出して、テキストに出力
        if [ $t = 2 ]; then
            echo $v >> "out.txt"
        fi
        t=$((t+1))
    done

    i=$((i+1))
done

# テキストから合計
t=$(awk '{s += $1; t+=1} END {ans=s/t; ans*=1000000; print ans}' < out.txt)
echo $t
echo "Score = "$t >> out.txt
