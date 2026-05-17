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
	0xe28f4e11, /*       28:    add        r4, pc, #272                 */
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
	0xe59f80cc, /*       5c:    ldr        r8, [pc, #204]               */
	0xe59f90cc, /*       60:    ldr        r9, [pc, #204]               */
	0xe24f0048, /*       64:    sub        r0, pc, #72                  */
	0xe520d004, /*       68:    str        sp, [r0, #-4]!               */
	0xe1a0d000, /*       6c:    mov        sp, r0                       */
	0xe10f2000, /*       70:    mrs        r2, CPSR                     */
	0xe92d4004, /*       74:    push       {r2, lr}                     */
	0xe38220c0, /*       78:    orr        r2, r2, #192                 */
	0xe121f002, /*       7c:    msr        CPSR_c, r2                   */
	0xee112f10, /*       80:    mrc        15, 0, r2, cr1, cr0, {0}     */
	0xe3120004, /*       84:    tst        r2, #4                       */
	0x03120a01, /*       88:    tsteq      r2, #4096                    */
	0x1a000003, /*       8c:    bne        a0 <return_to_fel_noswap>    */
	0xebffffe4, /*       90:    bl         28 <swap_all_buffers>        */
	0xeb000005, /*       94:    bl         b0 <call_entry>              */
	0xeaffffff, /*       98:    b          9c <return_to_secure_fel>    */
	/* <return_to_secure_fel>: */
	0xebffffe1, /*       9c:    bl         28 <swap_all_buffers>        */
	/* <return_to_fel_noswap>: */
	0xe8bd4004, /*       a0:    pop        {r2, lr}                     */
	0xe121f002, /*       a4:    msr        CPSR_c, r2                   */
	0xe59dd000, /*       a8:    ldr        sp, [sp]                     */
	0xe12fff1e, /*       ac:    bx         lr                           */
	/* <call_entry>: */
	0xe5995008, /*       b0:    ldr        r5, [r9, #8]                 */
	0xe59f2070, /*       b4:    ldr        r2, [pc, #112]               */
	0xe5892008, /*       b8:    str        r2, [r9, #8]                 */
	0xee072f9a, /*       bc:    mcr        15, 0, r2, cr7, cr10, {4}    */
	0xee102f10, /*       c0:    mrc        15, 0, r2, cr0, cr0, {0}     */
	0xe202280f, /*       c4:    and        r2, r2, #983040              */
	0xe3520806, /*       c8:    cmp        r2, #393216                  */
	0xce072f95, /*       cc:    mcrgt      15, 0, r2, cr7, cr5, {4}     */
	0xe1a0a00d, /*       d0:    mov        sl, sp                       */
	0xe1a0b00e, /*       d4:    mov        fp, lr                       */
	0xe1600070, /*       d8:    .word      0xe1600070                   */
	0xe5895008, /*       dc:    str        r5, [r9, #8]                 */
	0xe3a02000, /*       e0:    mov        r2, #0                       */
	0xee012f11, /*       e4:    mcr        15, 0, r2, cr1, cr1, {0}     */
	0xee0c2f30, /*       e8:    mcr        15, 0, r2, cr12, cr0, {1}    */
	0xe59f2044, /*       ec:    ldr        r2, [pc, #68]                */
	0xe3a0300f, /*       f0:    mov        r3, #15                      */
	0xe5823000, /*       f4:    str        r3, [r2]                     */
	0xe3a030f8, /*       f8:    mov        r3, #248                     */
	0xe5823004, /*       fc:    str        r3, [r2, #4]                 */
	0xe59f2034, /*      100:    ldr        r2, [pc, #52]                */
	0xe3a03003, /*      104:    mov        r3, #3                       */
	0xe5823000, /*      108:    str        r3, [r2]                     */
	0xe123f30a, /*      10c:    .word      0xe123f30a                   */
	0xe122f30b, /*      110:    .word      0xe122f30b                   */
	0xf57ff06f, /*      114:    .word      0xf57ff06f                   */
	0xe10f2000, /*      118:    mrs        r2, CPSR                     */
	0xe3c2201f, /*      11c:    bic        r2, r2, #31                  */
	0xe3822013, /*      120:    orr        r2, r2, #19                  */
	0xe129f002, /*      124:    msr        CPSR_fc, r2                  */
	0xe12fff18, /*      128:    bx         r8                           */
	/* <monitor_return>: */
	0xe1a0f00e, /*      12c:    .word      0xe1a0f00e                   */
