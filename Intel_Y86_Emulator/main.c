//
//  main.c
//  Henningsgaard_Project3_CEG4350
//
//  Created by Jacob Henningsgaard on 7/20/15.
//

#include <stdio.h>
#include <ctype.h>


//Global variables
unsigned char memArray[2000]; //this array stores instructions and handels the stack
int pc; //program counter
int ZF; //zero flag
int SF; //sign flag
int OF; //overflow flag
int numOfInstructions; //number of instructions from input file
char * programOutput; //stores the entire program instructions

////declare register variables
unsigned int * eax; // Register 0
unsigned int * ecx; // Register 1
unsigned int * edx; // Register 2
unsigned int * ebx; // Register 3
unsigned int * esp; // Register 4
unsigned int * ebp; // Register 5
unsigned int * esi; // Register 6
unsigned int * edi; // Register 7

/*This method simple prints out the name of the correct
 register given its hex value (e.g. register 0 == 'eax').*/
void printRegisterName(char registerValue){
    
    switch (registerValue) {
        case 0x00: //register 'eax'
            printf("eax");
            break;
        case 0x01: //register 'ecx'
            printf("ecx");
            break;
        case 0x02: //register 'edx'
            printf("edx");
            break;
        case 0x03: //register 'ebx'
            printf("ebx");
            break;
        case 0x04: //register 'esp'
            printf("esp");
            break;
        case 0x05: //register 'ebp'
            printf("ebp");
            break;
        case 0x06: //register 'esp'
            printf("esi");
            break;
        case 0x07: //register 'ebp'
            printf("edi");
            break;
            
        default:
            break;
    }
}

/*This method converts ascii characters to hexadecimal characters*/
unsigned char asciiToHex (char asciiChar){
    
    unsigned char hexChar;
    
    //convert asciiChar to a hexadecimal
    if (isdigit(asciiChar)) {
        hexChar = asciiChar - 48;
    }
    else if (isalpha(asciiChar)){
        hexChar = asciiChar - 87;
    }
    return hexChar;
}

/*This method excepts a hex value representing a register (e.g. 0x03)
 and returns the memory address of the correct register 9(e.g. 'ebx').*/
unsigned int * determineRegister(char registerValue){
    
    //    printf("\nDetermining the Register...\n");
    
    unsigned int * registerMemAddress = 0;
    
    //    printf("regsterValue is: %X\n", registerValue);
    
    switch (registerValue) {
        case 0x00: //register 'eax'
            registerMemAddress = eax;
            break;
        case 0x01: //register 'ecx'
            registerMemAddress = ecx;
            break;
        case 0x02: //register 'edx'
            registerMemAddress = edx;
            break;
        case 0x03: //register 'ebx'
            registerMemAddress = ebx;
            break;
        case 0x04: //register 'esp'
            registerMemAddress = esp;
            break;
        case 0x05: //register 'ebp'
            registerMemAddress = ebp;
            break;
        case 0x06: //register 'esi'
            registerMemAddress = esi;
            break;
        case 0x07: //register 'edi'
            registerMemAddress = edi;
            break;
            
        default:
            break;
    }
    
    //    printf("regsiterPointer address is: %X\n", registerMemAddress);
    return registerMemAddress;
}

/*Convert values in 'memArray' to little endian*/
int convertToLittleEndian(unsigned int val1, unsigned int val2, unsigned int val3, unsigned int val4){
    
    unsigned int littleEndianVal = 0;
    
    littleEndianVal |= val1;
    littleEndianVal |= val2 << 8;
    littleEndianVal |= val3 << 16;
    littleEndianVal |= val4 << 24;
    
    return littleEndianVal;
}

/*This method reads in an input file, converts the ASCII text to hexadecimal and stores in memory array*/
int loadInstructionsIntoMem(int argc, const char * argv[]) {
    
    unsigned int charFromFile;
    int i = 0; //index for 'memArray' array
    
    FILE *ptr_file;
    
    //    ptr_file =fopen("/Users/Jake/Dropbox/Summer\ \'15/CEG\ 4350/Project\ 3/Henningsgaard_Project3_CEG4350/Henningsgaard_Project3_CEG4350/input2.txt","r");
    
    ptr_file =fopen(argv[1], "r");
    if (!ptr_file){
        printf("File not found");
        return 1;
    }
    
    if (ptr_file==NULL) perror ("Error opening file");
    else
    {
        while ((charFromFile = fgetc(ptr_file)) != EOF) {
            //TODO
            memArray [i] = asciiToHex(charFromFile);
            memArray[i] = (memArray[i] << 4) | (asciiToHex(fgetc(ptr_file)));
            i++;
            numOfInstructions++;
        }
        fclose(ptr_file);
    }
    numOfInstructions = numOfInstructions / 2;
    printf("The instructions from the file have been loaded into memory.\n");
    return 0;
}

/* 'addl'
 |60|rA|rB|
 This instruction adds rA to rB*/
void addl(){
    
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    unsigned int rBTemp; //used when setting flags
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    rBTemp = *rBPointer; //temporarily store rB value before 'addl' executes
    
    //add the value in rA to the value in rB, and store in rB
    *rBPointer = *rBPointer + *rAPointer;
    
    //clear flags and set if necessary
    ZF = 0;
    SF = 0;
    OF = 0;
    
    if (*rBPointer >= 0x80000000) {
        //set SF
        printf("   SF has been set. ");
    }
    if (*rBPointer == 0) {
        //set ZF
        printf("   ZF has been set. ");
    }
    if ((*rAPointer >= 0x80000000) && (rBTemp >= 0x80000000) && (*rBPointer < 0x80000000)) {
        //set OF
        printf("   OF has been set. ");
    }
    if ((*rAPointer <= 0x80000000) && (rBTemp <= 0x80000000) && (*rBPointer >= 0x80000000)){
        //set OF
        printf("   OF has been set. ");
    }
    
    
    printf("\n0x%02X: %x%x          |\t addl %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
}

/* 'subl'
 |61|rA|rA|
 This instruction subtracts rA from rB.*/
void subl(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    //add the value in rA to the value in rB, and store in rB
    *rBPointer = *rBPointer - *rAPointer;
    
}

/* 'halt'
 |00|
 This instruction sets the program status to 'HLT'*/
void halt(){
    //UPDATE PROGRAM STATUS
}

/* 'nop'
 |10|
 This instruction performs no operation
 */
void nop(){
    //    printf("\nA 'nop' instruction (|10|) has been executed.  No operation has occured.\n");
    printf("\n0x%X: %x            |\t nop", &memArray[pc], memArray[pc]);
}

/* 'rrmovl'
 |20|rA|rB|
 This instruction copies contents in rA to rB*/
void rrmovl(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t rrmovl %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //copy contents in rA to rB
    *rBPointer = *rAPointer;
}

/* 'irmovl'
 |30|F|rB|____V_____|
 This instruction copies value into register rB*/
void irmovl(){
    unsigned char rB;
    int value = 0;
    unsigned int * rBPointer; //points to rB register
    
    pc++; //increment program counter to point to register rB
    
    //get rB register from 'memArray'
    rB = memArray[pc];
    rB &= 0x0F;
    
    //get pointers to the correct registers
    rBPointer = determineRegister(rB);
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    //move value into rB register
    *rBPointer = value;
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x%02x  |\t irmovl $%d, %%", &memArray[pc - 5], memArray[pc - 5]
           , memArray[pc -4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
    printRegisterName(rB);
    
    
}

/* 'rmmovl'
 |40|rA|rB|____D_____|
 This instruction copies value in register rA
 into register rB given an offset value*/
void rmmovl(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    int value = 0;
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x%02x  |\t rmmovl %%", &memArray[pc - 5], memArray[pc - 5]
           , memArray[pc -4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %d(%%", value);
    printRegisterName(rB);
    printf(")");
    
    //    //move value in rA into rB +/- offset
    //    rBPointer = &rBPointer + value; //add offset to rB register
    //    *rBPointer = *rAPointer;
    //
}

/* 'mrmovl'
 |50|rA|rB|____D_____|
 This instruction copies value in register rB(plus offset)
 into register rA*/
void mrmovl(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    int value = 0;
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x%02x  |\t mrmovl %d(%%", &memArray[pc - 5], memArray[pc - 5]
           , memArray[pc -4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
    printRegisterName(rB);
    printf("), %%");
    printRegisterName(rA);
    
    //move value in rA into rB +/- offset
    rBPointer = &rBPointer + value; //add offset to rB register
    *rBPointer = *rAPointer;
    
}

/* 'andl'
 |62|rA|rB|
 This instruction does a bitwise AND on rA and rB
 and stores the value in rB*/
void andl(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    unsigned int rBTemp; //used when setting flags
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    rBTemp = *rBPointer; //temporarily store rB value before 'addl' executes
    
    //print out instruction
    printf("\n0x%X: %x%02x          |\t andl %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //and the value in rA to the value in rB, and store in rB
    *rBPointer = *rBPointer & *rAPointer;
    
    //clear flags and set if necessary
    ZF = 0;
    SF = 0;
    OF = 0;
    
    if (*rBPointer >= 0x80000000) {
        //set SF
        printf("   //SF has been set. ");
    }
    if (*rBPointer == 0) {
        //set ZF
        printf("   //ZF has been set. ");
    }
    if ((*rAPointer >= 0x80000000) && (rBTemp >= 0x80000000) && (*rBPointer < 0x80000000)) {
        //set OF
        printf("   //OF has been set. ");
    }
    if ((*rAPointer <= 0x80000000) && (rBTemp <= 0x80000000) && (*rBPointer >= 0x80000000)){
        //set OF
        printf("   //OF has been set. ");
    }
}

/* 'xorl'
 |63|rA|rB|
 This instruction does a bitwise XOR on rA and rB
 and stores the value in rB*/
void xorl(){
    
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    unsigned int rBTemp; //used when setting flags
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    rBTemp = *rBPointer; //temporarily store rB value before 'addl' executes
    
    //print out instruction
    printf("\n0x%X: %x%02x          |\t xorl %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //and the value in rA to the value in rB, and store in rB
    *rBPointer = *rBPointer ^ *rAPointer;
    
    //clear flags and set if necessary
    ZF = 0;
    SF = 0;
    OF = 0;
    
    if (*rBPointer >= 0x80000000) {
        //set SF
        printf("   //SF has been set. ");
    }
    if (*rBPointer == 0) {
        //set ZF
        printf("   //ZF has been set. ");
    }
    if ((*rAPointer >= 0x80000000) && (rBTemp >= 0x80000000) && (*rBPointer < 0x80000000)) {
        //set OF
        printf("   //OF has been set. ");
    }
    if ((*rAPointer <= 0x80000000) && (rBTemp <= 0x80000000) && (*rBPointer >= 0x80000000)){
        //set OF
        printf("   //OF has been set. ");
    }
    
    
}

/* 'jmp'
 |70|D|
 This instruction updates program counter to point
 to the address in destination*/
void jmp(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t jmp '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
    
}

/* 'jle'
 |71|D|
 This instruction updates program counter to point
 to the address in destination*/
void jle(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t jle '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
}

/* 'jl'
 |72|D|
 This instruction updates program counter to point
 to the address in destination*/
void jl(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t jl '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
}

/* 'je'
 |73|D|
 This instruction updates program counter to point
 to the address in destination*/
void je(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t je '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
}

/* 'jne'
 |74|D|
 This instruction updates program counter to point
 to the address in destination*/
void jne(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t jne '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
}

/* 'jge'
 |75|D|
 This instruction updates program counter to point
 to the address in destination*/
void jge(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t jge '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
}

/* 'jg'
 |76|D|
 This instruction updates program counter to point
 to the address in destination*/
void jg(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t jg '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
}

/* 'cmovle'
 |21|rA|rB|
 This instruction copies contents in rA to rB (conditional)*/
void cmovle(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t cmovle %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //    //copy contents in rA to rB
    //    *rBPointer = *rAPointer;
    
}

/* 'cmovl'
 |22|rA|rB|
 This instruction copies contents in rA to rB (conditional)*/
void cmovl(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t cmovl %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //    //copy contents in rA to rB
    //    *rBPointer = *rAPointer;
    
}

/* 'cmove'
 |23|rA|rB|
 This instruction copies contents in rA to rB (conditional)*/
void cmove(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t cmove %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //    //copy contents in rA to rB
    //    *rBPointer = *rAPointer;
    //test comment
    
}

/* 'cmovne'
 |24|rA|rB|
 This instruction copies contents in rA to rB (conditional)*/
void cmovne(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t cmovne %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //    //copy contents in rA to rB
    //    *rBPointer = *rAPointer;
    
}

/* 'cmovge'
 |25|rA|rB|
 This instruction copies contents in rA to rB (conditional)*/
void cmovge(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t cmovge %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //    //copy contents in rA to rB
    //    *rBPointer = *rAPointer;
    
}

/* 'cmovg'
 |26|rA|rB|
 This instruction copies contents in rA to rB (conditional)*/
void cmovg(){
    unsigned char registerValues; //temporary storage for rA & rB pair
    unsigned char rA;
    unsigned char rB;
    unsigned int * rAPointer; //rA pointer
    unsigned int * rBPointer; //rB pointer
    
    pc++; //increment program counter to point to registers rA and rB
    
    //get register values from 'memArray'
    registerValues = memArray[pc];
    rA = registerValues >> 4;
    rB = registerValues & 0x0F;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    rBPointer = determineRegister(rB);
    
    printf("\n0x%02X: %x%x          |\t cmovg %%", &memArray[pc - 1], memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    printf(", %%"); printRegisterName(rB);
    
    //    //copy contents in rA to rB
    //    *rBPointer = *rAPointer;
    
}

/* 'call'
 |80|D|
 Pushes the return address on the stack and jumps to the destination address. The ret instruction returns from such a call*/
void call(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x%02x%02x%02x%02x    |\t call '0x%08x'", &memArray[pc - 4], memArray[pc - 4], memArray[pc - 3], memArray[pc - 2], memArray[pc - 1], memArray[pc], value);
    
    
    //push return address onto the stack
}

/* 'ret'
 |90|
 Pop value from stack, use as address for next instruction*/
void ret(){
    int value = 0;
    
    //get value to move into rB
    value = convertToLittleEndian(memArray[++pc], memArray[++pc], memArray[++pc], memArray[++pc]);
    
    /*if 'jmp' destination is inside 'memArray' update 'pc' otherwise return error*/
    
    //print out instruction
    printf("\n0x%X: %02x            |\t ret", &memArray[pc - 4], memArray[pc - 4]);
    
}


/* 'pushl'
 |A0|rA|f|
 Pop value from stack, use as address for next instruction*/
void pushl(){
    unsigned char rA;
    int value = 0;
    unsigned int * rAPointer; //points to rB register
    
    pc++; //increment program counter to point to register rB
    
    
    //get rA register from 'memArray'
    rA = memArray[pc];
    rA &= 0xF0;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    
    printf("\n0x%X: %02x%02          |\t pushl %%", &memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
}

/* 'popl'
 |B0|rA|f|
 Pop value from stack, use as address for next instruction*/
void popl(){
    unsigned char rA;
    int value = 0;
    unsigned int * rAPointer; //points to rB register
    
    pc++; //increment program counter to point to register rB
    
    
    //get rA register from 'memArray'
    rA = memArray[pc];
    rA &= 0xF0;
    
    //get pointers to the correct registers
    rAPointer = determineRegister(rA);
    
    printf("\n0x%X: %02x%02          |\t popl %%", &memArray[pc - 1], memArray[pc]);
    printRegisterName(rA);
    
}

void executeInstructions(){
    
    unsigned char opcode;
    
    //increment program counter and get instruction
    pc++;
    opcode = memArray[pc];
    
    //choose which instruction should be executed
    switch (opcode) {
            
            //halt (00)
        case 0:
            //do something
            break;
            
            //nop (10)
        case 16:
            nop();
            break;
            
            //rrmovl (20)
        case 32:
            rrmovl();
            break;
            
            //irmovl (30)
        case 48:
            irmovl();
            break;
            
            //rmmovl (40)
        case 64:
            rmmovl();
            break;
            
            //mrmovl (50)
        case 80:
            mrmovl();
            break;
            
            //addl (60)
        case 96:
            addl();
            break;
            
            //subl (61)
        case 97:
            subl();
            break;
            
            //andl (62)
        case 98:
            andl();
            break;
            
            //xorl (63)
        case 99:
            xorl();
            break;
            
            //jmp (70)
        case 112:
            jmp();
            break;
            
            //jle (71)
        case 113:
            jle();
            break;
            
            //jl (72)
        case 114:
            jl();
            break;
            
            //je (73)
        case 115:
            je();
            break;
            
            //jne (74)
        case 116:
            jne();
            break;
            
            //jge (75)
        case 117:
            jge();
            break;
            
            //jg (76)
        case 118:
            jge();
            break;
            
            //cmovle (21)
        case 33:
            cmovle();
            break;
            
            //cmovl (22)
        case 34:
            cmovl();
            break;
            
            //cmove (23)
        case 35:
            cmove();
            break;
            
            //cmovne (24)
        case 36:
            cmovne();
            break;
            
            //cmovge (25)
        case 37:
            cmovge();
            break;
            
            //cmovg (26)
        case 38:
            cmovg();
            break;
            
            //call (80)
        case 128:
            call();
            break;
            
            //ret (90)
        case 144:
            ret();
            break;
            
            //pushl (A0)
        case 160:
            pushl();
            break;
            
            //popl (B0)
        case 176:
            popl();
            break;
            
        default:
            break;
    }
    
}


int main(int argc, const char * argv[]) {
    
    //allocate memory for registers
    eax = (unsigned int *) malloc(4); // Register 0
    ecx = (unsigned int *) malloc(4); // Register 1
    edx = (unsigned int *) malloc(4); // Register 2
    ebx = (unsigned int *) malloc(4); // Register 3
    esp = (unsigned int *) malloc(4); // Register 4
    ebp = (unsigned int *) malloc(4); // Register 5
    esi = (unsigned int *) malloc(4); // Register 6
    edi = (unsigned int *) malloc(4); // Register 7
    
    numOfInstructions = 0;
    
    //initialize memory with 'nop'
    for (int i = 0; i < sizeof(memArray); i++) {
        memArray[i] = 0x10;
    }
    
    pc = -1;//initialize program counter
    
    loadInstructionsIntoMem(argc,  argv);
    printf("The program has begun executing instruction.\n");
    
    //execute the instructions loaded in memory
    for (int j = 0 ; j < (numOfInstructions + 5); j++) {
        executeInstructions();
    }
    
    
    
    //    //print 'memArray'
    //    for (int i = 0; i < 5; i++) {
    //        printf("Value at memory address 0x%d: %X\n", i, memArray[i]);
    //    }
    
    return 0;
}


