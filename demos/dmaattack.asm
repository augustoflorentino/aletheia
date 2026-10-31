        j    kernel

.org 0x10
        j    syscall
.org 0x50
        j    syscall
.org 0x70
        reti

syscall:
        lw   $s0, 0x02($zero)
        li   $s1, 32
hex:    addi $s1, $s1, -4
        srlv $a0, $s0, $s1
        andi $a0, $a0, 15
        addi $t3, $a0, -10
        addi $a0, $a0, '0'
        slt  $t3, $t3, $zero
        bne  $t3, $zero, out
        addi $a0, $a0, 7
out:    jal  putc
        bne  $s1, $zero, hex
        li   $a0, 10
        jal  putc
        halt

kernel: lui  $t0, 0x5EC2
        ori  $t0, $t0, 0xE7A1
        sw   $t0, 0x10($zero)
        lui  $t0, 0x0108
        sw   $t0, 0xFF35($zero)
        lui  $t0, 0x0110
        ori  $t0, $t0, 1
        sw   $t0, 0xFF35($zero)
        li   $t0, 1
        sw   $t0, 0xFF36($zero)
        sw   $t0, 0xFF37($zero)

user:   li   $t1, 2000
spin:   lw   $t0, 0x02($zero)
        bne  $t0, $zero, leak
        addi $t1, $t1, -1
        bne  $t1, $zero, spin
leak:   reti

putc:   lw   $t9, 0xFF00($zero)
        andi $t9, $t9, 1
        beq  $t9, $zero, putc
        sw   $a0, 0xFF00($zero)
        jr   $ra
