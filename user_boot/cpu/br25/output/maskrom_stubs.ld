  local_irq_enable = ABSOLUTE(0x117d40);
  local_irq_disable = ABSOLUTE(0x117d44);
  p33_buf = ABSOLUTE(0x117d48);
  p33_xor_1byte = ABSOLUTE(0x117d4c);
  p33_and_1byte = ABSOLUTE(0x117d50);
  p33_or_1byte = ABSOLUTE(0x117d54);
  p33_tx_1byte = ABSOLUTE(0x117d58);
  p33_rx_1byte = ABSOLUTE(0x117d5c);
  P33_CON_SET = ABSOLUTE(0x117d60);
  os_init = ABSOLUTE(0x117d64);
  os_start = ABSOLUTE(0x117d68);
  os_get_curr_tcb_var = ABSOLUTE(0x117d6c);
  os_task_create = ABSOLUTE(0x117d70);
  os_current_task = ABSOLUTE(0x117d74);
  os_time_dly = ABSOLUTE(0x117d78);
  os_time_get = ABSOLUTE(0x117d7c);
  os_task_del_req = ABSOLUTE(0x117d80);
  os_task_del_res = ABSOLUTE(0x117d84);
  os_task_del = ABSOLUTE(0x117d88);
  __os_taskq_pend = ABSOLUTE(0x117d8c);
  os_taskq_pend = ABSOLUTE(0x117d90);
  os_task_pend = ABSOLUTE(0x117d94);
  __os_taskq_post = ABSOLUTE(0x117d98);
  os_taskq_post = ABSOLUTE(0x117d9c);
  os_taskq_post_type = ABSOLUTE(0x117da0);
  os_taskq_flush = ABSOLUTE(0x117da4);
  task_queue_post_event = ABSOLUTE(0x117da8);
  os_taskq_post_msg = ABSOLUTE(0x117dac);
  os_taskq_post_event = ABSOLUTE(0x117db0);
  os_taskq_accept = ABSOLUTE(0x117db4);
  os_taskq_del = ABSOLUTE(0x117db8);
  os_taskq_del_type = ABSOLUTE(0x117dbc);
  os_sem_create = ABSOLUTE(0x117dc0);
  os_sem_del = ABSOLUTE(0x117dc4);
  os_sem_pend = ABSOLUTE(0x117dc8);
  os_sem_post = ABSOLUTE(0x117dcc);
  os_sem_set = ABSOLUTE(0x117dd0);
  os_sem_valid = ABSOLUTE(0x117dd4);
  os_sem_query = ABSOLUTE(0x117dd8);
  os_mutex_create = ABSOLUTE(0x117ddc);
  os_mutex_del = ABSOLUTE(0x117de0);
  os_mutex_pend = ABSOLUTE(0x117de4);
  os_mutex_post = ABSOLUTE(0x117de8);
  os_mutex_valid = ABSOLUTE(0x117dec);
  xTaskCreate = ABSOLUTE(0x117df0);
  xTaskGetCurrentTaskHandle = ABSOLUTE(0x117df4);
  xTaskGetTickCount = ABSOLUTE(0x117df8);
  xTaskGetHandle = ABSOLUTE(0x117dfc);
  vTaskDelete = ABSOLUTE(0x117e00);
  xTaskGetApplicationTaskTag = ABSOLUTE(0x117e04);
  vTaskSetApplicationTaskTag = ABSOLUTE(0x117e08);
  xQueueGenericCreateStatic = ABSOLUTE(0x117e0c);
  xQueueGenericReset = ABSOLUTE(0x117e10);
  xQueueGenericSend = ABSOLUTE(0x117e14);
  xQueueGenericSendFromISR = ABSOLUTE(0x117e18);
  xQueueGenericReceive = ABSOLUTE(0x117e1c);
  xQueueReceiveFromISR = ABSOLUTE(0x117e20);
  vQueueDelete = ABSOLUTE(0x117e24);
  uxQueueMessagesWaiting = ABSOLUTE(0x117e28);
  uxQueueMessagesWaitingFromISR = ABSOLUTE(0x117e2c);
  uxQueueSpacesAvailable = ABSOLUTE(0x117e30);
  pcTaskGetName = ABSOLUTE(0x117e34);
  prvGetExpectedIdleTime = ABSOLUTE(0x117e38);
  eTaskConfirmSleepModeStatus = ABSOLUTE(0x117e3c);
  vTaskStepTick = ABSOLUTE(0x117e40);
  pxPortInitialiseStack = ABSOLUTE(0x117e44);
  xPortStartScheduler = ABSOLUTE(0x117e48);
  vPortEndScheduler = ABSOLUTE(0x117e4c);
  vPortYield = ABSOLUTE(0x117e50);
  vPortExitCritical = ABSOLUTE(0x117e54);
  OS_ClrPending = ABSOLUTE(0x117e58);
  vTickISR = ABSOLUTE(0x117e5c);
  pmalloc_init = ABSOLUTE(0x117e60);
  pmalloc = ABSOLUTE(0x117e64);
  pfree = ABSOLUTE(0x117e68);
  pmalloc_get_unused_pages = ABSOLUTE(0x117e6c);
  get_physic_address = ABSOLUTE(0x117e70);
  pmm_get_var = ABSOLUTE(0x117e74);
  vmm_init_ = ABSOLUTE(0x117e78);
  vmalloc_ = ABSOLUTE(0x117e7c);
  vfree_ = ABSOLUTE(0x117e80);
  vmem_get_phy_adr = ABSOLUTE(0x117e84);
  vmem_start = ABSOLUTE(0x117e88);
  vmem_end = ABSOLUTE(0x117e8c);
  vmm_get_var = ABSOLUTE(0x117e90);
  vmalloc_v2 = ABSOLUTE(0x117e94);
  vfree_v2 = ABSOLUTE(0x117e98);
  vcopy_ = ABSOLUTE(0x117e9c);
  get_cvsd_codec_ops = ABSOLUTE(0x117ea0);
  VecPlus_fix_r_r_r = ABSOLUTE(0x117ea4);
  VecMinus_fix_r_r_r = ABSOLUTE(0x117ea8);
  VecDiv_fix_r_r_r = ABSOLUTE(0x117eac);
  VecEleMul_fix_r_r_r = ABSOLUTE(0x117eb0);
  VecEleMul_fix_c_r_c = ABSOLUTE(0x117eb4);
  VecMagAndDiv_fix = ABSOLUTE(0x117eb8);
  VecCopy_fix_r_r = ABSOLUTE(0x117ebc);
  VecConjEleMulAndDiv_fix = ABSOLUTE(0x117ec0);
  VecRecurAvg_fix = ABSOLUTE(0x117ec4);
  VecConjRecurAvg_fix = ABSOLUTE(0x117ec8);
  VecMagRecurAvg_fix = ABSOLUTE(0x117ecc);
  VecPlusMulScalar_fix = ABSOLUTE(0x117ed0);
  VecGetMag_fix = ABSOLUTE(0x117ed4);
  VecMax_fix_v_s = ABSOLUTE(0x117ed8);
  VecMin_fix_v_s = ABSOLUTE(0x117edc);
  VecOppos_fix = ABSOLUTE(0x117ee0);
  VecMean_fix = ABSOLUTE(0x117ee4);
  VecMeanSq_fix_c = ABSOLUTE(0x117ee8);
  VecRequanByMax_fix_r = ABSOLUTE(0x117eec);
  VecRequanByMax_fix_c = ABSOLUTE(0x117ef0);
  VecEleShift_fix_c = ABSOLUTE(0x117ef4);
  VecOvShift_fix = ABSOLUTE(0x117ef8);
  VecCopy_fix_r_c = ABSOLUTE(0x117efc);
  VecMeanSq_fix_r = ABSOLUTE(0x117f00);
  VecEleShift_fix_r = ABSOLUTE(0x117f04);
  logAprx_fix = ABSOLUTE(0x117f08);
  expAprx_fix = ABSOLUTE(0x117f0c);
  sqrtAprx_fix = ABSOLUTE(0x117f10);
  VecEleMul_s16_s16_s32 = ABSOLUTE(0x117f14);
  VecEleMul_s16_s32_s16 = ABSOLUTE(0x117f18);
  VecEleMul_s32_s16_s32 = ABSOLUTE(0x117f1c);
  VecOvShift_s16_s16 = ABSOLUTE(0x117f20);
  VecOvShift_s16_s32 = ABSOLUTE(0x117f24);
  VecCopy_s16_s32 = ABSOLUTE(0x117f28);
  VecCopy_s32_s16 = ABSOLUTE(0x117f2c);
  VecMinus_s16_s32_s32 = ABSOLUTE(0x117f30);
  VecMeanSq_s16_s32 = ABSOLUTE(0x117f34);
  CircVecArrEwMulAndSum_fix = ABSOLUTE(0x117f38);
  B_fir_cal_s = ABSOLUTE(0x117f3c);
  B_array_vol = ABSOLUTE(0x117f40);
  B_Syn_filt = ABSOLUTE(0x117f44);
  B_Residu = ABSOLUTE(0x117f48);
  B_comput_correlataionS = ABSOLUTE(0x117f4c);
  B_iircal = ABSOLUTE(0x117f50);
  repair_corefun_corr = ABSOLUTE(0x117f54);
  repair_corefun_mixdata = ABSOLUTE(0x117f58);
  get_repair_api = ABSOLUTE(0x117f5c);
  firfft = ABSOLUTE(0x117f60);
  firifft = ABSOLUTE(0x117f64);
  _rfifft_wrap = ABSOLUTE(0x117f68);
  _rififft_wrap = ABSOLUTE(0x117f6c);
  memmem = ABSOLUTE(0x117f70);
  memcpy = ABSOLUTE(0x117f74);
  memmove = ABSOLUTE(0x117f78);
  memcmp = ABSOLUTE(0x117f7c);
  memset = ABSOLUTE(0x117f80);
  strcmp = ABSOLUTE(0x117f84);
  strcpy = ABSOLUTE(0x117f88);
  strlen = ABSOLUTE(0x117f8c);
  strncmp = ABSOLUTE(0x117f90);
  strstr = ABSOLUTE(0x117f94);
  strchr = ABSOLUTE(0x117f98);
  __fixdfsi = ABSOLUTE(0x117f9c);
  __fixsfsi = ABSOLUTE(0x117fa0);
  spi_get_port = ABSOLUTE(0x117fa4);
  spi_set_port = ABSOLUTE(0x117fa8);
  sfc_suspend = ABSOLUTE(0x117fac);
  sfc_resume = ABSOLUTE(0x117fb0);
  sfc_drop_cache = ABSOLUTE(0x117fb4);
  chip_crc16 = ABSOLUTE(0x117fb8);
  CrcDecode = ABSOLUTE(0x117fbc);
  mask_init = ABSOLUTE(0x117fc0);
  wdt_clr = ABSOLUTE(0x117fc4);
  nvram_set_boot_state = ABSOLUTE(0x117fc8);
  chip_reset = ABSOLUTE(0x117fcc);
  the_debug_isr = ABSOLUTE(0x117fd0);
  flash_poweron = ABSOLUTE(0x117fd4);
  exception_irq_handler = ABSOLUTE(0x117fd8);
  print = ABSOLUTE(0x117fdc);
  sprintf = ABSOLUTE(0x117fe0);
  printf = ABSOLUTE(0x117fe4);
  puts = ABSOLUTE(0x117fe8);
  doe = ABSOLUTE(0x117fec);
  LZ4_setStreamDecode = ABSOLUTE(0x117ff0);
  LZ4_decompress_safe_continue = ABSOLUTE(0x117ff4);
  LZ4_decompress_generic = ABSOLUTE(0x117ff8);
  LZ4_decompress_safe_usingDict = ABSOLUTE(0x117ffc);
  bsinc_tab_6 = ABSOLUTE(0x110f60);
  bsinc_tab_6_l = ABSOLUTE(0x1111a4);
  ans_win = ABSOLUTE(0x110c60);
  nlp_win = ABSOLUTE(0x110d60);
  iCos_Tab = ABSOLUTE(0x115ea8);
  WOLA_ANA_128 = ABSOLUTE(0x110060);
  WOLA_ANA_64 = ABSOLUTE(0x110460);
  WOLA_SYN_128 = ABSOLUTE(0x110660);
  WOLA_SYN_64 = ABSOLUTE(0x110a60);
  boot_arg_list = ABSOLUTE(0x31bfc);
  _pll_init = ABSOLUTE(0x116f38);
  _IRQ_MEM_ADDR = ABSOLUTE(0x31f00);
  _MASK_MEM_BEGIN = ABSOLUTE(0x31be0);
  _MASK_MEM_SIZE = ABSOLUTE(0x318);


  HWI_Install = ABSOLUTE(0x115fb0);
  rom_irq_init = ABSOLUTE(0x112a40);
  nvram_list = ABSOLUTE(0x10880);

