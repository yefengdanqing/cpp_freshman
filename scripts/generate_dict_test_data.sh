#!/bin/bash

# 输出文件名
output_file="output.txt"

# 生成的行数
num_lines=2000000

# 清空输出文件，如果文件已存在
> "$output_file"

# 生成数据并写入文件
for ((i=1; i<=num_lines; i++)); do
  # 生成一个很长的字符串
  long_string=$(head /dev/urandom | tr -dc A-Za-z0-9 | head -c 100000)

  # 写入文件，以\t分割
  echo -e "$i\t$long_string" >> "$output_file"
done

echo "数据已生成并写入到 $output_file 文件中"