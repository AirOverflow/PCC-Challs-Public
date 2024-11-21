// Compile: gcc -o database database.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_NAME_LEN 0x50
#define MAX_OBJECTS 0x10
#define MAX_CLASSES_LEN 0x80

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

struct __obj {
    char name[MAX_NAME_LEN];
    __uint64_t age;
    __uint64_t id;
    char classes[MAX_CLASSES_LEN];
} __attribute__((packed));


typedef struct __obj Student;
typedef struct __obj Teacher;
typedef FILE* Storage;
Storage storage;

Student **students;
Teacher **teachers;

size_t last_student = 0, last_teacher = 0;

void init_db() {
    students = (Student **)malloc(MAX_OBJECTS * sizeof(Student *));
    teachers = (Teacher **)malloc(MAX_OBJECTS * sizeof(Teacher *));

    for (int i = 0; i < MAX_OBJECTS; i++) {
        students[i] = (Student *)malloc(sizeof(Student));
        teachers[i] = (Teacher *)malloc(sizeof(Teacher));
    }

    storage = fopen("/tmp/storage.db", "w+");
    if (storage == NULL) {
        printf("Failed to open storage.db\n");
        exit(1);
    }
}

/**** STUDENTS ****/

void print_student(Student *student) {
    printf("=== Student Info ===\n");
    printf("ID     : %ld\n", student->id);
    printf("Name   : %s\n", student->name);
    printf("Age    : %ld\n", student->age);
    printf("Classes: %s\n", student->classes);
}

void add_student() {
    __int64_t id;
    printf("Enter student id: ");
    scanf("%ld%*c", &id);
    students[id] = (!students[id]) ? &students[0] : students[id];
    printf("Enter student name: ");
    read(0, students[id]->name, MAX_NAME_LEN);
    students[id]->name[strcspn(students[id]->name, "\n")] = 0;
    printf("Enter student age: ");
    scanf("%ld%*c", &students[id]->age);
    printf("Enter student classes (Comma-Seperated): ");
    read(0, students[id]->classes, MAX_CLASSES_LEN);
    students[id]->classes[strcspn(students[id]->classes, "\n")] = 0;
    last_student = id;
}

void update_student_db() {
    fwrite(students[last_student], sizeof(Student), 1, storage);
    printf("Wrote %s to storage\n", students[last_student]->name);
}

/**** TEACHERS ****/

void print_teacher(Teacher *teacher) {
    printf("=== Teacher Info ===\n");
    printf("ID  : %ld\n", teacher->id);
    printf("Name: %s\n", teacher->name);
    printf("Age : %ld\n", teacher->age);
    printf("Classes: %s\n", teacher->classes);
}

void add_teacher() {
    __int64_t id;
    printf("Enter teacher id: ");
    scanf("%ld%*c", &id);
    teachers[id] = (!teachers[id]) ? &teachers[0] : teachers[id];
    printf("Enter teacher name: ");
    read(0, teachers[id]->name, MAX_NAME_LEN);
    teachers[id]->name[strcspn(teachers[id]->name, "\n")] = 0;
    printf("Enter teacher age: ");
    scanf("%ld%*c", &teachers[id]->age);
    printf("Enter classes taught by teacher (Comma-Seperated): ");
    read(0, teachers[id]->classes, MAX_CLASSES_LEN);
    teachers[id]->classes[strcspn(teachers[id]->classes, "\n")] = 0;
    last_teacher = id;
}

void update_teacher_db() {
    fwrite(
        teachers[last_teacher],
        sizeof(Teacher),
        1,
        storage
    );
    printf("Wrote %s to storage\n", teachers[last_teacher]->name);
}

void menu() {
    printf("\n=== Database Menu ===\n");
    printf("===== Student ====\n");
    printf("1. Add Student\n");
    printf("2. Print added Student\n");
    printf("3. Add Last Entry in DB\n");
    printf("===== Teacher ====\n");
    printf("4. Add Teacher\n");
    printf("5. Print Teacher\n");
    printf("6. Add Last Entry in DB\n");
    printf("0. Exit\n");
}

int main(int argc, char* argv[], char* envp[]) {

    init_db();
    __uint64_t choice;

    while (1) {
        menu();
        printf("Enter your choice: ");
        scanf("%lu%*c", &choice);
        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                print_student(students[last_student]);
                break;
            case 3:
                update_student_db();
                break;
            case 4:
                add_teacher();
                break;
            case 5:
                print_teacher(teachers[last_teacher]);
                break;
            case 6:
                update_teacher_db();
                break;
            case 0:
                exit(0);
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}