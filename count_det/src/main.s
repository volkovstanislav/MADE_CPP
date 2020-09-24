.text

.global count_det
count_det:
        ldr r1, [r0]
        ldr r2, [r0, #4]
        ldr r3, [r0, #8]
        ldr r4, [r0, #12]
        ldr r5, [r0, #16]
        ldr r6, [r0, #20]
        ldr r7, [r0, #24]
        ldr r8, [r0, #28]
        ldr r9, [r0, #32]
        mul r10, r1, r5
        mul r10, r10, r9
        mul r12, r7, r2
        mul r12, r12, r6
        add r10, r10, r12
        mul r12, r4, r8
        mul r12, r12, r3
        add r10, r10, r12
        mul r12, r7, r5
        mul r12, r12, r3
        sub r10, r10, r12
        mul r12, r1, r8
        mul r12, r12, r6
        sub r10, r10, r12
        mul r12, r4, r2
        mul r12, r12, r9
        sub r10, r10, r12
        mov r0, r10
        bx lr

