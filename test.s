addi x0, xzr, #5;
ldurs s0, [x0, #0];
ldurs s1, [x0, #8];
fdivs s0, s0, s1;
sturs s0, [x0, #16];
@ stur x10, [x2, #0];