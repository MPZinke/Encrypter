all:
	gcc decrypter.c -o decrypt
	gcc encrypter.c -o encrypt

encrypt:
	gcc encrypter.c -o encrypt

decrypt:
	gcc decrypter.c -o decrypt


debug:
	gcc decrypter.c -o decrypt -g
	gcc encrypter.c -o encrypt -g