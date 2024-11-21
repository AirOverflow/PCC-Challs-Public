// Compile: gcc -o packed packed.c

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define MAX_OBJS 0x10 // Number of max elements allowed.
#define MAX_BUFFER_SIZE 0x18

int __flush;

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IOFBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

struct Owner {
    int32_t id;
    int8_t ownedAnimals;
    char *name;
} __attribute__((packed));

struct Animal {
    char *name;
    int price;
    int32_t id;
    int owner_id;
    int32_t is_sold;
} __attribute__((packed));

struct Zoo {
    int32_t zoo_id;
    char *name;
    void (*handler)();
};

/* Globals */
int32_t g_zooID = 0, g_animalID = 0, g_ownerID = 0;
struct Zoo*    zoos[MAX_OBJS];
struct Animal* animals[MAX_OBJS];
struct Owner*  owners[MAX_OBJS];

void init_app() {
    for(int i = 0; i < MAX_OBJS; i++) {
        zoos[i]    = NULL;
        owners[i]  = NULL;
        animals[i] = NULL;
    }
}

/* All Owner related tasks: */
void create_owner() {
    if(g_ownerID >= MAX_OBJS) {
        printf("[!] Maximum number of owners created.\n");
        exit(1);
    }

    struct Owner *owner = (struct Owner*)malloc(sizeof(struct Owner));
    owner->name = (char*)malloc(MAX_BUFFER_SIZE);
    printf("Enter your name: ");
    fgets(owner->name, MAX_BUFFER_SIZE, stdin);

    owner->id = g_ownerID++;
    owner->ownedAnimals = 0;
    owners[owner->id] = owner;
    printf("[*] Created owner %s with id: %d\n", owner->name, owner->ownedAnimals);
}

void delete_owner() {
    if(g_ownerID <= 0) {
        printf("[!] Cannot delete non-existing users.\n");
        exit(1);
    }

    int id;
    printf("Enter the id you want to delete: ");
    scanf("%d%*c", &id);
    if(id < 0 || id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!owners[id]) {
        printf("[!] Cannot empty an already empty owner chunk.");
        exit(1);
    }
    void *name = owners[id]->name;
    free(owners[id]);
    if(name) free(name);
    printf("[*] Owner %d deleted successfully.\n", id);
}

/* All Animal related tasks */
void create_animal() {
    if(g_animalID >= MAX_OBJS) {
        printf("[!] Maximum number of animals created.\n");
        exit(1);
    }

    struct Animal *animal = (struct Animal*)malloc(sizeof(struct Animal));
    animal->name = (char*)malloc(MAX_BUFFER_SIZE);
    printf("Enter the name of the animal: ");
    read(0, animal->name, MAX_BUFFER_SIZE);
    animal->name[strcspn(animal->name, "\n")] = 0x0;
    printf("Enter the price of the animal: ");
    scanf("%d%*c", &animal->price);
    animal->id = g_animalID++;
    animal->owner_id = -1;
    animal->is_sold = false;
    animals[animal->id] = animal;
    printf("[*] Created animal %s with id: %d\n", animal->name, animal->id);
}
void delete_animal() {
    int id;
    printf("Enter the id you want to delete: ");
    scanf("%d%*c", &id);
    if(id < 0 || id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!animals[id]) {
        printf("[!] Cannot empty an already empty animal chunk.");
        exit(1);
    }
    void *name = animals[id]->name;     
    free(animals[id]);
    if(name) free(name);
    printf("[*] Animal %d deleted successfully.\n", id);
}
void modify_animal() {
    int id;
    printf("Enter the id you want to modify: ");
    scanf("%d%*c", &id);
    if(id < 0 || id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!animals[id]) {
        printf("[!] Cannot modify a non-existing animal.\n");
        exit(1);
    }

    if(animals[id]->price < 0) {
        printf("[!] Cannot modify the price of the animal.\n");
        exit(1);
    }

    if(animals[id]->is_sold || animals[id]->owner_id >= 0) {
        printf("[!] Cannot modify the animal as it is already sold.\n");
        exit(1);
    }

    printf("Enter the new name of the animal: ");
    read(0, animals[id]->name, MAX_BUFFER_SIZE);
    animals[id]->name[strcspn(animals[id]->name, "\n")] = 0;
    printf("Enter the new price of the animal: ");
    scanf("%d%*c", &animals[id]->price);
}
void buy_animal() {
    int animal_id, owner_id;
    printf("Enter the animal id you want to buy: ");
    scanf("%d%*c", &animal_id);
    printf("Enter the owner id you want to buy: ");
    scanf("%d%*c", &owner_id);

    if(animal_id < 0 || animal_id >= MAX_OBJS || owner_id < 0 || owner_id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!animals[animal_id] || !owners[owner_id]) {
        printf("[!] Cannot buy an animal from non-existing owner or animal.\n");
        exit(1);
    }

    if(animals[animal_id]->is_sold) {
        printf("[!] Animal already sold.\n");
        exit(1);
    }

    animals[animal_id]->owner_id = owner_id;
    animals[animal_id]->is_sold = true;
    owners[owner_id]->ownedAnimals++;
    printf("[*] Animal %s bought by %s\n", animals[animal_id]->name, owners[owner_id]->name);
}
void print_animal() {
    int id;
    printf("Enter the id you want to print: ");
    scanf("%d%*c", &id);

    if(id < 0 || id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!animals[id]) {
        printf("[!] Cannot print a non-existing animal.\n");
        exit(1);
    }

    printf("Animal ID: %d\n", animals[id]->id);
    printf("Animal Name: %s\n", animals[id]->name);
    printf("Animal Price: %d\n", animals[id]->price);
    printf("Animal Owner ID: %d\n", animals[id]->owner_id);
    printf("Animal Sold: %s\n", animals[id]->is_sold ? "Yes" : "No");
}

// ** Setup Zoo based on area ** //
void __init_zoo() {
    printf("Welcome to the Zoo. Hope you'll have fun.\n");
}
void __leave_zoo() {
    printf("Thank you for visting the zoo. But it is time to leave now!\n");
    exit(0);
}

/* All Zoo related tasks */
void create_zoo() {
    struct Zoo *obj = (struct Zoo*)malloc(sizeof(struct Zoo));
    obj->name = (char*)malloc(MAX_BUFFER_SIZE);
    void (*lh)() = __leave_zoo;
    obj->handler =  (g_zooID % 2 == 0) ? __init_zoo : &lh;
    printf("Enter the name of the Zoo: ");
    size_t sz = read(0, obj->name, MAX_BUFFER_SIZE);
    obj->name[strcspn(obj->name, "\n")] = 0;
    obj->zoo_id = g_zooID++;
    zoos[obj->zoo_id] = obj;
    printf("[*] Created zoo %s with id: %d\n", obj->name, obj->zoo_id);
}
void delete_zoo() {
    int id;
    printf("Enter the id you want to delete: ");
    scanf("%d%*c", &id);

    if(id < 0 || id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!zoos[id]) {
        printf("[!] Cannot empty an already empty zoo chunk.");
        exit(1);
    }

    void *name = zoos[id]->name;
    free(zoos[id]);
    if(name) free(name);
    printf("[*] Zoo %d deleted successfully.\n", id);
}

void visit_zoo() {
    printf("Which zoo do you want to visit: ");
    int id;
    scanf("%d%*c", &id);

    if(id < 0 || id >= MAX_OBJS) {
        printf("[!] Cannot read data from out of bounds\n");
        exit(1);
    }

    if(!zoos[id]) {
        printf("[!] Cannot visit a non-existing zoo.\n");
        exit(1);
    }

    uintptr_t handler = (uintptr_t)zoos[id]->handler;

    bool check = 0;
    _check_corrupt:
    if(handler != (uintptr_t)__init_zoo && handler != (uintptr_t)__leave_zoo && (&handler < handler)) {
        printf("[!] Corrupted handler.\n");
        exit(1);
    }
    if(!check) {
        if((handler+0x10) == ((uintptr_t)&handler))
            handler = *(uintptr_t*)handler+0x10;
        check = 1;
        goto _check_corrupt;
    }
    if(check) zoos[id]->handler = (void(*)())handler;
    zoos[id]->handler();
}

void menu() {
    printf("=== Zoo Management System ===\n");
    printf("1. Create Owner\n");
    printf("2. Delete Owner\n");
    printf("3. Create Animal\n");
    printf("4. Delete Animal\n");
    printf("5. Modify Animal\n");
    printf("6. Buy Animal\n");
    printf("7. Print Animal\n");
    printf("8. Create Zoo\n");
    printf("9. Delete Zoo\n");
    printf("10. Visit Zoo\n");
    printf("0. Exit\n");
}

int main(int argc, char* argv[], char* envp[]) {

    while(1) {
        menu();
        int choice;
        printf("Enter your choice: ");
        scanf("%d%*c", &choice);

        switch(choice) {
            case 1:
                create_owner();
                break;
            case 2:
                delete_owner();
                break;
            case 3:
                create_animal();
                break;
            case 4:
                delete_animal();
                break;
            case 5:
                modify_animal();
                break;
            case 6:
                buy_animal();
                break;
            case 7:
                print_animal();
                break;
            case 8:
                create_zoo();
                break;
            case 9:
                delete_zoo();
                break;
            case 10:
                visit_zoo();
                break;
            case 0:
                goto _ret;
                break;
            default:
                puts("[!] Invalid choice.");
                break;
        }
    }
_ret:
    return 0;
}