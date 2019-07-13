/**
 * @file int/pic.c
 * @brief PIC-related methods.
 *
 * Inspired by CMU's 2016 OS course
 *
 * @author Alex O'Neill <me@aoneill.me>
 *
 * @bugs No known bugs.
 */

#include <int/pic.h>
#include <sys/coms.h>

/** @brief Bring up the PICs on this system.
 *
 * @param master_base is the offset into the IDT that the first PIC
 *        uses for its IRQ0.  (So master_base through master_base + 7
 *        will be used.)
 *
 * @param slave_base is the offset into the IDT that the slave PIC
 *        uses for its IRQ0.
 *
 * @post This function will enable IRQs on the PICs; EFLAGS:IF still
 *       can be used to disable interrupts.  It is therefore suggested
 *       that this be called before IF is enabled the first time.
 *
 * @note This function assumes that there are only two PICs and that
 *       they are layed out as is conventional in IO space and that
 *       they are ganged together through master's IRQ2.  The last
 *       system that violated these assumptions went offline roughly
 *       forever ago.  If you don't like these, use the APICs which
 *       are better for you anyway.
 */
static void pic_init(unsigned char master_base, unsigned char slave_base) {
  // Bring the master IDT up
  outb(MASTER_ICW, PICM_ICW1);
  outb(MASTER_OCW, master_base);
  outb(MASTER_OCW, PICM_ICW3);
  outb(MASTER_OCW, PICM_ICW4);

  // Bring the slave IDT up
  outb(SLAVE_ICW, PICS_ICW1);
  outb(SLAVE_OCW, slave_base);
  outb(SLAVE_OCW, PICS_ICW3);
  outb(SLAVE_OCW, PICS_ICW4);

  // Tell the master and slave that any IRQs they had outstanding
  //   have been acknowledged.
  outb(MASTER_ICW, NON_SPEC_EOI);
  outb(SLAVE_ICW, NON_SPEC_EOI);

  // Enable all IRQs on master and slave
  outb(SLAVE_OCW, 0);
  outb(MASTER_OCW, 0);
}

/**
 * @brief Initialize and setup the PIC
 */
void pic_setup(void) {
  // IRQ 0..15, please occupy IDT slots 32...47
  pic_init(X86_PIC_MASTER_IRQ_BASE, X86_PIC_SLAVE_IRQ_BASE);
}
