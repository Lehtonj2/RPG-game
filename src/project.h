#ifndef _PROJECT__H_
#define _PROJECT__H_

typedef struct character_st {
    char* name;
    int hit_points;
    int experience;
    char* weapon;
    int weapon_damage;
} Character;


int addCharacter(char* buffer, Character* character_array, int array_length);
void attack(char* buffer, Character* character_array, int array_length);
int cmpexperience (const void* a, const void* b);
void saveToFile(char* buffer, Character* character_array, int array_length);
int loadFromFile(char* buffer1, Character* character_array);
void quitProgram(Character* character_array, int array_length);


#endif //! _PROJECT__H_