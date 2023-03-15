.data
	string_4: .asciiz "Element["
string_5: .asciiz "] = "

	nextline: .asciiz "\n"
.text	
.globl main
	main: 
li $t0, 1
sw $t0, 40($sp)
li $t0, 1
sw $t0, 44($sp)
while_0: 
lw $t0, 44($sp)

li $t1, 10

slt $t0, $t0, $t1

beqz $t0, endWhile_1
lw $t0, 40($sp)

li $t1, 100

add $t0, $t0, $t1

sw $t0, 0($sp)
lw $t0, 44($sp)

li $t1, 1

add $t0, $t0, $t1

sw $t0, 44($sp)
lw $t0, 40($sp)

li $t1, 1

add $t0, $t0, $t1

sw $t0, 40($sp)

j while_0
endWhile_1: 
li $t0, 1
sw $t0, 44($sp)
while_2: 
lw $t0, 44($sp)

li $t1, 10

slt $t0, $t0, $t1

beqz $t0, endWhile_3
la $a0, string_4
li $v0, 4
syscall
lw $t0, 44($sp)

move $a0, $t0
li $v0, 1
syscall

la $a0, string_5
li $v0, 4
syscall
lw $t0, 44($sp)

li $a0, 4
mult $a0, $t0
mflo $t0
la $t1, 0($sp)
add $t0, $t1, $t0
lw $t0, 0($t0)

move $a0, $t0
li $v0, 1
syscall
la $a0, nextline
li $v0, 4
syscall
lw $t0, 44($sp)

li $t2, 1

add $t0, $t0, $t2

sw $t0, 44($sp)

j while_2
endWhile_3: 

li $v0, 10
syscall

