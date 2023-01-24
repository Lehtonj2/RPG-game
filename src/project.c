#include "project.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int addCharacter(char* buffer, Character* character_array, int array_length) {
    char command;
    char name[1000] = "";
    int hit_points = 0;
    char weapon[1000] = "";
    int weapon_damage = 0;
    int parse = sscanf(buffer, "%c %s %d %s %d", &command, name, &hit_points, weapon, &weapon_damage);
    if (parse != 5) {
        printf("A should be followed by exactly 4 arguments.\n");
        return 0;
    }

    if (strcmp(name, "") == 0 || name[0] == ' ') {
        printf("Character name cannot be empty or space.\n");
        return 0;
    }
    if (strcmp(weapon, "") == 0 || weapon[0] == ' ') {
        printf("Weapon name cannot be empty or space.\n");
        return 0;
    }

    if (hit_points <= 0) {
        printf("HP cannot be lower than 1\n");
        return 0;
    }
    if (weapon_damage <= 0) {
        printf("Max damage cannot be lower than 1\n");
        return 0;
    }


    for (int i = 0; i < array_length; i++) {
        Character c = character_array[i];
        if (strcmp(c.name, name) == 0) {
            printf("Fighter \"%s\" is already in the database.\n", name);
            return 0;
        }
    }

    character_array[array_length].name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(character_array[array_length].name, name);
    character_array[array_length].hit_points = hit_points;
    character_array[array_length].experience = 0;
    character_array[array_length].weapon = calloc(strlen(weapon) + 1, sizeof(char));
    strcpy(character_array[array_length].weapon, weapon);
    character_array[array_length].weapon_damage = weapon_damage;
    printf("SUCCESS\n");
    return 1;
}

void attack(char* buffer, Character* character_array, int array_length) {
    char command;
    char attacker_name[1000];
    char target_name[1000];

    if (sscanf(buffer, "%c %s %s", &command, attacker_name, target_name) != 3) {
        printf("H should be followed by exactly 2 arguments.\n");
        return;
    }

    int attacker_index;
    int target_index;

    int valid_attacker_name = 0;
    int valid_target_name = 0;

    for (int i = 0; i < array_length; i++) {
        Character c = character_array[i];
        if (strcmp(c.name, attacker_name) == 0) {
            valid_attacker_name = 1;
            attacker_index = i;
        }
        if (strcmp(c.name, target_name) == 0) {
            valid_target_name = 1;
            target_index = i;
        }
    }

    if (!valid_attacker_name && !valid_target_name) {
        printf("Attacker \"%s\" and target \"%s\" names are not in the database.\n", attacker_name, target_name);
        return;
    } else if (!valid_attacker_name) {
        printf("Attacker \"%s\" is not in the database.\n", attacker_name);
        return;
    } else if (!valid_target_name) {
        printf("Target \"%s\" is not in the database.\n", target_name);
        return;
    }
    Character* attacker = &character_array[attacker_index];
    if (attacker->hit_points == 0) {
        printf("Invalid command arguments\n");
        return;
    }
    Character* target = &character_array[target_index];
    if (strcmp(attacker->name, target->name) == 0) {
        printf("Attacker \"%s\" cannot attack to itself.\n", attacker->name);
        return;
    }

    int damage_inflicted;
    if (target->hit_points < attacker->weapon_damage) {
        damage_inflicted = target->hit_points;
        target->hit_points = 0;
    } else {
        target->hit_points = target->hit_points - attacker->weapon_damage;
        damage_inflicted = attacker->weapon_damage;
    }
    attacker->experience += damage_inflicted;

    printf("%s attacked %s with %s by %d damage.\n", attacker_name, target_name, attacker->weapon, damage_inflicted);
    printf("%s has %d hit points remaining.\n", target_name, target->hit_points);
    printf("%s gained %d experience points.\n", attacker_name, damage_inflicted);
    printf("SUCCESS\n");
}

int cmpexperience (const void* a, const void* b) {
    Character *A = (Character *)a;
    Character *B = (Character *)b;

    return ( (int)B->experience - (int)A->experience );
}

void printGame(Character* character_array, int array_length) {
    Character* print_array = character_array;
    qsort(print_array, array_length, sizeof(Character), (cmpexperience));
    for (int i = 0; i < array_length; i++) {
        Character c = print_array[i];
        printf("%s %d %d %s %d\n", c.name, c.hit_points, c.experience, c.weapon, c.weapon_damage);
    }
    printf("SUCCESS\n");
}


void saveToFile(char* buffer, Character* character_array, int array_length) {
    char filename[1000];
    char command;

    if (sscanf(buffer, "%c %s", &command, filename) != 2) {
        printf("W should be followed by exactly 1 argument.\n");
        return;
    }

	FILE *fp;

	fp = fopen( filename , "w" );
	if(fp == NULL) {
        printf("Cannot open file %s for reading.\n", filename);
        return;
    }


	
	for (int i = 0; i < array_length; i++) {
        Character c = character_array[i];
        fprintf(fp, "%s %d %d %s %d\n", c.name, c.hit_points, c.experience, c.weapon, c.weapon_damage);
    }
	
	fclose(fp);
    printf("SUCCESS\n");
	}

int loadFromFile(char* buffer1, Character* character_array) {
    char filename[1000];
    char command;

    if (sscanf(buffer1, "%c %s", &command, filename) != 2) {
        printf("O should be followed by exactly 1 argument.\n");
        return 0;
    }


    FILE *fp;

    fp = fopen( filename , "r" );
    if(fp == NULL) {
        printf("Cannot open file %s for reading.\n", filename);
        return(0);
    }

        //variables 
    char name[1000];
    int hit_points;
    int experience;
    char weapon[1000];
    int weapon_damage;
    int y = 0;
    while(1) {
        if (feof(fp)) {
            break;
        }
        char buffer[1000] = {'\0'};
        fgets(buffer, 1000, fp);
        if (strlen(buffer) == 0) {
            break;
        }
        sscanf(buffer, "%s %d %d %s %d", name, &hit_points, &experience, weapon, &weapon_damage);
        free(character_array[y].name);
        character_array[y].name = calloc(strlen(name) + 1, sizeof(char));
        strcpy(character_array[y].name, name);
        character_array[y].hit_points = hit_points;
        character_array[y].experience = experience;
        free(character_array[y].weapon);
        character_array[y].weapon = calloc(strlen(weapon) + 1, sizeof(char));
        strcpy(character_array[y].weapon, weapon);
        character_array[y].weapon_damage = weapon_damage;
        y++;
    }

    fclose(fp);
    printf("SUCCESS\n");
    return(y);

    }

void quitProgram(Character* character_array, int array_length) {
    for (int i = 0; i < array_length; i++) {
        free(character_array[i].name);
        free(character_array[i].weapon);
    }
    free(character_array);
    printf("SUCCESS\n");
} 




int main(void) {
    Character* character_array;
    character_array = (Character*)calloc(1000, sizeof(Character));
    int array_length = 0;
    int continue_game = 1;



    while(continue_game) {
        char buffer[1000];

        if (fgets(buffer, 1000, stdin) != NULL) {
            char command = buffer[0];
            switch(command) {
                case 'A': ;
                    if (addCharacter(buffer, character_array, array_length)) {
                        array_length++;
                    }
                    break;
                case 'H':
                    attack(buffer, character_array, array_length);
                    break;
                case 'L':
                    printGame(character_array, array_length);
                    break;
                case 'W':
                    saveToFile(buffer, character_array, array_length);
                    break;
                case 'O': ;
                    int result = loadFromFile(buffer, character_array);
                    if (result) {
                        array_length = result;
                    }
                    break;
                case 'Q':
                    quitProgram(character_array, array_length);
                    continue_game = 0;
                    break;
                default:
                    printf("Invalid command %c\n", buffer[0]);
                    break;
                
            }


        }
    }
}