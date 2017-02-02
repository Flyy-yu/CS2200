      addi $t0, $zero, 5
      addi $t1, $zero, 10
      add $v0, $t0, $t1
      addi $s0,$zero,6
      jalr $s0,$at
      beq $zero,$zero,bran
      
      addi $t1,$t1,1
      jalr $at,$t0


      addi $s0,$zero,10
bran:
      addi $s1,$zero,2
      sw $s1,0($a1)
      lw $a0,0($a1)
      nand $s2,$a0,$t1;
      halt

      
 #ze 0000 0          a1 0000 0          t2 0000 0          k0 0000 0         
 #at 0005 5          a2 0000 0          s0 0006 6          sp 0000 0         
 #v0 000F 15         t0 0008 8          s1 0002 2          fp 0000 0         
 #a0 0002 2          t1 000B 11         s2 FFFD -3         ra 0000 0 