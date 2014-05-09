#cppcheck --help
#cppcheck -I /home/witold/prog/qttest/rl/include --check-config ../
cppcheck -I /home/witold/prog/qttest/rl/include --force --enable=all --template '{file}:{line},{severity},{id},{message}' -q ../src

