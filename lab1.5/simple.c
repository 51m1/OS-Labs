
#!/bin/bash

#first

echo "Hello World"
touch out.txt
echo "First the code" >> out.txt
echo "" >> out.txt
find /usr/include/linux -name "*.c" >> out.txt
echo "" >> out.txt
echo "Now the headers" >> out.txt
echo "" >> out.txt
find /usr/include/linux -name "*.h" >> out.txt

