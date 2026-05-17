	/* <entry_point>: */
	0xea000015, /*        0:    b          5c <setup_stack>             */
	/* <stack_begin>: */
	0xffffffff, /*        4:    .word      0xffffffff                   */
	0xffffffff, /*        8:    .word      0xffffffff                   */
	0xffffffff, /*        c:    .word      0xffffffff                   */
	0xffffffff, /*       10:    .word      0xffffffff                   */
	0xffffffff, /*       14:    .word      0xffffffff                   */
	0xffffffff, /*       18:    .word      0xffffffff                   */
	0xffffffff, /*       1c:    .word      0xffffffff                   */
	0xffffffff, /*       20:    .word      0xffffffff                   */
	/* <stack_end>: */
	0xe1a00000, /*       24:    nop                                     */
	/* <swap_all_buffers>: */
	0xe28f4c01, /*       28:    add        r4, pc, #256                 */
	/* <swap_next_buffer>: */
	0xe4940004, /*       2c:    ldr        r0, [r4], #4                 */
	0xe4941004, /*       30:    ldr        r1, [r4], #4                 */
	0xe4946004, /*       34:    ldr        r6, [r4], #4                 */
	0xe3560000, /*       38:    cmp        r6, #0                       */
	0x012fff1e, /*       3c:    bxeq       lr                           */
	/* <swap_next_word>: */
	0xe5902000, /*       40:    ldr        r2, [r0]                     */
	0xe5913000, /*       44:    ldr        r3, [r1]                     */
	0xe2566004, /*       48:    subs       r6, r6, #4                   */
	0xe4812004, /*       4c:    str        r2, [r1], #4                 */
	0xe4803004, /*       50:    str        r3, [r0], #4                 */
	0x1afffff9, /*       54:    bne        40 <swap_next_word>          */
	0xeafffff3, /*       58:    b          2c <swap_next_buffer>        */
	/* <setup_stack>: */
	0xe59f80c8, /*       5c:    ldr        r8, [pc, #200]               */
	0xe24f0044, /*       60:    sub        r0, pc, #68                  */
	0xe520d004, /*       64:    str        sp, [r0, #-4]!               */
	0xe1a0d000, /*       68:    mov        sp, r0                       */
	0xe10f2000, /*       6c:    mrs        r2, CPSR                     */
	0xe92d4004, /*       70:    push       {r2, lr}                     */
	0xe38220c0, /*       74:    orr        r2, r2, #192                 */
	0xe121f002, /*       78:    msr        CPSR_c, r2                   */
	0xee112f10, /*       7c:    mrc        15, 0, r2, cr1, cr0, {0}     */
	0xe3120004, /*       80:    tst        r2, #4                       */
	0x03120a01, /*       84:    tsteq      r2, #4096                    */
	0x1a000004, /*       88:    bne        a0 <return_to_fel_noswap>    */
	0xebffffe5, /*       8c:    bl         28 <swap_all_buffers>        */
	0xeb000006, /*       90:    bl         b0 <call_entry>              */
	0xeaffffff, /*       94:    b          98 <return_to_secure_fel>    */
	/* <return_to_secure_fel>: */
	0xebffffe2, /*       98:    bl         28 <swap_all_buffers>        */
	0xeb000017, /*       9c:    bl         100 <setup_secure_gic>       */
	/* <return_to_fel_noswap>: */
	0xe8bd4004, /*       a0:    pop        {r2, lr}                     */
	0xe121f002, /*       a4:    msr        CPSR_c, r2                   */
	0xe59dd000, /*       a8:    ldr        sp, [sp]                     */
	0xe12fff1e, /*       ac:    bx         lr                           */
	/* <call_entry>: */
	0xee072f9a, /*       b0:    mcr        15, 0, r2, cr7, cr10, {4}    */
	0xee102f10, /*       b4:    mrc        15, 0, r2, cr0, cr0, {0}     */
	0xe202280f, /*       b8:    and        r2, r2, #983040              */
	0xe3520806, /*       bc:    cmp        r2, #393216                  */
	0xce072f95, /*       c0:    mcrgt      15, 0, r2, cr7, cr5, {4}     */
	0xe1a0a00d, /*       c4:    mov        sl, sp                       */
	0xe1a0b00e, /*       c8:    mov        fp, lr                       */
	0xe1600070, /*       cc:    .word      0xe1600070                   */
	0xe3a02000, /*       d0:    mov        r2, #0                       */
	0xee012f11, /*       d4:    mcr        15, 0, r2, cr1, cr1, {0}     */
	0xee0c2f30, /*       d8:    mcr        15, 0, r2, cr12, cr0, {1}    */
	0xeb000007, /*       dc:    bl         100 <setup_secure_gic>       */
	0xe123f30a, /*       e0:    .word      0xe123f30a                   */
	0xe122f30b, /*       e4:    .word      0xe122f30b                   */
	0xf57ff06f, /*       e8:    .word      0xf57ff06f                   */
	0xe10f2000, /*       ec:    mrs        r2, CPSR                     */
	0xe3c2201f, /*       f0:    bic        r2, r2, #31                  */
	0xe3822013, /*       f4:    orr        r2, r2, #19                  */
	0xe129f002, /*       f8:    msr        CPSR_fc, r2                  */
	0xe12fff18, /*       fc:    bx         r8                           */
	/* <setup_secure_gic>: */
	0xe59f201c, /*      100:    ldr        r2, [pc, #28]                */
	0xe3a0300f, /*      104:    mov        r3, #15                      */
	0xe5823000, /*      108:    str        r3, [r2]                     */
	0xe3a030f8, /*      10c:    mov        r3, #248                     */
	0xe5823004, /*      110:    str        r3, [r2, #4]                 */
	0xe59f200c, /*      114:    ldr        r2, [pc, #12]                */
	0xe3a03003, /*      118:    mov        r3, #3                       */
	0xe5823000, /*      11c:    str        r3, [r2]                     */
	0xe12fff1e, /*      120:    bx         lr                           */
	/* <gicc_ctlr>: */
	0x03022000, /*      124:    .word      0x03022000                   */
	/* <gicd_ctlr>: */
	0x03021000, /*      128:    .word      0x03021000                   */
