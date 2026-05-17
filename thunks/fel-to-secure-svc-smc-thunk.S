/* secure-FEL thunk returning from SMC in monitor mode. */

.arm

BUF1		.req	r0
BUF2		.req	r1
TMP1		.req	r2
TMP2		.req	r3
SWAPTBL		.req	r4
ORIG_VECTOR	.req	r5
BUFSIZE		.req	r6
ENTRY_ADDR	.req	r8
MON_VECTOR	.req	r9
SAVED_SP	.req	r10
SAVED_LR	.req	r11

entry_point:
	b	setup_stack

stack_begin:
	.space	32, 0xff
stack_end:
	nop

	/* Walk the table and swap all buffers. */
swap_all_buffers:
	adr	SWAPTBL,   appended_data + 16
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

setup_stack:
	ldr	ENTRY_ADDR, appended_data
	ldr	MON_VECTOR, appended_data + 4
	adr	BUF1,      stack_end
	str	sp,        [BUF1, #-4]!
	mov	sp,        BUF1
	mrs	TMP1,      cpsr
	push	{TMP1, lr}

	/* Disable IRQ and FIQ. */
	orr	TMP1,      #0xc0
	msr	cpsr_c,    TMP1

	/* Check if the instructions or data cache is enabled. */
	mrc	p15, 0, TMP1, c1, c0, 0
	tst	TMP1,      #(1 << 2)
	tsteq	TMP1,      #(1 << 12)
	bne	return_to_fel_noswap

	bl	swap_all_buffers
	bl	call_entry
	b	return_to_secure_fel

return_to_secure_fel:
	bl	swap_all_buffers

return_to_fel_noswap:
	pop	{TMP1, lr}
	msr	cpsr_c,    TMP1
	ldr     sp,        [sp]
	bx	lr

call_entry:
	/* Make the monitor SMC vector return without restoring SPSR_mon. */
	ldr	ORIG_VECTOR, [MON_VECTOR, #8]
	ldr	TMP1,      monitor_return
	str	TMP1,      [MON_VECTOR, #8]

	/* Make the patched vector visible before entering monitor mode. */
	mcr	p15, 0, TMP1, c7, c10, 4	/* CP15DSB */
	mrc	p15, 0, TMP1, c0, c0, 0		/* read MIDR */
	and	TMP1, TMP1, #(0xf << 16)	/* architecture */
	cmp	TMP1, #(0x6 << 16)		/* ARMv5TEJ */
	mcrgt	p15, 0, TMP1, c7, c5, 4		/* CP15ISB, if > ARMv5TEJ */

	/* Save the non-secure SVC SP/LR for the secure bank below. */
	mov	SAVED_SP,  sp
	mov	SAVED_LR,  lr
	.word	0xe1600070			/* smc	#0 */

	/* The monitor SMC vector returns here without changing mode. */
	str	ORIG_VECTOR, [MON_VECTOR, #8]
	mov	TMP1,      #0
	mcr	p15, 0, TMP1, c1, c1, 0		/* SCR = secure */
	mcr	p15, 0, TMP1, c12, c0, 1	/* MVBAR = 0 */

	ldr	TMP1,      appended_data + 8
	mov	TMP2,      #0xf
	str	TMP2,      [TMP1]		/* GICC_CTLR */
	mov	TMP2,      #0xf8
	str	TMP2,      [TMP1, #4]		/* GICC_PMR */
	ldr	TMP1,      appended_data + 12
	mov	TMP2,      #3
	str	TMP2,      [TMP1]		/* GICD_CTLR */

	.word	0xe123f30a			/* msr	SP_svc, SAVED_SP */
	.word	0xe122f30b			/* msr	LR_svc, SAVED_LR */
	.word	0xf57ff06f			/* isb */
	mrs	TMP1,      cpsr
	bic	TMP1,      TMP1, #0x1f
	orr	TMP1,      TMP1, #0x13
	msr	cpsr_fc,   TMP1			/* switch to secure-SVC mode */
	bx	ENTRY_ADDR

monitor_return:
	.word	0xe1a0f00e			/* mov	pc, lr */

appended_data:
/*
 * The appended data uses the following format:
 *
 *     struct {
 *         uint32_t          entry_addr;
 *         uint32_t          monitor_vector_addr;
 *         uint32_t          gicc_ctlr_addr;
 *         uint32_t          gicd_ctlr_addr;
 *         sram_swap_buffers swaptbl[];
 *     };
 */
