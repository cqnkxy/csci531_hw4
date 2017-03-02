# USC CSCI531 HW4

A simple implementation of DES and EDE(triple DES). Because DES is meant to be implemented by hardware and it's painful to do it by coding, I use vector of bool to represent bits to ease the job. It's inefficient, but does work.

## Compile

type `make` or `make hw4` to generate executable.
type `make clean` to clear up *.o file and executable.

## Command

### checktable file: hw4 tablecheck -t=tablefile
Check the integrity of tablefile. Please see [tablefile format](http://merlot.usc.edu/cs531-s17/homeworks/hw4/#format) to see the requirements.

### des encrypt: hw4 encrypt -k=key -t=tablefile [file]
Encrypts the input using DES driven by tablefile in the ECB mode using key as the encryption key (8-byte long and in hexstring format). If the filesize of the input is not a multiple of 8 bytes, please pad it with zeroes. (Please treat file as a binary file.)

### des decrypt: hw4 decrypt -k=key -t=tablefile [file]
Decrypts the input using DES driven by tablefile in the ECB mode using key as the encryption key (8-byte long and in hexstring format). If the filesize of the input is not a multiple of 8 bytes, please pad it with zeroes. (Please treat file as a binary file.)

### EDE encrypt: hw4 encrypt3 -k=key3 -t=tablefile [file]
Encrypts the input using EDE triple DES driven by tablefile in the ECB mode using key3 as the encryption key (24-byte long and in hexstring format). If the filesize of the input is not a multiple of 8 bytes, please pad it with zeroes. (Please treat file as a binary file.)

### DED decrypt: hw4 decrypt3 -k=key3 -t=tablefile [file]
Decrypts the input (assuming it was created using the "encrypt3" command) driven by tablefile in the ECB mode using key3 as the encryption key (24-byte long and in hexstring format). If the filesize of the input is not a multiple of 8 bytes, please pad it with zeroes. (Please treat file as a binary file.)
