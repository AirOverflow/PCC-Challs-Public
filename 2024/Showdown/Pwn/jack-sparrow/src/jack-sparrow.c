// Compile: gcc -o jack-sparrow jack-sparrow.c

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#define POS_SPEC "%llu"

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    signal(SIGALRM, exit);
    alarm(0x20);
}

typedef unsigned long long POSITION;

typedef struct {
  char destination[0x50];
  POSITION *lat;
  POSITION lon;
  int _alt;
} direction_ship_t;

// Make sure destination can only be specified once.
int _spec = 0;

void get_input(int *in) {
    // Secure integer input function.
    // https://stackoverflow.com/questions/41145908/how-to-stop-user-entering-char-as-int-input-in-c
    char next;
    if (scanf("%d", in) < 0 || *in < 0 || ((next = getchar()) != EOF && next != '\n')) {
         clearerr(stdin);
         do next = getchar(); while (next != EOF && next != '\n');
         clearerr(stdin);
    }
}

void say(char *__f, char *__s) { printf("[%s]: %s", __f, __s); }
void say_jack(char *__s) { say("Jack Sparrow", __s); }
void say_crew(char *__s) { say("Crew", __s); }

int menu() {
  int idx;
  puts("== Jack Sparrow's choices ==");
  puts("1. Specify Destination");
  puts("2. Specify position (latitude and longitude)");
  puts("3. Abandon ship");
  printf("$ ");
  get_input(&idx);
  return idx;
}

void specify_destination(char* buf, size_t sz) {
  if(_spec >= 1 || _spec < 0) {
    say_crew("Nah cap. That'll be a problem if we change the destination now!\n");
    return;
  }
  say_jack("My crew, where shall we sail to?\n");
  say_crew("");

  fgets(buf, sz, stdin);
  buf[strcspn(buf, "\n")] = '\0';
  say_jack("Well crew, good choice: ");
  printf(buf);
  puts("");
  ++_spec;
}

void alter_position(direction_ship_t *dt) {
  if(dt->_alt) {
    say("Ragetti", "No cap! We already changed the position!\n");
    return;
  }
  say("Ragetti", "Tell me the coords\n");
  say("Latitude", "");
  scanf(POS_SPEC, &dt->lat);

  say("Longitude", "");
  scanf(POS_SPEC, &dt->lon);

  say_jack("Wait, I think those are wrong. ");
  printf("Latitude=%p Longitude=%p\n", dt->lat, dt->lon);
  say_jack("Yeah, they're wrong..");
  *dt->lat = dt->lon;
  say("Ragetti", "Too late cap. But I'll make sure we don't change course now.\n");
  ++dt->_alt;
}

int main() {

  int choice;
  int dir_set = 0;

  direction_ship_t dir;
  dir._alt = 0; // Unaltered course of action.

  say_jack("I'm back\n");
  say_crew("Caps' back boys\n");

  while(1) {
    choice = menu();
    switch (choice) {
    case 1:
      if(dir_set) {
        say("Crew", "Cap, we already changed a destination recently, can't do it again now.\n");
        break;
      }
      specify_destination(dir.destination, 0x50);
      ++dir_set;
      break;
    case 2:
      if(dir._alt) {
        say("Ragetti", "No cap! We already changed the position!\n");
        break;
      }
      alter_position(&dir);
      break;
    case 3:
      say_jack("Well crew, you see; they have caught up to me, you take my baby Black Pearl, and live.\n");
      say_crew("No capppp.....\n");
      goto _end;
      break;
    default:
      printf("What, what is this choice? I do not understand this..\nQuick quick, Sparrow boutta die.\n\n");
      break;
    }
  }

_end:
  free(&dir);
  return 0;
}
