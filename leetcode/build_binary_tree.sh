#!/bin/bash

#选择编译二叉树算法

cat <<EOF
1 BST所有路径--257
2 BST中的众数--501
EOF

read -p "选择编译二叉树算法: " MENU

define_arg=""
case $MENU in 
    1)
        echo "BST所有路径"
        define_arg="-DSEARCH_ALL_PATH"
    ;;
    2)
        echo "BST中的众数"
        define_arg="-DFIND_MODE"
    ;;
    *)
        echo "选择错误"
    ;;
esac

build_arg="-g binary_tree.cpp -o binary_tree --std=c++11 ${define_arg}" 

echo "build arg :g++ ${build_arg}"

g++ $build_arg

echo "------->"

./binary_tree
