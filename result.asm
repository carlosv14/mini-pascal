.data
	string_2: .asciiz "value of a: "

	nextline: .asciiz "\n"
.text	
.globl main
	main: 
li $t0, 10
sw $t0, 0($sp)
while_0: 
lw $t0, 0($sp)

li $t1, 20

slt $t0, $t0, $t1

beqz $t0, endWhile_1
la $a0, string_2
li $v0, 4
syscall
lw $t0, 0($sp)

move $a0, $t0
li $v0, 1
syscall
la $a0, nextline
li $v0, 4
syscall
lw $t0, 0($sp)

li $t1, 1

add $t0, $t0, $t1

sw $t0, 0($sp)

j while_0
endWhile_1: 

li $v0, 10
syscall

