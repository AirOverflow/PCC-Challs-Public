// Compile: gcc -o directors-cut directors-cut.c -lseccomp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <seccomp.h>
#include <sys/mman.h>
#include <sys/prctl.h>

#define PAGE_SIZE 0x1000
#define PAGE_ADDR 0x13370000

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

void init_sbx() {

    scmp_filter_ctx ctx;
    ctx = seccomp_init(SCMP_ACT_ALLOW);

    if (ctx == NULL) exit(0);

    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execveat), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(write), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(read), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(readv), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(writev), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(preadv2), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(pwritev2), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(pwrite64), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(open_by_handle_at), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(mremap), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(fork), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(shmat), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(dup), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(chdir), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(rename), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(fchdir), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(getcwd), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(readlink), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(ptrace), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(setuid), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(capset), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(chroot), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(mount), 0);
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(getdents), 0);

    seccomp_load(ctx);
}

int main(int argc, char* argv[], char* envp[]) {

    char* shellcode = (char*)mmap(
        (void*)PAGE_ADDR,
        PAGE_SIZE,
        PROT_EXEC | PROT_READ | PROT_WRITE,
        MAP_SHARED | MAP_ANONYMOUS,
        -1, 0LL);
    memset(shellcode, 0x0, PAGE_SIZE);

    /*                    DISASSEMBLY
        0:  48 31 c0                xor    rax,rax
        3:  48 31 db                xor    rbx,rbx
        6:  48 31 c9                xor    rcx,rcx
        9:  48 31 d2                xor    rdx,rdx
        c:  48 31 ff                xor    rdi,rdi
        f:  4d 31 c0                xor    r8,r8
        12: 4d 31 c9                xor    r9,r9
        15: 4d 31 d2                xor    r10,r10
        18: 4d 31 db                xor    r11,r11
        1b: 4d 31 e4                xor    r12,r12
        1e: 4d 31 ed                xor    r13,r13
        21: 4d 31 f6                xor    r14,r14
        24: 4d 31 ff                xor    r15,r15
        27: 48 31 e4                xor    rsp,rsp
        2a: 48 31 ed                xor    rbp,rbp
        2d: 66 0f ef c0             pxor   xmm0,xmm0
        31: 66 0f ef c9             pxor   xmm1,xmm1
        35: 66 0f ef d2             pxor   xmm2,xmm2
        39: 66 0f ef e4             pxor   xmm4,xmm4
        3d: 66 0f ef ed             pxor   xmm5,xmm5
        41: 66 0f ef f6             pxor   xmm6,xmm6
        45: 66 0f ef ff             pxor   xmm7,xmm7
        49: 66 45 0f ef c0          pxor   xmm8,xmm8
    */
    sprintf(
        shellcode,
        "%s%s%s%s%s",
        "\x48\x31\xC0\x48\x31\xDB\x48\x31\xC9\x48\x31\xD2\x48\x31\xFF\x4D\x31\xC0",
        "\x4D\x31\xC9\x4D\x31\xD2\x4D\x31\xDB\x4D\x31\xE4\x4D\x31\xED\x4D\x31\xF6",
        "\x4D\x31\xFF\x48\x31\xE4\x48\x31\xED\x66\x0F\xEF\xC0\x66\x0F\xEF\xC9\x66",
        "\x0F\xEF\xD2\x66\x0F\xEF\xE4\x66\x0F\xEF\xED\x66\x0F\xEF\xF6\x66\x0F\xEF",
        "\xFF\x66\x45\x0F\xEF\xC0");

    printf("Give me your shellcode: ");
    read(0, shellcode+0x4E, (PAGE_SIZE-1));

    init_sbx();
    ((void (*)(void))shellcode)();

    return 0;

}