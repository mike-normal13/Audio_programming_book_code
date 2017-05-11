#include <stdio.h>

int nameToPc(char name){
    switch(name){
     case 'C': case 'c':
       return 0;
     case 'D': case 'd':
       return 2;
     case 'E': case 'e':
       return 4;
     case 'F': case 'f':
       return 5;
     case 'G': case 'g':
       return 7;
     case 'A': case 'a':
       return 9;
     case 'B': case 'b':
       return 11;
    default:  /* error code */
       return 100;
   }
}

int main()
 {
   char note1,note2, dummy;
   int interval;
  
   printf("Please enter two natural notes.\nfirst note: ");
   scanf("%c%c",&note1, &dummy);
   printf("second note: ");
   scanf("%c",&note2);
   
   interval = nameToPc(note2) - nameToPc(note1);
   if(interval > 20 || interval < -11) {
     printf("either %c or %c are invalid notes\n", note1, note2);
     return 1;
   }  
   if(interval < 0) interval += 12;
   else if(interval > 11) interval -= 12;
   printf("%d semitones up or %d semitones down\n", interval, 
	  interval ? 12-interval : 0 );
   
   return 0;

 }
