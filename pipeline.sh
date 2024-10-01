#!/bin/bash

# 编译C++代码（假设代码文件名为program.cpp）
g++ -o xiaoxiaole xiaoxiaole.cpp

# 检查编译是否成功
if [ $? -ne 0 ]; then
    echo "编译失败！"
    exit 1
fi

# 运行次数
RUNS=100

# 输出文件
OUTPUT_FILE="output.txt"

# 清空输出文件
> $OUTPUT_FILE

# 循环运行程序并将输出追加到同一个文件
for ((i=1; i<=RUNS; i++))
do
    # echo "运行第 $i 次：" >> $OUTPUT_FILE
    ./xiaoxiaole >> $OUTPUT_FILE
    # echo "-------------------------" >> $OUTPUT_FILE
done

echo "所有运行完成，结果保存在 $OUTPUT_FILE 文件中。"