        addi $t0, $zero, 1
        sw   $t0, 0xFF34($zero)
        addi $t0, $zero, 200
        sw   $t0, 0xFF30($zero)
        addi $t0, $zero, 1
        sw   $t0, 0xFF32($zero)

loop:   beq  $s0, $s1, -1
        add  $s1, $s0, $zero
        addi $t0, $s1, 48
        lw   $t1, 0xFF00($zero)
        beq  $t1, $zero, -2
        sw   $t0, 0xFF00($zero)
        addi $t3, $s1, 0xFF0F
        sw   $t0, 0($t3)
        slti $t2, $s1, 8
        beq  $t2, $zero, 1
        j    loop
        halt

.org 0x70
        addi $s0, $s0, 1
        lw   $k0, 0xFF33($zero)
        addi $k1, $zero, 200
        sw   $k1, 0xFF30($zero)
        reti
