.intel_syntax noprefix
.text
.globl addCalc, delCalc

# =========================================================
# 1. Функция addCalc (целочисленная)
# =========================================================
.type addCalc, @function
addCalc:
    push rbp
    mov rbp, rsp

    # --- Обработка указателя a2 (rsi) ---
    test rsi, rsi
    jnz .a2_ok
    mov eax, 1
    jmp .a2_done
.a2_ok:
    mov eax, [rsi]              # val_a2 = *a2
.a2_done:
    movsxd rax, eax
    movsxd rdi, edi
    imul rax, rdi               # (a1 * val_a2)

    # --- Вычитание (a3 + a4) ---
    movsx rdx, dx               # a3 (short -> int64)
    add rdx, rcx                # a3 + a4
    sub rax, rdx                # (a1 * val_a2) - (a3 + a4)

    # --- XOR операции (a5 ^ a6) ---
    movzx r8, r8b               # a5 (char -> uint64)
    xor r8, r9                  # a5 ^ a6
    add rax, r8                 # part_regs -> rax

    # --- Обработка аргументов из стека ---
    mov r10, [rbp + 16]         # a7 (int*)
    test r10, r10
    jnz .a7_ok
    mov r11d, 1
    jmp .a7_done
.a7_ok:
    mov r11d, [r10]             # val_a7 = *a7
.a7_done:
    movsxd r11, r11d
    movsx rdx, word ptr [rbp + 24] # a8 (int16_t -> int64)
    add r11, rdx                # val_a7 + a8
    movsxd rdx, dword ptr [rbp + 32] # a9 (int -> int64)
    sub r11, rdx                # part_stack = val_a7 + a8 - a9

    # --- Итоговое перемножение (результат в RAX) ---
    imul rax, r11

    pop rbp
    ret

# =========================================================
# 2. Функция delCalc (дробная + SSE)
# =========================================================
.type delCalc, @function
delCalc:
    push rbp
    mov rbp, rsp

    # --- Конвертация float в double и перемножение ---
    cvtss2sd xmm0, xmm0         # f1 -> double
    mulsd xmm0, xmm1            # f1 * d2

    cvtss2sd xmm2, xmm2         # f3 -> double
    mulsd xmm2, xmm3            # f3 * d4

    cvtss2sd xmm4, xmm4         # f5 -> double
    mulsd xmm4, xmm5            # f5 * d6

    cvtss2sd xmm6, xmm6         # f7 -> double
    mulsd xmm6, xmm7            # f7 * d8

    # --- Расчет числителя ---
    subsd xmm0, xmm2            # (f1*d2) - (f3*d4)
    addsd xmm0, xmm4            # + (f5*d6)
    subsd xmm0, xmm6            # - (f7*d8)

    # --- Модуль числа (|num|) ---
    xorpd xmm1, xmm1
    comisd xmm0, xmm1
    jae .abs_ok
    mulsd xmm0, [.LC_MINUS_ONE]
.abs_ok:

    # --- ПРЯМАЯ SSE ИНСТРУКЦИЯ (Вместо call sqrt@PLT) ---
    sqrtsd xmm0, xmm0           # Аппаратный квадратный корень

    # --- Расчет знаменателя ---
    cvtss2sd xmm1, dword ptr [rbp + 16] # f9
    addsd xmm1, qword ptr [rbp + 24]    # + d10
    cvtss2sd xmm2, dword ptr [rbp + 32] # f11
    addsd xmm1, xmm2
    addsd xmm1, [.LC_ONE]       # + 1.0

    # --- Деление ---
    divsd xmm0, xmm1

    pop rbp
    ret

.section .rodata
.align 8
.LC_MINUS_ONE: .double -1.0
.LC_ONE:       .double 1.0