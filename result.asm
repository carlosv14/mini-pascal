.data
	string_2: .asciiz "a="
string_5: .asciiz "Element["
string_6: .asciiz "] = "

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

lw $t1, 40($sp)

li $a0, 4
mult $a0, $t1
mflo $t2
la $t3, 0($sp)
add $t2, $t2, $t3
lw $t0, 40($sp)

li $t1, 100

add $t0, $t0, $t1


sw $t0, 0($t2)
lw $t0, 40($sp)

li $a0, 4
mult $a0, $t0
mflo $t0
la $t1, 0($sp)
add $t0, $t1, $t0
lw $t0, 0($t0)
sw $t0, 48($sp)

la $a0, string_2
li $v0, 4
syscall
lw $t0, 48($sp)

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
lw $t0, 40($sp)

li $t2, 1

add $t0, $t0, $t2

sw $t0, 40($sp)

j while_0
endWhile_1: 
li $t0, 1
sw $t0, 44($sp)
while_3: 
lw $t0, 44($sp)

li $t2, 10

slt $t0, $t0, $t2

beqz $t0, endWhile_4

la $a0, string_5
li $v0, 4
syscall
lw $t0, 44($sp)

move $a0, $t0
li $v0, 1
syscall

la $a0, string_6
li $v0, 4
syscall
lw $t0, 44($sp)

li $a0, 4
mult $a0, $t0
mflo $t0
la $t2, 0($sp)
add $t0, $t2, $t0
lw $t0, 0($t0)

move $a0, $t0
li $v0, 1
syscall
la $a0, nextline
li $v0, 4
syscall
lw $t0, 44($sp)

li $t3, 1

add $t0, $t0, $t3

sw $t0, 44($sp)

j while_3
endWhile_4: 

li $v0, 10
syscall

