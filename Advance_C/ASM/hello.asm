section .text
    global _start

_start:
    ; sys_write (syscall number 1) - write(1, message, 13)
    mov rax, 1          ; syscall number for sys_write
    mov rdi, 1          ; file descriptor 1 (stdout)
    lea rsi, [rel message] ; pointer to message
    mov rdx, 13         ; message length
    syscall             ; invoke syscall

    ; sys_exit (syscall number 60) - exit(0)
    mov rax, 60         ; syscall number for sys_exit
    xor rdi, rdi        ; exit code 0
    syscall             ; invoke syscall

section .data
message db "Hello, World!", 0xa  ; Message + newline
