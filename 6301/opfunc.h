/* Automagically generated Wed Oct  9 07:41:08 GMT+0100 1996 - dont edit */
#ifndef M68XX_OPFUNC_H
#define M68XX_OPFUNC_H

#if defined(__STDC__) || defined(__cplusplus)
# define P_(s) s
#else
# define P_(s) ()
#endif


/* ../../src/arch/m68xx/opfunc.c */
extern int getaddr_dir P_((void));
extern int getaddr_ext P_((void));
extern int getaddr_ix P_((void));
extern u_char getbyte_imm P_((void));
extern u_char getbyte_dir P_((void));
extern u_char getbyte_ext P_((void));
extern u_char getbyte_ix P_((void));
extern u_short getword_imm P_((void));
extern u_short getword_dir P_((void));
extern u_short getword_ext P_((void));
extern u_short getword_ix P_((void));
extern int branch_expr P_((int expr));
extern int pushbyte P_((u_char value));
extern int pushword P_((u_int value));
extern int popbyte P_((void));
extern int popword P_((void));
extern int asr_addr P_((u_int addr));
extern int clr_addr P_((int addr));
extern int com_addr P_((int addr));
extern int dec_addr P_((int addr));
extern int jsr_addr P_((int addr));
extern int inc_addr P_((int addr));
extern int lsl_addr P_((int addr));
extern int lsr_addr P_((int addr));
extern int neg_addr P_((int addr));
extern int rol_addr P_((int addr));
extern int ror_addr P_((int addr));
extern int staa_addr P_((int addr));
extern int stab_addr P_((int addr));
extern int sts_addr P_((int addr));
extern int stx_addr P_((int addr));
extern int tst_addr P_((int addr));
extern int int_addr P_((u_int addr));
extern int aba_inh P_((void));
extern int adca_imm P_((void));
extern int adca_dir P_((void));
extern int adca_ext P_((void));
extern int adca_ind_x P_((void));
extern int adcb_imm P_((void));
extern int adcb_dir P_((void));
extern int adcb_ext P_((void));
extern int adcb_ind_x P_((void));
extern int adda_imm P_((void));
extern int adda_dir P_((void));
extern int adda_ext P_((void));
extern int adda_ind_x P_((void));
extern int addb_imm P_((void));
extern int addb_dir P_((void));
extern int addb_ext P_((void));
extern int addb_ind_x P_((void));
extern int anda_imm P_((void));
extern int anda_dir P_((void));
extern int anda_ext P_((void));
extern int anda_ind_x P_((void));
extern int andb_imm P_((void));
extern int andb_dir P_((void));
extern int andb_ext P_((void));
extern int andb_ind_x P_((void));
extern int asr_ext P_((void));
extern int asr_ind_x P_((void));
extern int asra_inh P_((void));
extern int asrb_inh P_((void));
extern int bcc_rel P_((void));
extern int bcs_rel P_((void));
extern int beq_rel P_((void));
extern int bge_rel P_((void));
extern int bgt_rel P_((void));
extern int bhi_rel P_((void));
extern int bita_imm P_((void));
extern int bita_dir P_((void));
extern int bita_ext P_((void));
extern int bita_ind_x P_((void));
extern int bitb_imm P_((void));
extern int bitb_dir P_((void));
extern int bitb_ext P_((void));
extern int bitb_ind_x P_((void));
extern int ble_rel P_((void));
extern int bls_rel P_((void));
extern int blt_rel P_((void));
extern int bmi_rel P_((void));
extern int bne_rel P_((void));
extern int bpl_rel P_((void));
extern int bra_rel P_((void));
extern int bsr_rel P_((void));
extern int bvc_rel P_((void));
extern int bvs_rel P_((void));
extern int cba_inh P_((void));
extern int clc_inh P_((void));
extern int cli_inh P_((void));
extern int clr_ext P_((void));
extern int clr_ind_x P_((void));
extern int clra_inh P_((void));
extern int clrb_inh P_((void));
extern int clv_inh P_((void));
extern int cmpa_imm P_((void));
extern int cmpa_dir P_((void));
extern int cmpa_ext P_((void));
extern int cmpa_ind_x P_((void));
extern int cmpb_imm P_((void));
extern int cmpb_dir P_((void));
extern int cmpb_ext P_((void));
extern int cmpb_ind_x P_((void));
extern int com_ext P_((void));
extern int com_ind_x P_((void));
extern int coma_inh P_((void));
extern int comb_inh P_((void));
extern int cpx_imm P_((void));
extern int cpx_dir P_((void));
extern int cpx_ext P_((void));
extern int cpx_ind_x P_((void));
extern int daa_inh P_((void));
extern int dec_ext P_((void));
extern int dec_ind_x P_((void));
extern int deca_inh P_((void));
extern int decb_inh P_((void));
extern int des_inh P_((void));
extern int dex_inh P_((void));
extern int eora_imm P_((void));
extern int eora_dir P_((void));
extern int eora_ext P_((void));
extern int eora_ind_x P_((void));
extern int eorb_imm P_((void));
extern int eorb_dir P_((void));
extern int eorb_ext P_((void));
extern int eorb_ind_x P_((void));
extern int inc_ext P_((void));
extern int inc_ind_x P_((void));
extern int inca_inh P_((void));
extern int incb_inh P_((void));
extern int ins_inh P_((void));
extern int inx_inh P_((void));
extern int jmp_ext P_((void));
extern int jmp_ind_x P_((void));
extern int jsr_ext P_((void));
extern int jsr_ind_x P_((void));
extern int ldaa_imm P_((void));
extern int ldaa_dir P_((void));
extern int ldaa_ext P_((void));
extern int ldaa_ind_x P_((void));
extern int ldab_imm P_((void));
extern int ldab_dir P_((void));
extern int ldab_ext P_((void));
extern int ldab_ind_x P_((void));
extern int lds_imm P_((void));
extern int lds_dir P_((void));
extern int lds_ext P_((void));
extern int lds_ind_x P_((void));
extern int ldx_imm P_((void));
extern int ldx_dir P_((void));
extern int ldx_ext P_((void));
extern int ldx_ind_x P_((void));
extern int lsl_ext P_((void));
extern int lsl_ind_x P_((void));
extern int lsla_inh P_((void));
extern int lslb_inh P_((void));
extern int lsr_ext P_((void));
extern int lsr_ind_x P_((void));
extern int lsra_inh P_((void));
extern int lsrb_inh P_((void));
extern int neg_ext P_((void));
extern int neg_ind_x P_((void));
extern int nega_inh P_((void));
extern int negb_inh P_((void));
extern int nop_inh P_((void));
extern int oraa_imm P_((void));
extern int oraa_dir P_((void));
extern int oraa_ext P_((void));
extern int oraa_ind_x P_((void));
extern int orab_imm P_((void));
extern int orab_dir P_((void));
extern int orab_ext P_((void));
extern int orab_ind_x P_((void));
extern int psha_inh P_((void));
extern int pshb_inh P_((void));
extern int pula_inh P_((void));
extern int pulb_inh P_((void));
extern int rol_ext P_((void));
extern int rol_ind_x P_((void));
extern int rola_inh P_((void));
extern int rolb_inh P_((void));
extern int ror_ext P_((void));
extern int ror_ind_x P_((void));
extern int rora_inh P_((void));
extern int rorb_inh P_((void));
extern int rti_inh P_((void));
extern int rts_inh P_((void));
extern int sba_inh P_((void));
extern int sbca_imm P_((void));
extern int sbca_dir P_((void));
extern int sbca_ext P_((void));
extern int sbca_ind_x P_((void));
extern int sbcb_imm P_((void));
extern int sbcb_dir P_((void));
extern int sbcb_ext P_((void));
extern int sbcb_ind_x P_((void));
extern int sec_inh P_((void));
extern int sei_inh P_((void));
extern int sev_inh P_((void));
extern int staa_dir P_((void));
extern int staa_ext P_((void));
extern int staa_ind_x P_((void));
extern int stab_dir P_((void));
extern int stab_ext P_((void));
extern int stab_ind_x P_((void));
extern int sts_dir P_((void));
extern int sts_ext P_((void));
extern int sts_ind_x P_((void));
extern int stx_dir P_((void));
extern int stx_ext P_((void));
extern int stx_ind_x P_((void));
extern int suba_imm P_((void));
extern int suba_dir P_((void));
extern int suba_ext P_((void));
extern int suba_ind_x P_((void));
extern int subb_imm P_((void));
extern int subb_dir P_((void));
extern int subb_ext P_((void));
extern int subb_ind_x P_((void));
extern int swi_inh P_((void));
extern int tab_inh P_((void));
extern int tap_inh P_((void));
extern int tba_inh P_((void));
extern int tpa_inh P_((void));
extern int trap P_((void));
extern int tst_ext P_((void));
extern int tst_ind_x P_((void));
extern int tsta_inh P_((void));
extern int tstb_inh P_((void));
extern int tsx_inh P_((void));
extern int txs_inh P_((void));
extern int wai_inh P_((void));
extern int abx_inh P_((void));
extern int addd_imm P_((void));
extern int addd_dir P_((void));
extern int addd_ext P_((void));
extern int addd_ind_x P_((void));
extern int asld_inh P_((void));
extern int brn_rel P_((void));
extern int jsr_dir P_((void));
extern int ldd_imm P_((void));
extern int ldd_dir P_((void));
extern int ldd_ext P_((void));
extern int ldd_ind_x P_((void));
extern int lsld_inh P_((void));
extern int lsrd_inh P_((void));
extern int mul_inh P_((void));
extern int pshx_inh P_((void));
extern int pulx_inh P_((void));
extern int std_dir P_((void));
extern int std_ext P_((void));
extern int std_ind_x P_((void));
extern int subd_imm P_((void));
extern int subd_dir P_((void));
extern int subd_ext P_((void));
extern int subd_ind_x P_((void));
extern int aim_dir P_((void));
extern int aim_ind_x P_((void));
extern int eim_dir P_((void));
extern int eim_ind_x P_((void));
extern int oim_dir P_((void));
extern int oim_ind_x P_((void));
extern int tim_dir P_((void));
extern int tim_ind_x P_((void));
extern int xgdx_inh P_((void));
extern int slp_inh P_((void));
extern int getaddr_iy P_((void));
extern int getbyte_iy P_((void));
extern int getword_iy P_((void));
extern int bset_addr P_((u_int addr));
extern int bclr_addr P_((u_int addr));
extern int int_6811 P_((u_int addr));
extern int aby_inh P_((void));
extern int adca_ind_y P_((void));
extern int adcb_ind_y P_((void));
extern int adda_ind_y P_((void));
extern int addb_ind_y P_((void));
extern int addd_ind_y P_((void));
extern int anda_ind_y P_((void));
extern int andb_ind_y P_((void));
extern int asl_ind_y P_((void));
extern int asr_ind_y P_((void));
extern int bclr_dir P_((void));
extern int bclr_ind_x P_((void));
extern int bclr_ind_y P_((void));
extern int bita_ind_y P_((void));
extern int bitb_ind_y P_((void));
extern int brclr_dir P_((void));
extern int brclr_ind_x P_((void));
extern int brclr_ind_y P_((void));
extern int brset_dir P_((void));
extern int brset_ind_x P_((void));
extern int brset_ind_y P_((void));
extern int bset_dir P_((void));
extern int bset_ind_x P_((void));
extern int bset_ind_y P_((void));
extern int clr_ind_y P_((void));
extern int cmpa_ind_y P_((void));
extern int cmpb_ind_y P_((void));
extern int com_ind_y P_((void));
extern int cpd_imm P_((void));
extern int cpd_dir P_((void));
extern int cpd_ext P_((void));
extern int cpd_ind_x P_((void));
extern int cpd_ind_y P_((void));
extern int cpx_ind_y P_((void));
extern int cpy_imm P_((void));
extern int cpy_dir P_((void));
extern int cpy_ext P_((void));
extern int cpy_ind_x P_((void));
extern int cpy_ind_y P_((void));
extern int dec_ind_y P_((void));
extern int dey_inh P_((void));
extern int eora_ind_y P_((void));
extern int eorb_ind_y P_((void));
extern int idiv_inh P_((void));
extern int fdiv_inh P_((void));
extern int inc_ind_y P_((void));
extern int iny_inh P_((void));
extern int jmp_ind_y P_((void));
extern int jsr_ind_y P_((void));
extern int ldaa_ind_y P_((void));
extern int ldab_ind_y P_((void));
extern int ldd_ind_y P_((void));
extern int ldx_ind_y P_((void));
extern int lds_ind_y P_((void));
extern int ldy_imm P_((void));
extern int ldy_dir P_((void));
extern int ldy_ext P_((void));
extern int ldy_ind_x P_((void));
extern int ldy_ind_y P_((void));
extern int lsl_ind_y P_((void));
extern int lsr_ind_y P_((void));
extern int neg_ind_y P_((void));
extern int oraa_ind_y P_((void));
extern int orab_ind_y P_((void));
extern int pshy_inh P_((void));
extern int puly_inh P_((void));
extern int rol_ind_y P_((void));
extern int ror_ind_y P_((void));
extern int rti_6811 P_((void));
extern int sbca_ind_y P_((void));
extern int sbcb_ind_y P_((void));
extern int staa_ind_y P_((void));
extern int stab_ind_y P_((void));
extern int std_ind_y P_((void));
extern int sts_ind_y P_((void));
extern int stx_ind_y P_((void));
extern int sty_dir P_((void));
extern int sty_ext P_((void));
extern int sty_ind_x P_((void));
extern int sty_ind_y P_((void));
extern int stop_inh P_((void));
extern int suba_ind_y P_((void));
extern int subb_ind_y P_((void));
extern int subd_ind_y P_((void));
extern int swi_6811 P_((void));
extern int test_inh P_((void));
extern int trap_6811 P_((void));
extern int tst_ind_y P_((void));
extern int tsy_inh P_((void));
extern int tys_inh P_((void));
extern int xgdy_inh P_((void));
extern int getaddr_ix0 P_((void));
extern int getaddr_ix1 P_((void));
extern int getaddr_ix2 P_((void));
extern int getbyte_ix0 P_((void));
extern int getbyte_ix1 P_((void));
extern int getbyte_ix2 P_((void));
extern int bset_bit P_((int addr, int m));
extern int bclr_bit P_((int addr, int m));
extern int getirq P_((void));
extern int adc_ix0 P_((void));
extern int adc_ix1 P_((void));
extern int adc_ix2 P_((void));
extern int add_ix0 P_((void));
extern int add_ix1 P_((void));
extern int add_ix2 P_((void));
extern int and_ix0 P_((void));
extern int and_ix1 P_((void));
extern int and_ix2 P_((void));
extern int asl_ix0 P_((void));
extern int asl_ix1 P_((void));
extern int asl_ix2 P_((void));
extern int asl_dir P_((void));
extern int aslx_inh P_((void));
extern int asr_ix0 P_((void));
extern int asr_ix1 P_((void));
extern int asr_ix2 P_((void));
extern int asr_dir P_((void));
extern int asrx_inh P_((void));
extern int brset0_btb P_((void));
extern int brset1_btb P_((void));
extern int brset2_btb P_((void));
extern int brset3_btb P_((void));
extern int brset4_btb P_((void));
extern int brset5_btb P_((void));
extern int brset6_btb P_((void));
extern int brset7_btb P_((void));
extern int brclr0_btb P_((void));
extern int brclr1_btb P_((void));
extern int brclr2_btb P_((void));
extern int brclr3_btb P_((void));
extern int brclr4_btb P_((void));
extern int brclr5_btb P_((void));
extern int brclr6_btb P_((void));
extern int brclr7_btb P_((void));
extern int bset0_bsc P_((void));
extern int bset1_bsc P_((void));
extern int bset2_bsc P_((void));
extern int bset3_bsc P_((void));
extern int bset4_bsc P_((void));
extern int bset5_bsc P_((void));
extern int bset6_bsc P_((void));
extern int bset7_bsc P_((void));
extern int bclr0_bsc P_((void));
extern int bclr1_bsc P_((void));
extern int bclr2_bsc P_((void));
extern int bclr3_bsc P_((void));
extern int bclr4_bsc P_((void));
extern int bclr5_bsc P_((void));
extern int bclr6_bsc P_((void));
extern int bclr7_bsc P_((void));
extern int bhcc_rel P_((void));
extern int bhcs_rel P_((void));
extern int bmc_rel P_((void));
extern int bms_rel P_((void));
extern int bih_rel P_((void));
extern int bil_rel P_((void));
extern int mul_inh_6805 P_((void));
extern int neg_ix0 P_((void));
extern int neg_ix1 P_((void));
extern int neg_ix2 P_((void));
extern int neg_dir P_((void));
extern int negx_inh P_((void));
extern int com_ix0 P_((void));
extern int com_ix1 P_((void));
extern int com_ix2 P_((void));
extern int com_dir P_((void));
extern int comx_inh P_((void));
extern int lsr_ix0 P_((void));
extern int lsr_ix1 P_((void));
extern int lsr_ix2 P_((void));
extern int lsr_dir P_((void));
extern int lsrx_inh P_((void));
extern int ror_ix0 P_((void));
extern int ror_ix1 P_((void));
extern int ror_ix2 P_((void));
extern int ror_dir P_((void));
extern int rorx_inh P_((void));
extern int lsl_ix0 P_((void));
extern int lsl_ix1 P_((void));
extern int lsl_ix2 P_((void));
extern int lsl_dir P_((void));
extern int lslx_inh P_((void));
extern int rol_ix0 P_((void));
extern int rol_ix1 P_((void));
extern int rol_ix2 P_((void));
extern int rol_dir P_((void));
extern int rolx_inh P_((void));
extern int dec_ix0 P_((void));
extern int dec_ix1 P_((void));
extern int dec_ix2 P_((void));
extern int dec_dir P_((void));
extern int decx_inh P_((void));
extern int inc_ix0 P_((void));
extern int inc_ix1 P_((void));
extern int inc_ix2 P_((void));
extern int inc_dir P_((void));
extern int incx_inh P_((void));
extern int tst_ix0 P_((void));
extern int tst_ix1 P_((void));
extern int tst_ix2 P_((void));
extern int tst_dir P_((void));
extern int tstx_inh P_((void));
extern int clr_ix0 P_((void));
extern int clr_ix1 P_((void));
extern int clr_ix2 P_((void));
extern int clr_dir P_((void));
extern int clrx_inh P_((void));
extern int stop_inh_6805 P_((void));
extern int rsp_inh P_((void));
extern int tax_inh P_((void));
extern int txa_inh P_((void));
extern int wait_inh P_((void));
extern int sub_ix0 P_((void));
extern int sub_ix1 P_((void));
extern int sub_ix2 P_((void));
extern int cmp_ix0 P_((void));
extern int cmp_ix1 P_((void));
extern int cmp_ix2 P_((void));
extern int sbc_ix0 P_((void));
extern int sbc_ix1 P_((void));
extern int sbc_ix2 P_((void));
extern int cpx_ix0 P_((void));
extern int cpx_ix1 P_((void));
extern int cpx_ix2 P_((void));
extern int cpx_imm_6805 P_((void));
extern int cpx_dir_6805 P_((void));
extern int cpx_ext_6805 P_((void));
extern int bit_ix0 P_((void));
extern int bit_ix1 P_((void));
extern int bit_ix2 P_((void));
extern int lda_ix0 P_((void));
extern int lda_ix1 P_((void));
extern int lda_ix2 P_((void));
extern int sta_ix0 P_((void));
extern int sta_ix1 P_((void));
extern int sta_ix2 P_((void));
extern int eor_ix0 P_((void));
extern int eor_ix1 P_((void));
extern int eor_ix2 P_((void));
extern int ora_ix0 P_((void));
extern int ora_ix1 P_((void));
extern int ora_ix2 P_((void));
extern int jmp_dir P_((void));
extern int jmp_ix0 P_((void));
extern int jmp_ix1 P_((void));
extern int jmp_ix2 P_((void));
extern int jsr_ix0 P_((void));
extern int jsr_ix1 P_((void));
extern int jsr_ix2 P_((void));
extern int ldx_ix0 P_((void));
extern int ldx_ix1 P_((void));
extern int ldx_ix2 P_((void));
extern int ldx_imm_6805 P_((void));
extern int ldx_dir_6805 P_((void));
extern int ldx_ext_6805 P_((void));
extern int stx_ix0 P_((void));
extern int stx_ix1 P_((void));
extern int stx_ix2 P_((void));
extern int stx_dir_6805 P_((void));
extern int stx_ext_6805 P_((void));
extern int int_6805 P_((u_int addr));
extern int rti_6805 P_((void));
extern int swi_6805 P_((void));
extern int trap_6805 P_((void));

#undef P_
#endif /* M68XX_OPFUNC_H */
