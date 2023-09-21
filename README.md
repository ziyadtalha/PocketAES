# PocketAES

PocketAES takes in two inputs of 16 bits each — a plaintext and an encryption key.
It encrypts the plaintext to produce a block of ciphertext, again 16 bits in size.

Alternatively, it can take in a block of ciphertext along with the decryption key and.
It then decrypts the ciphertext to produce a block of plain text.

A plaintext block is subdivided into four nibbles (4 bits).

To perform encryption, a data block goes through several different stages:
- SubNibbles
- AddRoundKey
- MixColumns
- ShiftRows
- Generate Round Keys

Some of these stages are used multiple times.

Decryption is simply the opposite order of the steps taken in encryption with some differences in the functions used.

Current features:
- Demonstration of individual steps of Encryption
- Decrypting one block of cipher text
- Decrypting an encrypted file
