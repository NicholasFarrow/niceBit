#include <stdio.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>
#include "lib/base58.c"

/* https://en.bitcoin.it/wiki/Wallet_import_format */

char *create_wif(const unsigned char *privatekey) {
	unsigned char newKey[65];
	unsigned char SHAkey[65];
	unsigned char SHAkey2[65];
	unsigned char checksum[11];
	unsigned char combinedKey[75];

	size_t combinedKeySize = 37;	
	size_t wifSize = 52;
	char wif[52];
	
	wifSize = 80;

	/* Add 0x80 byte in front */
	newKey[0] = 128;
	for(int i=0; i<32; i++) {
		newKey[i+1] = privatekey[i];
	}

	/*
	for(int i=0; i<32; i++) {
		printf("%02X", privatekey[i]);
	}
	printf("\n");

	for(int i=0; i<33; i++) {
		printf("%02X", newKey[i]);
	}
	printf("\n");
	*/

	/* Perform SHA-256 hash on the extended key */
	SHA256(newKey, 33, SHAkey); 
	/*
	for(int i=0; i<32; i++) {
		printf("%02X", SHAkey[i]);
	}
	printf("\n");
	*/
	
	/* Perform SHA-256 hash again on the result */
	SHA256(SHAkey, 32, SHAkey2); 
	/*
	for(int i=0; i<32; i++) {
		printf("%02X", SHAkey2[i]);
	}
	printf("\n");
	*/
	
	/* Checksum is first 4 bytes of 2nd SHA*/
	for(int i=0; i<4; i++) {
		checksum[i] = SHAkey2[i];
	}

	/*
	for(int i=0; i<4; i++) {
		printf("%02X", checksum[i]);
	}
	printf("\n");
	*/

	/* Append checksum to end of 2nd SHA */
	for(int i=0; i<33; i++) {
		combinedKey[i] = newKey[i];
	}
	for(int i=0; i<4; i++) {
		combinedKey[33+i] = checksum[i];
	}
	/*
	for(int i=0; i<37; i++) {
		printf("%02X", combinedKey[i]);
	}
	printf("\n");
	*/

	/* Encode with base-58 */	
	b58enc(wif, &wifSize, combinedKey, combinedKeySize);
	puts(wif);

	return 0;
}
