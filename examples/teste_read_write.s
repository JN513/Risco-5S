.global _start # calculate hamming code

_start:
    jal main
    li a0, 0
    li a7, 93
    ecall

main:
    addi sp, sp, -32
    sw ra, 28(sp)
    sw s0, 24(sp)
    sw s1, 20(sp)
    sw s2, 16(sp)

    li a0, 5

    call read

    la s0, input_address
    la s1, result

decode:
    lbu t0, 0(s0) #d1
    lbu t1, 1(s0) #d2
    lbu t2, 2(s0) #d3
    lbu t3, 3(s0) #d4

    addi t0, t0, -48
    addi t1, t1, -48
    addi t2, t2, -48
    addi t3, t3, -48

    xor t4, t0, t1
    xor t4, t4, t3
    addi t4, t4, 48

    sb t4, 0(s1) # first bit - p1

    xor t4, t0, t2
    xor t4, t4, t3
    addi t4, t4, 48

    sb t4, 1(s1) # second bit - p2
    xor t4, t1, t2
    xor t4, t4, t3

    addi t0, t0, 48
    addi t1, t1, 48
    addi t2, t2, 48
    addi t3, t3, 48
    addi t4, t4, 48

    sb t0, 2(s1) # third bit
    sb t4, 3(s1) # fourth bit - p3
    sb t1, 4(s1) # fifth bit
    sb t2, 5(s1) # sixth bit
    sb t3, 6(s1) # seventh bit
    li t4, 10
    sb t4, 7(s1) # newline

    li a0, 8
    call write

encode:
    call read

    lbu t0, 0(s0) # first bit
    lbu t1, 1(s0) # second bit
    lbu t2, 2(s0) # third bit
    lbu t3, 3(s0) #  bit
    lbu t4, 4(s0) #  bit
    lbu t5, 5(s0) #  bit
    lbu t6, 6(s0) #  bit

    sb t2, 0(s1) # first bit - p1
    sb t4, 1(s1) # second bit - p2
    sb t5, 2(s1) # third bit
    sb t6, 3(s1) # fourth bit - p3
    
    li a3, 10
    sb a3, 4(s1) # fifth bit

    li a0, 5

    call write

check:
    addi t0, t0, -48
    addi t1, t1, -48
    addi t2, t2, -48
    addi t3, t3, -48
    addi t4, t4, -48
    addi t5, t5, -48
    addi t6, t6, -48

    xor a3, t0, t2
    xor a3, a3, t4
    xor a3, a3, t6

    bnez a3, error

    xor a3, t1, t2
    xor a3, a3, t5
    xor a3, a3, t6

    bnez a3, error

    xor a3, t3, t4
    xor a3, a3, t5
    xor a3, a3, t6

    bnez a3, error

    li a0, 2
    li t0, 48
    li t1, 10
    sb t0, 0(s1)
    sb t1, 1(s1)
    call write

    j end

error:
    li a0, 2
    li t0, 49
    li t1, 10
    sb t0, 0(s1)
    sb t1, 1(s1)
    call write

    j end
end:
    lw ra, 28(sp)
    lw s0, 24(sp)
    lw s1, 20(sp)
    lw s2, 16(sp)
    addi sp, sp, 32
    ret

read:
    mv a2, a0
    li a0, 0 # file descriptor = 0 (stdin)
    la a1, input_address #  buffer 
    li a7, 63 # syscall read (63)
    ecall
    ret

write:
    mv a2, a0
    li a0, 1            # file descriptor = 1 (stdout)
    la a1, result       # buffer
    li a7, 64           # syscall write (64)
    ecall
    ret

.bss

input_address: .skip 0x14
result: .skip 0x14
