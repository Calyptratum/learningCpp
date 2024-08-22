#!/bin/bash

# 定义可执行文件的数组
executables=(
    "./searchEngine"
)

# 遍历数组并执行每个可执行文件
for exec in "${executables[@]}"; do
    if [[ -x "$exec" ]]; then
        echo "staring: $exec"
        "$exec"  # 执行可执行文件
        if [ $? -ne 0 ]; then
            echo "error: $exec Execution failed and the script terminated"
            exit 1  # 如果执行失败，退出脚本
        fi
    else
        echo "error: $exec This file is not an executable file"
        exit 1  # 如果文件不可执行，退出脚本
    fi
done

echo "success"
