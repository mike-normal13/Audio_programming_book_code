#include <stdio.h>

int main()
 {
   char note1,note2, dummy;
   int pc1, pc2, interval;

   printf("Please enter two natural notes.\nfirst note: ");
   scanf("%c%c",&note1, &dummy);
   printf("second note: ");
   scanf("%c",&note2);

   switch(note1){

     case 'C': case 'c':
     pc1 = 0;
     break;

     case 'D': case 'd':
     pc1 = 2;
     break;

     case 'E': case 'e':
     pc1 = 4;
     break;

     case 'F': case 'f':
     pc1 = 5;
     break;

     case 'G': case 'g':
     pc1 = 7;
     break;

     case 'A': case 'a':
     pc1 = 9;
     break;

     case 'B': case 'b':
     pc1 = 11;
     break;
      
     default:
     printf("error: %c is not a natural note\n",note1);
     return 1;
   }

   switch(note2){

     case 'C': case 'c':
     pc2 = 0;
     break;

     case 'D': case 'd':
     pc2 = 2;
     break;

     case 'E': case 'e':
     pc2 = 4;
     break;

     case 'F': case 'f':
     pc2 = 5;
     break;

     case 'G': case 'g':
     pc2 = 7;
     break;

     case 'A': case 'a':
     pc2 = 9;
     break;

     case 'B': case 'b':
     pc2 = 11;
     break;
      
     default:
     printf("error: %c is not a natural note\n",note2);
     return 1;
   }
   
   interval = pc2 - pc1;
   if(interval < 0) interval += 12;
   else if(interval > 11) interval -= 12;
   printf("%d semitones up or %d semitones down\n", interval, 
	  interval ? 12-interval : 0 );
   
   switch(interval){
     
      case 1:
      printf("minor 2nd up or major 7th down\n");
      break;

      case 2:
      printf("major 2nd up or minor 7th down\n");
      break;
     
      case 3:
      printf("minor 3rd up or major 6th down\n");
      break;

      case 4:
      printf("major 3rd up or minor 6th down\n");
      break;

      case 5: 
      printf("perfect 4th up or perfect 5th down\n");
      break;

      case 6:
      printf("augmented 4th \n");
      break;

      case 7:
      printf("perfect 5th up or perfect 4th down\n");
      break;

      case 8:
      printf("minor 6th up or major 3rd down\n");
      break;

      case 9:
      printf("major 6th up or minor 3rd down\n");
      break;

      case 10:
      printf("minor 7th up or major 2nd down\n");
      break;
     
      case 11:
      printf("major 7th up or minor 2nd down\n");
      break;

      default:
      printf("unison \n");
     
   }

   return 0;
 }
