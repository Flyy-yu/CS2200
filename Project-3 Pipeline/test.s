	addi $s0,$zero,10
loop:
	addi $a0,$a0,1 
	beq $s0,$a0,nextt
	beq $zero,$zero,loop
nextt:
	beq $s2,$s0,end
	lw $s2,0($zero)
	addi $s2,$s2,1
	sw $s2,0($zero)
	beq $zero,$zero,nextt
end:
	add $zero,$zero,$zero