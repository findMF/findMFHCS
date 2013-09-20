#cppcheck --help
#cppcheck -I /home/witold/prog/qttest/rl/include --check-config ../
cppcheck -I $1/include --force --enable=all --template "{file}	{line}	{severity}	{id},{message}" -q $1/src

