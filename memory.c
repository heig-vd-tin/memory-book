#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define BLOCK_SIZE (0x10000)

typedef struct file {
    char name[64];
    int size;
    char permissions;
    struct file *parent;
    struct file *child;
    struct file *next_sibling;
    struct file *prev_sibling;
} File;

typedef struct memory {
    struct {
        char *zeros[5];
        char *ones[2];
        char *random;
        int *primes;
        char *program;
        void *arguments;
        char *poemes;
        char *swann;

    } addresses;
    char zeros0[BLOCK_SIZE];
    char ones0[BLOCK_SIZE];
    char random[0x80000];
    char zeros1[BLOCK_SIZE];
    int primes[0x1024];
    char filetree[BLOCK_SIZE];
    char program[BLOCK_SIZE];
    char zeros2[BLOCK_SIZE];
    struct arguments {
        int argc;
        char *argv[4];
    } arguments;
    char zeros3[BLOCK_SIZE];
    struct {
        char *beginning;
        char *albatros;
        char *chevelure;
        char *chat;
    } poemes;
    char baudelaire[149206]; // Unicode
    char zeros4[BLOCK_SIZE];
    char swann[893662]; // ASCII
    char ones1[BLOCK_SIZE];
    char arg0[12];
    char arg1[6];
    char arg2[10];
    char arg3[22];
} Memory;

Memory memory = {
    .addresses = {
        .zeros = {
            memory.zeros0,
            memory.zeros1,
            memory.zeros2,
            memory.zeros3,
            memory.zeros4
        },
        .ones = {
            memory.ones0,
            memory.ones1
        },
        .random = memory.random,
        .primes = memory.primes,
        .program = memory.program,
        .arguments = &memory.arguments,
    },
    .arguments = {
        .argc = 4,
        .argv[0] = memory.arg0,
        .argv[1] = memory.arg1,
        .argv[2] = memory.arg2,
        .argv[3] = memory.arg3
    },
    .arg0 = "./memory",
    .arg1 = "-odata",
    .arg2 = "--verbose",
    .arg3 = "a bright unicorn"
};

int is_prime(int test){
    for (int i = 2; i * i <= test; i++){
        if (test % i == 0){
            return false;
        }
    }
    return true;
}

int main(void) {

    // Zeros
    for (int i = 0; i < sizeof(memory.addresses.zeros)/sizeof(memory.addresses.zeros[0]); i++)
        memset(memory.addresses.zeros[i], 0x00, BLOCK_SIZE);

    // Ones
    for (int i = 0; i < sizeof(memory.addresses.ones)/sizeof(memory.addresses.ones[0]); i++)
        memset(memory.addresses.ones[i], 0xFF, BLOCK_SIZE);

    // Random
    for (int i = 0; i < BLOCK_SIZE; i++)
        memory.random[i] = rand() % 0xFF;

    // Primes
    {
        int prime = 1;
        for (int i = 0; i < BLOCK_SIZE; i++) {
            while (!is_prime(++prime));
            memory.primes[i] = prime;
        }
    }

    // Fill with poems
    {
        FILE *fp = fopen("baudelaire.txt", "r");
        fseek(fp, 0, SEEK_END);
        int fsize = ftell(fp);
        rewind(fp);
        fread(memory.baudelaire, 1, fsize, fp);
        fclose(fp);
    }

    // Fill with Proust
    {
        FILE *fp = fopen("swann.md", "r");
        fseek(fp, 0, SEEK_END);
        int fsize = ftell(fp);
        rewind(fp);
        fread(memory.swann, 1, fsize, fp);
        fclose(fp);
    }

    // Adjust addresses
    char *address = (char*)&memory.addresses;
    for (int i = 0; i < sizeof(memory.addresses)/sizeof(char*); i++) {
        address[i] -= (uintptr_t)&memory;
    }

    // Generate data
    char *data = (char*)&memory;
    for (int i = 0; i < sizeof(memory); i++) {
        fputc(data[i], stdout);
    }
}