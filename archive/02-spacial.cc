#include <stdio.h>
#include <string.h>

int main() {
  char buff[16];
  int pass = 0;

  printf("\n Enter the password : \n");
  gets(buff);

  if (strcmp(buff, "correctpassword")) {
    printf("\n Wrong Password \n");
  } else {
    printf("\n Correct Password \n");
    pass = 1;
  }

  if (pass) {
    // Now Give root or admin rights to user*/
    printf("\n Root privileges given to the user \n");
  }

  return 0;
}
