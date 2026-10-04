        addi $s0, $zero, 0
        jal  getc
        addi $t1, $zero, '4'
        bne  $v0, $t1, deny
        jal  getc
        addi $t1, $zero, '7'
        bne  $v0, $t1, deny
        jal  getc
        addi $t1, $zero, '1'
        bne  $v0, $t1, deny
        jal  getc
        addi $t1, $zero, '9'
        bne  $v0, $t1, deny

.org 0x20
grant:  addi $a0, $zero, 'O'
        jal  putc
        addi $a0, $zero, 'P'
        jal  putc
        addi $a0, $zero, 'E'
        jal  putc
        addi $a0, $zero, 'N'
        jal  putc
        addi $a0, $zero, 10
        jal  putc
        halt

.org 0x30
deny:   addi $a0, $zero, 'D'
        jal  putc
        addi $a0, $zero, 'E'
        jal  putc
        addi $a0, $zero, 'N'
        jal  putc
        addi $a0, $zero, 'Y'
        jal  putc
        addi $a0, $zero, 10
        jal  putc
        halt

putc:   lw   $t9, 0xFF00($zero)
        andi $t9, $t9, 1
        beq  $t9, $zero, putc
        sw   $a0, 0xFF00($zero)
        jr   $ra

getc:   lw   $t9, 0xFF00($zero)
        andi $t9, $t9, 2
        beq  $t9, $zero, getc
        lw   $v0, 0xFF01($zero)
        jr   $ra
