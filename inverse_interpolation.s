;x and y values reversed before hand.
;for inverse to exist function has to be invertible.
;saved registers
; x0 : address of 'a'
; x1 : address of N
; x2 : base address of 'x' array
; x3 : base address of 'f' array
; x4 : base address of 'c' array
; x5 : addr of f(a)
; x6 : addr of extra_space

;temporary registers
; x10 : i
; x11 : j
; x12 : N
; x13 : xptr
; x14 : fptr
; x15: mat

;initialization (reversing x and f(x))
add x10, xzr, xzr;          i = 0
add x14, xzr, x2;           fptr
ldur x12, [x1, #0];         N
add x15, xzr, x6;           mat

;function start

loop1:
    sub xzr, x10, x12;      i<N
    b.ge end_loop1;
    ldurs s0, [x14, #0];    s0 = *fptr
    sturs s0, [x15, #0];    mat[i] = f[i]
    addi x14, x14, #1;      fptr++
    addi x15, x15, #1;      mat++
    addi x10, x10, #1;      i++
    b loop1;                callback

end_loop1:
    addi x10, xzr, #1;      i = 1

loop2:
    sub xzr, x10, x12;      xzr = i-N
    b.ge end_loop2;         
    add x11, xzr, x10;      j = i
    lsl x16, x10, #0;       x16 = i*8
    add x13, x3, x16;       x13 = &x[i]
    add x15, x15, x16;      mat += i

loop3:
    sub xzr, x11, x12;      xzr = j-N
    b.ge end_loop3;
    lsl x16, x12, #0;       x16 = N*8
    sub x16, x15, x16;      x16 = &mat[i-1][j]
    ldurs s0, [x16, #0];    s0 = mat[i-1][j]
    subi x16, x16, #1;      x16--
    ldurs s1, [x16, #0];    s1 = mat[i-1][j-1]
    fsubs s0, s0, s1;       s0 = mat[i-1][j] - mat[i-1][j-1] (N)
    ldurs s1, [x13, #0];    s1 = x[j]
    lsl x16, x10, #0;       x16 = i*8
    sub x16, x13, x16;      x16 = &x[j-i]
    ldurs s2, [x16, #0];    s2 = x[j-i]
    fsubs s1, s1, s2;       s1 = x[j] - x[j-i] (D)
    fdivs s0, s0, s1;       s0 = N/D
    sturs s0, [x15, #0];    mat[i][j] = N/D

    addi x15, x15, #1;      mat++
    addi x11, x11, #1;      j++
    addi x13, x13, #1;      xptr++
    b loop3;                callback for loop3

end_loop3:
    addi x10, x10, #1;      i++
    b loop2;                callback for loop2

end_loop2:
    add x10, xzr,xzr;       i = 0
    add x15, xzr, x6;       mat = &empty_space
    add x16, xzr, x4;       x16 = &c

loop4:
    sub xzr, x10, x12;      xzr = i-N
    b.ge exit;
    ldurs s0, [x15, #0];    s0 = mat[i][i]
    sturs s0, [x16, #0];    c[i] = mat[i][i]

    addi x16, x16, #1;      c++
    add x15, x15, x12;      mat += N
    addi x15, x15, #1;      mat++
    addi x10, x10, #1;      i++
    b loop4;

exit:
    add xzr, xzr, xzr;      nothing




