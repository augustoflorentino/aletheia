        j    boot

.org 0x10
        addi $s6, $zero, 'U'
        j    fault
.org 0x50
        addi $s6, $zero, 'A'
        j    fault
.org 0x70
        reti

fault:  addi $sp, $zero, 0x100
        addi $a0, $zero, 10
        jal  putc
        addi $a0, $zero, '!'
        jal  putc
        add  $a0, $s6, $zero
        jal  putc
        j    prompt

boot:   addi $sp, $zero, 0x100
        sw   $zero, 0xFF34($zero)
        sw   $zero, 0xFF32($zero)

        jal  newline
        addi $a0, $zero, 'A'
        jal  putc
        addi $a0, $zero, 'L'
        jal  putc
        addi $a0, $zero, 'E'
        jal  putc
        addi $a0, $zero, 'T'
        jal  putc
        addi $a0, $zero, 'H'
        jal  putc
        addi $a0, $zero, 'E'
        jal  putc
        addi $a0, $zero, 'I'
        jal  putc
        addi $a0, $zero, 'A'
        jal  putc
        jal  space
        addi $a0, $zero, '3'
        jal  putc
        addi $a0, $zero, '2'
        jal  putc
        jal  newline

        addi $t0, $zero, 'A'
        sw   $t0, 0xFF10($zero)
        addi $t0, $zero, 'L'
        sw   $t0, 0xFF11($zero)
        addi $t0, $zero, 'E'
        sw   $t0, 0xFF12($zero)
        addi $t0, $zero, 'T'
        sw   $t0, 0xFF13($zero)
        addi $t0, $zero, 'H'
        sw   $t0, 0xFF14($zero)
        addi $t0, $zero, 'E'
        sw   $t0, 0xFF15($zero)
        addi $t0, $zero, 'I'
        sw   $t0, 0xFF16($zero)
        addi $t0, $zero, 'A'
        sw   $t0, 0xFF17($zero)

post_ram:
        addi $a0, $zero, 'R'
        jal  putc
        addi $a0, $zero, 'A'
        jal  putc
        addi $a0, $zero, 'M'
        jal  putc
        jal  space
        addi $t1, $zero, 0x100
        addi $t2, $zero, 1
ram_sz: sw   $zero, 0($zero)
        sw   $t2, 0($t1)
        lw   $t3, 0($zero)
        bne  $t3, $zero, ram_go
        sll  $t1, $t1, 1
        j    ram_sz
ram_go: lui  $s0, 0xA5A5
        addi $t0, $zero, 0
ram_w:  xor  $t2, $s0, $t0
        sw   $t2, 0($t0)
        addi $t0, $t0, 1
        bne  $t0, $t1, ram_w
        addi $t0, $zero, 0
ram_r:  xor  $t2, $s0, $t0
        lw   $t3, 0($t0)
        bne  $t2, $t3, ram_bad
        nor  $t2, $t2, $zero
        sw   $t2, 0($t0)
        lw   $t3, 0($t0)
        bne  $t2, $t3, ram_bad
        sw   $zero, 0($t0)
        addi $t0, $t0, 1
        bne  $t0, $t1, ram_r
        add  $a0, $t1, $zero
        addi $a1, $zero, 4
        jal  puthex
        addi $a0, $zero, 'W'
        jal  putc
        jal  ok
        j    post_timer
ram_bad:
        add  $a0, $t0, $zero
        addi $a1, $zero, 4
        jal  puthex
        jal  bad

post_timer:
        addi $a0, $zero, 'T'
        jal  putc
        addi $a0, $zero, 'I'
        jal  putc
        addi $a0, $zero, 'M'
        jal  putc
        addi $a0, $zero, 'E'
        jal  putc
        addi $a0, $zero, 'R'
        jal  putc
        lw   $zero, 0xFF33($zero)
        addi $t0, $zero, 4
        sw   $t0, 0xFF30($zero)
        addi $t0, $zero, 1
        sw   $t0, 0xFF32($zero)
        addi $t1, $zero, 64
tim_w:  lw   $t2, 0xFF33($zero)
        bne  $t2, $zero, tim_ok
        addi $t1, $t1, -1
        bne  $t1, $zero, tim_w
        sw   $zero, 0xFF32($zero)
        jal  bad
        j    post_uart
tim_ok: sw   $zero, 0xFF32($zero)
        jal  ok

post_uart:
        addi $a0, $zero, 'U'
        jal  putc
        addi $a0, $zero, 'A'
        jal  putc
        addi $a0, $zero, 'R'
        jal  putc
        addi $a0, $zero, 'T'
        jal  putc
        jal  ok

prompt: addi $sp, $zero, 0x100
        jal  newline
        addi $a0, $zero, '>'
        jal  putc
        jal  space

        addi $s0, $zero, 0xC0
        addi $s1, $zero, 0xDF
read:   jal  getc
        addi $t0, $v0, -10
        beq  $t0, $zero, line
        addi $t0, $v0, -13
        beq  $t0, $zero, line
        addi $t0, $v0, -8
        beq  $t0, $zero, rubout
        addi $t0, $v0, -127
        beq  $t0, $zero, rubout
        beq  $s0, $s1, read
        sw   $v0, 0($s0)
        addi $s0, $s0, 1
        add  $a0, $v0, $zero
        jal  putc
        j    read
rubout: addi $t0, $zero, 0xC0
        beq  $s0, $t0, read
        addi $s0, $s0, -1
        addi $a0, $zero, 8
        jal  putc
        jal  space
        addi $a0, $zero, 8
        jal  putc
        j    read

line:   sw   $zero, 0($s0)
        addi $s0, $zero, 0xC0
        jal  skipsp
        lw   $s2, 0($s0)
        beq  $s2, $zero, prompt
        addi $s0, $s0, 1
        ori  $s2, $s2, 0x20
        addi $t0, $s2, -109
        beq  $t0, $zero, cmd_m
        addi $t0, $s2, -119
        beq  $t0, $zero, cmd_w
        addi $t0, $s2, -103
        beq  $t0, $zero, cmd_g
error:  jal  newline
        addi $a0, $zero, '?'
        jal  putc
        j    prompt

cmd_m:  jal  hexarg
        beq  $v1, $zero, error
        add  $s3, $v0, $zero
        jal  hexarg
        add  $s4, $v0, $zero
        bne  $v1, $zero, m_chk
        add  $s4, $s3, $zero
m_chk:  sltu $t0, $s4, $s3
        beq  $t0, $zero, m_row
        add  $s4, $s3, $zero
m_row:  jal  newline
        add  $a0, $s3, $zero
        addi $a1, $zero, 8
        jal  puthex
        addi $a0, $zero, ':'
        jal  putc
m_word: jal  space
        lw   $a0, 0($s3)
        addi $a1, $zero, 8
        jal  puthex
        beq  $s3, $s4, prompt
        addi $s3, $s3, 1
        andi $t0, $s3, 3
        bne  $t0, $zero, m_word
        j    m_row

cmd_w:  jal  hexarg
        beq  $v1, $zero, error
        add  $s3, $v0, $zero
        jal  hexarg
        beq  $v1, $zero, error
w_next: sw   $v0, 0($s3)
        addi $s3, $s3, 1
        jal  hexarg
        bne  $v1, $zero, w_next
        j    prompt

cmd_g:  jal  hexarg
        beq  $v1, $zero, error
        jalr $v0
        j    prompt

hexarg: addi $sp, $sp, -1
        sw   $ra, 0($sp)
        jal  skipsp
        addi $v0, $zero, 0
        addi $v1, $zero, 0
hx_c:   lw   $t0, 0($s0)
        addi $t1, $t0, -48
        sltiu $t2, $t1, 10
        bne  $t2, $zero, hx_d
        ori  $t0, $t0, 0x20
        addi $t1, $t0, -87
        addi $t2, $t0, -97
        sltiu $t2, $t2, 6
        beq  $t2, $zero, hx_end
hx_d:   sll  $v0, $v0, 4
        or   $v0, $v0, $t1
        addi $v1, $v1, 1
        addi $s0, $s0, 1
        j    hx_c
hx_end: lw   $ra, 0($sp)
        addi $sp, $sp, 1
        jr   $ra

skipsp: lw   $t0, 0($s0)
        addi $t0, $t0, -32
        bne  $t0, $zero, sk_end
        addi $s0, $s0, 1
        j    skipsp
sk_end: jr   $ra

puthex: addi $sp, $sp, -3
        sw   $ra, 0($sp)
        sw   $s0, 1($sp)
        sw   $s1, 2($sp)
        add  $s0, $a0, $zero
        sll  $s1, $a1, 2
ph_d:   addi $s1, $s1, -4
        srlv $t0, $s0, $s1
        andi $t0, $t0, 15
        addi $a0, $t0, 48
        sltiu $t1, $t0, 10
        bne  $t1, $zero, ph_p
        addi $a0, $t0, 55
ph_p:   jal  putc
        bne  $s1, $zero, ph_d
        lw   $ra, 0($sp)
        lw   $s0, 1($sp)
        lw   $s1, 2($sp)
        addi $sp, $sp, 3
        jr   $ra

ok:     addi $sp, $sp, -1
        sw   $ra, 0($sp)
        jal  space
        addi $a0, $zero, 'O'
        jal  putc
        addi $a0, $zero, 'K'
        j    tail
bad:    addi $sp, $sp, -1
        sw   $ra, 0($sp)
        jal  space
        addi $a0, $zero, 'F'
        jal  putc
        addi $a0, $zero, 'A'
        jal  putc
        addi $a0, $zero, 'I'
        jal  putc
        addi $a0, $zero, 'L'
tail:   jal  putc
        jal  newline
        lw   $ra, 0($sp)
        addi $sp, $sp, 1
        jr   $ra

newline:
        addi $a0, $zero, 10
        j    putc
space:  addi $a0, $zero, 32
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
