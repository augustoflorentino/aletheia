        addi $t0, $zero, 1
        sw   $t0, 0x134($zero)
        addi $t0, $zero, 30
        sw   $t0, 0x130($zero)
        addi $t0, $zero, 1
        sw   $t0, 0x132($zero)

loop:   beq  $s0, $s1, -1
        add  $s1, $s0, $zero
        addi $t0, $s1, 48
        lw   $t1, 0x100($zero)
        beq  $t1, $zero, -2
        sw   $t0, 0x100($zero)
        addi $t3, $s1, 0x10F
        sw   $t0, 0($t3)
        addi $t2, $zero, 8
        beq  $s0, $t2, 1
        j    loop
        halt

.org 0x70
        addi $s0, $s0, 1
        lw   $k0, 0x133($zero)
        addi $k1, $zero, 30
        sw   $k1, 0x130($zero)
        reti
