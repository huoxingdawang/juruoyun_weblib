chmod +w jbl/*
cp -a -f $libs/jbl .
chmod -w jbl/*
chmod +w jbl/jbl*config.h
cat examples/makefile jbl/makefile jwl/makefile > makefile
cp -a -f jwl $libs