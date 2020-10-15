;function for computing f(x)
;saved registers
; x0 : address of 'a'
; x1 : address of N
; x2 : base address of 'x' array
; x3 : base address of 'f' array
; x4 : base address of 'c' array
; x5 : addr of f(a)
; x6 : addr of extra_space

;temporary registers
;x7 = N-1
;s0 = a
;s1 = product = 1
;s2 = f(x) = result = c[0] (initially)
;x8 = cptr
;x9 = xptr

;initialization
ldur x7, [x1, #0];          load N to x7
subi x7, x7, #1;            x7 = N-1
add x8, xzr, x4;            x8 = cptr
add x9, xzr, x3;            x9 = xptr (corrected for inverse)
ldurs s2, [x8, #0];         s2 = c[0]
ldurs s0, [x0, #0];         s0 = a

addi x8, x8, #1;            cptr++
fdivs s1, s0, s0;           s1 = 1 = product (assuming a != 0)

;function start
loop:
    subi xzr, x7, #0;       check N > 0
    b.le exit;              if N <= 0 exit
    ldurs s3, [x9, #0];     s3 = *xptr
    fsubs s3, s0, s3;       s3 = x-x[i]
    fmuls s1, s1, s3;       product = product*(x-x[i])
    ldurs s3, [x8, #0];     s3 = *cptr
    fmuls s3, s1, s3;       s3 = product*(*cptr)
    fadds s2, s2, s3;       f(x) = f(x) + temp*(*cptr)

    subi x7, x7, #1;        N--
    addi x8, x8, #1;        cptr++
    addi x9, x9, #1;        xptr++
    b loop;                 callback

exit:
    sturs s2, [x5, #0];    store f(x)