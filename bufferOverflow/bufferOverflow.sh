clear

gcc -fno-stack-protector bufferOverflow.c util.c -o bufferOverflow && ./bufferOverflow $1