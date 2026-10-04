        li   $sp, 0xE0
        li   $s7, 16
        li   $t0, 0
        li   $t1, 12345
fill:   sll  $t2, $t1, 2
        add  $t1, $t1, $t2
        addi $t1, $t1, 7
        andi $t2, $t1, 0xFFFF
        sw   $t2, 0($t0)
        addi $t0, $t0, 1
        bne  $t0, $s7, fill
        jal  dump

        addi $s1, $s7, -1
outer:  li   $t0, 0
        li   $t5, 0
inner:  lw   $t1, 0($t0)
        lw   $t2, 1($t0)
        sltu $t3, $t2, $t1
        beq  $t3, $zero, keep
        sw   $t2, 0($t0)
        sw   $t1, 1($t0)
        li   $t5, 1
keep:   addi $t0, $t0, 1
        bne  $t0, $s1, inner
        beq  $t5, $zero, done
        addi $s1, $s1, -1
        bne  $s1, $zero, outer
done:   jal  dump
        halt

dump:   addi $sp, $sp, -1
        sw   $ra, 0($sp)
        li   $s0, 0
d_next: lw   $a0, 0($s0)
        jal  puthex
        li   $a0, ' '
        jal  putc
        addi $s0, $s0, 1
        bne  $s0, $s7, d_next
        li   $a0, 10
        jal  putc
        lw   $ra, 0($sp)
        addi $sp, $sp, 1
        jr   $ra

puthex: addi $sp, $sp, -1
        sw   $ra, 0($sp)
        move $t6, $a0
        li   $t7, 16
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
