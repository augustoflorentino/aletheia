        li   $sp, 0xE0
        li   $t0, 0
        li   $t1, 8
        lui  $t2, 0xD0
fill:   or   $t3, $t2, $t0
        sw   $t3, 0($t0)
        addi $t0, $t0, 1
        bne  $t0, $t1, fill

        sw   $zero, 0xFF38($zero)
        li   $t0, 0x40
        sw   $t0, 0xFF39($zero)
        sw   $t1, 0xFF3A($zero)
        li   $t0, 1
        sw   $t0, 0xFF3B($zero)
wait:   lw   $t0, 0xFF3B($zero)
        andi $t0, $t0, 2
        beq  $t0, $zero, wait
        sw   $zero, 0xFF3B($zero)

        li   $s0, 0x40
        li   $s1, 0x48
show:   lw   $a0, 0($s0)
        jal  puthex
        li   $a0, ' '
        jal  putc
        addi $s0, $s0, 1
        bne  $s0, $s1, show
        li   $a0, 10
        jal  putc
        halt

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
