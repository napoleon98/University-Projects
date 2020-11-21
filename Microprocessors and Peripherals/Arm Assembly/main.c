#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__asm int ispalindrom(const char *src){

    PUSH {r5, r6, r7}// push in stack
    MOV r3, #0 // r3 will contain the size of string,and it is initialized with 0
    MOV r5, r0 // move the first address of string in r5

loopLength

    LDRB r2 , [r5] // load byte into r2, from the memory pointed by r5.


    CMP r2 , #0 // was r2 equal to 0?

    ADDNE r5, r5, #1 // increment src pointer. At the end of the loop,r5 will contains the address of null byte.

    ADDNE r3 , r3, #1 // if not, increase r3(size)..
    BNE loopLength //... and repeat the loopLength

    SUB r5, r5, #1 // now the r5 contains the address of the last char of string(because at the end of the loop , r5 contains the null byte )
    MOV r6, #0 //r6 will contain the number of chars that are equal
    MOV r7, r3 // save the size (that is in r3)of string in r7

loopCheck

    LDRB r2, [r5] //  load byte into r2, from the address in r5.
    LDRB r4, [r0] // load byte into r4 from memory pointed by r0.

    CMP r2, r4 // compare the chars that is in r2 and r4
    ADDEQ r6, r6, #1 //if the chars are equal, increase r6
    SUB r7, r7, #1//decrese r7 

    CMP r7, #0// if r7 is zero,the string has finished,so that is the end of loop
    ADDNE r0, r0, #1 //if not,increase r0,so the address in r0 points the next char from the beginning of string...
    SUBNE r5, r5, #1//...decrease r5 ,so the address in r5 points the next char from the end of string...

    BNE loopCheck// ...and repeat the loop

    CMP r6, r3 //compare the number of chars that are equal (from the beginning to the end of string) in r6 with the size of string in r3

    MOVEQ r0, #1 // if yes put 1 to r0 
    MOVNE r0, #0 // if not put 0 to r0 
    MOV32 r1, #0x20000650 // move in r1 the address 0x20000650
	
    STR r0, [r1] // store r0(that contains the final result) into memory pointed by r1
		


    POP {r5, r6, r7} // pop of the stack
    BX lr // return in main


}






__asm int arrays(const int *src){//SRC IS AN ARRAY WITH 10 INTEGERS

	PUSH {r4-r12}
	
	MOV r2, #40
	ADD r2, #8
	LDR r0,[r0,r2]
	BX lr
	
/*
forloop
	
	CMP r2, #10
	BEQ endfor
	LDR r1, [r0]
	ADD r2, r2, #1
	ADD r1, r1, #10 //add 10 to each element
	STR r1, [r0]
	ADD r0, r0, #4 // LSL r0, r0, #2
	B forloop
	
endfor
	POP{r4-r12}
	BX lr
	*/
	
	
}

int main()
{
	/*
    char str[] = "anna";// string that is going to be checked
    int return_result; // variable in which,the final result will be stored
  return_result = ispalindrom(str); // ispalindrom() function returns,and the result is stored in return_result variable.

	*/
	int array[2][10] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

	int result;
	
	result = arrays(array);
	
    return 0;
}

