file=`find .. -name *.cpp`
nrl=6 # number of lines in header
headerfile=headertemplate.txt

for i in $file # or whatever other pattern...
do
	#if exists than remove from file
  if grep -q "// Copyright : ETH Zurich" $i
  then
   cat  $headerfile > $i.new
   tail -n +$nrl $i >> $i.new
   mv $i.new $i
  else
    cat $headerfile > $i.new
    cat $i >> $i.new
    mv $i.new $i
  fi
done

