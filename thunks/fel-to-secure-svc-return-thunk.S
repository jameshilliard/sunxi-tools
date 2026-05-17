/* H616 secure-FEL monitor-to-SVC return thunk. */

.arm

BUF1		.req	r0
BUF2		.req	r1
TMP1		.req	r2
TMP2		.req	r3
SWAPTBL		.req	r4
BUFSIZE		.req	r6
ENTRY_ADDR	.req	r8
SAVED_SP	.req	r10
SAVED_LR	.req	r11

.set	SUNXI_GICC_BASE,	0x03022000
.set	SUNXI_GICD_BASE,	0x03021000

entry_point:
	b	setup_stack

stack_begin:
	.space	32, 0xff
stack_end:
	nop

	/* A function, which walks the table and swaps all buffers */
swap_all_buffers:
	adr	SWAPTBL,   appended_data + 4
swap_next_buffer:
	ldr	BUF1,      [SWAPTBL],  #4
	ldr	BUF2,      [SWAPTBL],  #4
	ldr	BUFSIZE,   [SWAPTBL],  #4
	cmp	BUFSIZE,   #0
	bxeq    lr
swap_next_word:
	ldr	TMP1,      [BUF1]
	ldr	TMP2,      [BUF2]
	subs	BUFSIZE,   BUFSIZE,    #4
	str	TMP1,      [BUF2],     #4
	str	TMP2,      [BUF1],     #4
	bne	swap_next_word
	b	swap_next_buffer

setup_stack: /* Save the original SP, LR and CPSR to stack */
	ldr	ENTRY_ADDR, appended_data
	adr	BUF1,      stack_end
	str	sp,        [BUF1, #-4]!
	mov	sp,        BUF1
	mrs	TMP1,      cpsr
	push	{TMP1, lr}

	/* Disable IRQ and FIQ */
	orr	TMP1,      #0xc0
	msr	cpsr_c,    TMP1

	/* Check if the instructions or data cache is enabled */
	mrc	p15, 0, TMP1, c1, c0, 0
	tst	TMP1,      #(1 << 2)
	tsteq	TMP1,      #(1 << 12)
	bne	return_to_fel_noswap

	bl	swap_all_buffers
	bl	call_entry
	b	return_to_secure_fel

return_to_secure_fel:
	bl	swap_all_buffers
	bl	setup_secure_gic

return_to_fel_noswap:
	pop	{TMP1, lr}
	msr	cpsr_c,    TMP1 /* Restore the original CPSR */
	ldr     sp,        [sp]
	bx	lr

call_entry:
	mcr	p15, 0, TMP1, c7, c10, 4	/* CP15DSB */
	mrc	p15, 0, TMP1, c0, c0, 0		/* read MIDR */
	and	TMP1, TMP1, #(0xf << 16)	/* architecture */
	cmp	TMP1, #(0x6 << 16)		/* ARMv5TEJ */
	mcrgt	p15, 0, TMP1, c7, c5, 4		/* CP15ISB, if > ARMv5TEJ */
	mov	SAVED_SP,  sp
	mov	SAVED_LR,  lr
	.word	0xe1600070			/* smc	#0 */
	mov	TMP1,      #0
	mcr	p15, 0, TMP1, c1, c1, 0		/* SCR = secure */
	mcr	p15, 0, TMP1, c12, c0, 1	/* MVBAR = 0 */
	bl	setup_secure_gic

	.word	0xe123f30a			/* msr	SP_svc, SAVED_SP */
	.word	0xe122f30b			/* msr	LR_svc, SAVED_LR */
	.word	0xf57ff06f			/* isb */
	mrs	TMP1,      cpsr
	bic	TMP1,      TMP1, #0x1f
	orr	TMP1,      TMP1, #0x13
	msr	cpsr_fc,   TMP1			/* switch to secure-SVC mode */
	bx	ENTRY_ADDR

setup_secure_gic:
	/* Set the secure GIC view expected by secure-SVC FEL execution. */
	ldr	TMP1,      gicc_ctlr
	mov	TMP2,      #0xf
	str	TMP2,      [TMP1]
	mov	TMP2,      #0xf8
	str	TMP2,      [TMP1, #4]
	ldr	TMP1,      gicd_ctlr
	mov	TMP2,      #3
	str	TMP2,      [TMP1]
	bx	lr

gicc_ctlr:
	.word	SUNXI_GICC_BASE
gicd_ctlr:
	.word	SUNXI_GICD_BASE

appended_data:
/*
 * The appended data uses the following format:
 *
 *     struct {
 *         uint32_t          entry_addr;
 *         sram_swap_buffers swaptbl[];
 *     };
 */
