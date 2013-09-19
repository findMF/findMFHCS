CODE_PATH=/home/witold/prog/qttest/mandelbrot
EXTRA_INCLUDE_PATHS=/home/witold/prog/qttest/mandelbrot/include

echo cppcheck --force --inline-suppr --template '{file}:{line},{severity},{id},{message}' -i "$EXTRA_INCLUDE_PATHS" -q $(for b in $(for a in $(find "$CODE_PATH" | egrep '\.h$'); do dirname $a; dirname $(dirname $a); done | sort -u); do echo -n " -I $b"; done) "$CODE_PATH" 
#>err.txt 2> err2.txt; more err2.txt


$(for b in $(for a in $(find "$CODE_PATH" | egrep '\.h$'); do dirname $a; dirname $(dirname $a); done | sort -u); do echo -n " -I $b"; done) "$CODE_PATH"
