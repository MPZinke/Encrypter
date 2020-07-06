
/***********************************************************************************************************
*
*	created by: MPZinke
*	on ..
*
*	DESCRIPTION: TEMPLATE
*	BUGS:
*	FUTURE:
*
***********************************************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>


typedef struct Translation
{
	char text[5];
	struct Translation* next;
} Translation;


// ——— TRANSLATION ———
void delete_translation(Translation*);
Translation* new_translation_segment();
Translation* translate_file(FILE*);
void update_iterator(Translation***, Translation*);
// ——— ENCRYPTION ———
bool encode_section(Translation***, FILE*);
void encrypt(char[], char[]);
// ——— FILE IO ———
int read_from_file(FILE*, char[]);
void write_to_file(Translation*, char[]);
// ——— OTHER ———
void prompt();


// ————————————————— TRANSLATION ——————————————————

void delete_translation(Translation* head)
{
	while(head)
	{
		free(head);
		head = head->next;
	}
}


Translation* new_translation_segment()
{
	return calloc(1, sizeof(Translation));
}


Translation* translate_file(FILE* file)
{
	Translation* head = NULL;
	Translation** iter = &head;
	int x = 50;
	while(encode_section(&iter, file) && x--);

	fclose(file);
	return head;
}


void update_iterator(Translation*** iter, Translation* segment)
{
	if(!**iter) **iter = segment;
	else 
	{
		(**iter)->next = segment;
		*iter = &(**iter)->next;
	}
}


// ————————————————— ENCRYPTION ——————————————————

bool encode_section(Translation*** iter, FILE* file)
{
	char unencrypted[4] = {' ', ' ', ' ', ' '};
	int characters_read = read_from_file(file, unencrypted);
	if(characters_read == 1) return false;

	Translation* segment = new_translation_segment();
	encrypt(segment->text, unencrypted);

	update_iterator(iter, segment);
	return characters_read == 4;
}


void encrypt(char encrypted[], char unencrypted[])
{
	for(int x = 0; x < 4; x++) {
		for(int y = 0; y < 8; y++) {
			int position = x * 10 + y + 2;
			// get bit from 8-bit; cast to 10-bit equivalent position
			int bit = ((unencrypted[x] >> (7 - y)) & 1) << (7 - (position % 8));
			encrypted[position / 8] |= bit;  // turn bit at equivant index
		}
	}

	for(int x = 0; x < 5; x++) encrypted[x] = encrypted[x] ^ 0xFF;
}


// ——————————————————— FILE IO ———————————————————

int read_from_file(FILE* file, char unencrypted[])
{
	int x = 0;
	do
	{
		unencrypted[x] = fgetc(file);
	} while(unencrypted[x++] != EOF && x < 4);

	if(unencrypted[x-1] == EOF) unencrypted[x-1] = ' ';  // replace EOF with ' ' for consistent size (EOF added later)

	return x;
}


void write_to_file(Translation* trans, char filename[])
{
	int length = strlen(filename);
	char* out_filename = malloc(length + 1);  // plus null terminator
	strcpy(out_filename, filename);
	strcpy(out_filename+length-3, "znk");

	FILE* file = fopen(out_filename, "w");
	while(trans)
	{
		fprintf(file, "%c%c%c%c%c", trans->text[0], trans->text[1], trans->text[2], trans->text[3], trans->text[4]);
		trans = trans->next;
	}

	fclose(file);
	free(out_filename);
}


// ——————————————————— OTHER ————————————————————

void prompt()
{
	printf(	"\033[0;35mE\033[0;34mN\033[0;32mC\033[0;33mR\033[0;31mY"
			"\033[0;35mT\033[0;34mI\033[0;32mO\033[0;33mN \033[0;31mP"
			"\033[0;35mR\033[0;34mO\033[0;32mG\033[0;33mR\033[0;31mA\033[0;35mM\033[0;00m\n");
}


int main(int argc, char* argv[])
{
	// input checking
	if(argc < 2) return 1;  // mystically end from no file being passed
	FILE* file = fopen(argv[1], "r");
	if(!file) return 2;  // mysticall end from invalid file being passed

	Translation* translation = translate_file(file);

	write_to_file(translation, argv[1]);
	remove(argv[1]);
	delete_translation(translation);

	return 0;
}
