        j    kernel

.org 0x10
        li   $a0, 'U'
        j    trap
.org 0x50
        li   $a0, 'A'
        j    trap
.org 0x70
        reti

trap:   jal  putc
        lw   $a0, 0x01($zero)
        jal  putc
        li   $a0, 10
        jal  putc
        addi $s7, $s7, 1
        li   $t0, 1
        beq  $s7, $t0, to_u2
        li   $t0, 2
        beq  $s7, $t0, to_u3
        halt
to_u2:  sw   $t0, 0xFF37($zero)
        j    user2
to_u3:  li   $t0, 1
        sw   $t0, 0xFF37($zero)
        j    user3

kernel: lui  $t0, 0x5EC2
        ori  $t0, $t0, 0xE7A1
        sw   $t0, 0x10($zero)
        li   $t0, 42
        sw   $t0, 0x00($zero)
        lui  $t0, 0x0108
        sw   $t0, 0xFF35($zero)
        lui  $t0, 0x0110
        ori  $t0, $t0, 1
        sw   $t0, 0xFF35($zero)
        li   $t0, 1
        sw   $t0, 0xFF36($zero)
        sw   $t0, 0xFF37($zero)

user1:  lw   $t1, 0x00($zero)
        addi $t1, $t1, 37
        sw   $t1, 0x01($zero)
        lw   $t2, 0x10($zero)
        sw   $t2, 0x02($zero)
user2:  li   $t1, 'B'
        sw   $t1, 0x01($zero)
        sw   $t1, 0xFF00($zero)
user3:  li   $t1, 'C'
        sw   $t1, 0x01($zero)
        reti

putc:   lw   $t9, 0xFF00($zero)
        andi $t9, $t9, 1
        beq  $t9, $zero, putc
        sw   $a0, 0xFF00($zero)
        jr   $ra
