        li   $sp, 0xE0
        li   $s0, 0xFFFFFF40
        li   $s1, 0xFFFFFF44
show:   lw   $a0, 0($s0)
        jal  puthex
        li   $a0, ' '
        jal  putc
        addi $s0, $s0, 1
        bne  $s0, $s1, show
        li   $a0, 10
        jal  putc

        lui  $t0, 0x5EC2
        ori  $t0, $t0, 0xE7A1
        sw   $t0, 0xFF40($zero)
        lui  $t0, 0xC0DE
        ori  $t0, $t0, 0x4719
        sw   $t0, 0xFF41($zero)
idle:   b    idle

puthex: addi $sp, $sp, -1
        sw   $ra, 0($sp)
        move $t6, $a0
        li   $t7, 32
ph_d:   addi $t7, $t7, -4
        srlv $t4, $t6, $t7
        andi $t4, $t4, 15
        addi $a0, $t4, '0'
        sltiu $t3, $t4, 10
        bne  $t3, $zero, ph_p
        addi $a0, $t4, 55
ph_p:   jal  putc
        bne  $t7, $zero, ph_d
        lw   $ra, 0($sp)
        addi $sp, $sp, 1
        jr   $ra

putc:   lw   $t9, 0xFF00($zero)
        andi $t9, $t9, 1
        beq  $t9, $zero, putc
        sw   $a0, 0xFF00($zero)
        jr   $ra
