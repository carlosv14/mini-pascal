.data
	n: .space 40
i: .word 0
w: .word 0
a: .word 0
string_4: .asciiz "Element["
string_5: .asciiz "] = "

	nextline: .asciiz "\n"
.text	
.globl main
	main: 

li $t0, 1
sw $t0, i
li $t0, 1
sw $t0, w
while_0: 
lw $t0, w

li $t1, 10

slt $t0, $t0, $t1

beqz $t0, endWhile_1
lw $t0, i

li $t1, 100

add $t0, $t0, $t1

lw $t1, i

li $a0, 4
mult $a0, $t1
mflo $t2
la $t3, n
add $t2, $t2, $t3
lw $t0, i

li $t1, 100

add $t0, $t0, $t1


sw $t0, 0($t2)
lw $t0, w

li $t1, 1

add $t0, $t0, $t1

sw $t0, w
lw $t0, i

li $t1, 1

add $t0, $t0, $t1

sw $t0, i

j while_0
endWhile_1: 
li $t0, 1
sw $t0, w
while_2: 
lw $t0, w

li $t1, 10

slt $t0, $t0, $t1

beqz $t0, endWhile_3

la $a0, string_4
li $v0, 4
syscall
lw $t0, w

move $a0, $t0
li $v0, 1
syscall

la $a0, string_5
li $v0, 4
syscall
lw $t0, w

li $a0, 4
mult $a0, $t0
mflo $t0
la $t1, n
add $t0, $t1, $t0
lw $t0, 0($t0)

move $a0, $t0
li $v0, 1
syscall
la $a0, nextline
li $v0, 4
syscall
lw $t0, w

li $t1, 1

add $t0, $t0, $t1

sw $t0, w

j while_2
endWhile_3: 
li $v0, 10
syscall

