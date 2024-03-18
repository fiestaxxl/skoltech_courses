#!/usr/bin/bash
x=105.78
myvar=$(echo "scale=0;$x/1"|bc)
echo $myvar
