!=================================================================
! General conventions:
!   1) Stack grows from high addresses to low addresses, and the
!      top of the stack points to valid data
!
!   2) Register usage is as implied by assembler names and manual
!
!   3) Function Calling Convention:
!
!       Setup)
!       * Immediately upon entering a function, push the RA on the stack.
!       * Next, push all the registers used by the function on the stack.
!
!       Teardown)
!       * Load the return value in $v0.
!       * Pop any saved registers from the stack back into the registers.
!       * Pop the RA back into $ra.
!       * Return by executing jalr $ra, $zero.
!=================================================================

!vector table
vector0:    .fill 0x00000000 !0
            .fill 0x00000000 !1
            .fill 0x00000000 !2
            .fill 0x00000000
            .fill 0x00000000 !4
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000 !8
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000
            .fill 0x00000000 !15
!end vector table

main:           la $sp, stack           ! Initialize stack pointer
                lw $sp, 0($sp)          
                la $s0, ti_inthandler   ! Load address of int handler to s0
                sw $s0, 0x1($zero)  
                la $s0, ti_keyhandler   ! Load address of int handler to s0
                sw $s0, 0x3($zero) 
                ! Install timer interrupt handler into vector table
                ei                      ! Dont forget to enable interrupts...
                la $at, factorial       ! load address of factorial label into $at
                addi $a0, $zero, 5      ! $a0 = 5, the number to factorialize
                jalr $at, $ra           ! jump to factorial, set $ra to return addr
                halt                    ! when we return, just halt

factorial:      addi    $sp, $sp, -1    ! push RA
                sw      $ra, 0($sp)
                addi    $sp, $sp, -1    ! push a0
                sw      $a0, 0($sp)
                addi    $sp, $sp, -1    ! push s0
                sw      $s0, 0($sp)
                addi    $sp, $sp, -1    ! push s1
                sw      $s1, 0($sp)

                beq     $a0, $zero, base_zero
                addi    $s1, $zero, 1
                beq     $a0, $s1, base_one
                beq     $zero, $zero, recurse
                
    base_zero:  addi    $v0, $zero, 1   ! 0! = 1
                beq     $zero, $zero, done

    base_one:   addi    $v0, $zero, 1   ! 1! = 1
                beq     $zero, $zero, done

    recurse:    add     $s1, $a0, $zero     ! save n in s1
                addi    $a0, $a0, -1        ! n! = n * (n-1)!
                la      $at, factorial
                jalr    $at, $ra

                add     $s0, $v0, $zero     ! use s0 to store (n-1)!
                add     $v0, $zero, $zero   ! use v0 as sum register
        mul:    beq     $s1, $zero, done    ! use s1 as counter (from n to 0)
                add     $v0, $v0, $s0
                addi    $s1, $s1, -1
                beq     $zero, $zero, mul

    done:       lw      $s1, 0($sp)     ! pop s1
                addi    $sp, $sp, 1
                lw      $s0, 0($sp)     ! pop s0
                addi    $sp, $sp, 1
                lw      $a0, 0($sp)     ! pop a0
                addi    $sp, $sp, 1
                lw      $ra, 0($sp)     ! pop RA
                addi    $sp, $sp, 1
                jalr    $ra, $zero

ti_keyhandler:


addi $sp,$sp,100
    sw $at,0($sp)
    sw $v0, -1($sp)
    sw $a0, -2($sp)
    sw $a1, -3($sp)
    sw $a2, -4($sp)
    sw $a3, -5($sp)
    sw $a4, -6($sp)
    sw $s0, -7($sp)
    sw $s1, -8($sp)
    sw $s2, -9($sp)
    sw $s3, -10($sp)
    sw $k0, -11($sp)
    sw $fp, -12($sp)
    sw $ra, -13($sp)
    
    la $s1,keyboard
    lw $s0,0($s1)
    boni $s2,$s0,0
    addi $s0,$s0,1
    sw $s0,0($s1)
    ei
     lw $at, 0($sp)      ! load all of the registers back
    lw $v0, -1($sp)
    lw $a0, -2($sp)
    lw $a1, -3($sp)
    lw $a2, -4($sp)
    lw $a3, -5($sp)
    lw $a4, -6($sp)
    lw $s0, -7($sp)
    lw $s1, -8($sp)
    lw $s2, -9($sp)
    lw $s3, -10($sp)
    lw $k0, -11($sp)
    lw $fp, -12($sp)
    lw $ra, -13($sp)
    addi $sp,$sp,-100
    di
    reti

ti_inthandler:
    addi $sp,$sp,100
    sw $at,0($sp)
    sw $v0, -1($sp)
    sw $a0, -2($sp)
    sw $a1, -3($sp)
    sw $a2, -4($sp)
    sw $a3, -5($sp)
    sw $a4, -6($sp)
    sw $s0, -7($sp)
    sw $s1, -8($sp)
    sw $s2, -9($sp)
    sw $s3, -10($sp)
    sw $k0, -11($sp)
    sw $fp, -12($sp)
    sw $ra, -13($sp)
    ei
 
  la $s0,seconds
    lw $s0,0($s0)
    lw $s1,0($s0)
    addi $s1,$s1,1
    addi $s2,$zero,60
    beq $s1,$s2,Min
    sw $s1,0($s0)
    beq $zero, $zero, End
Min:
    sw $zero,0($s0)
    la $s0,minutes
    lw $s0,0($s0)
    lw $s1,0($s0)
    addi $s1,$s1,1
    addi $s2,$zero,60
    beq $s1,$s2,Hou
    sw $s1,0($s0)
    beq $zero, $zero, End
Hou:
    sw $zero,0($s0)
    la $s0,hours
    lw $s0,0($s0)
    lw $s1,0($s0)
    addi $s1,$s1,1
    sw $s1,0($s0)
End:
    lw $at, 0($sp)      ! load all of the registers back
    lw $v0, -1($sp)
    lw $a0, -2($sp)
    lw $a1, -3($sp)
    lw $a2, -4($sp)
    lw $a3, -5($sp)
    lw $a4, -6($sp)
    lw $s0, -7($sp)
    lw $s1, -8($sp)
    lw $s2, -9($sp)
    lw $s3, -10($sp)
    lw $k0, -11($sp)
    lw $fp, -12($sp)
    lw $ra, -13($sp)
    addi $sp,$sp,-100
    di
    reti

keyboard:   .fill 0xEEEEE0
stack:      .fill 0xA00000
seconds:    .fill 0xFFFFFC
minutes:    .fill 0xFFFFFD
hours:      .fill 0xFFFFFE
