.data
	a: .word 0
c: .word 0
d: .word 0
m: .word 0
string_3: .asciiz " Enter three numbers: "
string_4: .asciiz " Minimum: "

	nextline: .asciiz "\n"
.text	
.globl main
	findMin: 
addiu $sp, $sp, -12

sw $ra, 0($sp)

sw $a0, 4($sp)
sw $a1, 8($sp)
if_0: 
lw $t0, 4($sp)

lw $t1, 8($sp)

slt $t0, $t0, $t1

beqz $t0, else_1
lw $t0, 4($sp)
sw $t0, m

j endif_2
else_1: lw $t0, 8($sp)
sw $t0, m

endif_2: 

lw $ra, 0($sp)

addiu $sp, $sp, 12
jr $ra
main: 


la $a0, string_3
li $v0, 4
syscall
la $a0, nextline
li $v0, 4
syscall
lw $t0, a

li $v0, 5
syscall
move $t0, $v0
sw $t0, a
lw $t0, c

li $v0, 5
syscall
move $t0, $v0
sw $t0, c
lw $t0, d

li $v0, 5
syscall
move $t0, $v0
sw $t0, d
lw $t0, a

lw $t1, c

move $a0, $t0
move $a1, $t1
jal findMin

la $a0, string_4
li $v0, 4
syscall
lw $t0, m

move $a0, $t0
li $v0, 1
syscall
la $a0, nextline
li $v0, 4
syscall
li $v0, 10
syscall

