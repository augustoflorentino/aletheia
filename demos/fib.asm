        li   $sp, 0xE0
        li   $t0, 1
        li   $t1, 0xF0
        li   $t2, 0xFA
pow:    sw   $t0, 0($t1)
        sll  $t3, $t0, 3
        sll  $t4, $t0, 1
        add  $t0, $t3, $t4
        addi $t1, $t1, 1
        bne  $t1, $t2, pow

        li   $s0, 0
        li   $s1, 1
        li   $s2, 30
loop:   move $a0, $s0
        jal  putdec
        li   $a0, ' '
        jal  putc
        add  $t0, $s0, $s1
        move $s0, $s1
        move $s1, $t0
        addi $s2, $s2, -1
        bne  $s2, $zero, loop
        li   $a0, 10
        jal  putc
        halt

putdec: addi $sp, $sp, -1
        sw   $ra, 0($sp)
        move $t5, $a0
        li   $t6, 0xF9
        li   $t7, 0
pd_pow: lw   $t1, 0($t6)
        li   $t2, 0
pd_sub: sltu $t3, $t5, $t1
        bne  $t3, $zero, pd_dig
        sub  $t5, $t5, $t1
        addi $t2, $t2, 1
        b    pd_sub
pd_dig: or   $t7, $t7, $t2
        li   $t4, 0xF0
        beq  $t6, $t4, pd_out
        beq  $t7, $zero, pd_next
pd_out: addi $a0, $t2, '0'
        jal  putc
pd_next:
        li   $t4, 0xF0
        beq  $t6, $t4, pd_end
        addi $t6, $t6, -1
        b    pd_pow
pd_end: lw   $ra, 0($sp)
        addi $sp, $sp, 1
        jr   $ra

putc:   lw   $t9, 0xFF00($zero)
        andi $t9, $t9, 1
        beq  $t9, $zero, putc
        sw   $a0, 0xFF00($zero)
        jr   $ra
