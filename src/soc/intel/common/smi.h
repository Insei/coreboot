/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2016 Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _INTEL_COMMON_SMI_H_
#define _INTEL_COMMON_SMI_H_

/*
 * The register value is used with get_reg and set_reg
 */
enum smm_reg {
	RAX,
	RBX,
	RCX,
	RDX,
};


struct	smm_save_state_ops {
	/* return io_misc_info from SMM Save State Area */
	uint32_t (*get_io_misc_info)(void *state);

	/* return value of the requested register from
	 * SMM Save State Area
	 */
	uint64_t (*get_reg)(void *state, enum smm_reg reg);

	void (*set_reg)(void *state, enum smm_reg reg, uint64_t val);
};

typedef void (*smi_handler_t)(const struct smm_save_state_ops *save_state_ops);

/*
 * SOC SMI Handler has to provide this structure which has methods to access
 * the SOC specific SMM Save State Area
 */
const struct smm_save_state_ops *get_smm_save_state_ops(void);

/*
 * southbridge_smi should be defined inside SOC specific code and should have
 * handlers for any SMI events that need to be handled. Default handlers
 * for some SMI events are provided in soc/intel/common/smihandler.c
 */
extern const smi_handler_t southbridge_smi[32];

/*
 * This function should be implemented in SOC specific code to handle
 * the SMI event on SLP_EN. The default functionality is provided in
 * soc/intel/common/smihandler.c
 */
void southbridge_smi_sleep(const struct smm_save_state_ops *save_state_ops);

/*
 * This function should be implemented in SOC specific code to handle
 * SMI_APM event. The default functionality is provided in
 * soc/intel/common/smihandler.c
 */
void southbridge_smi_apmc(const struct smm_save_state_ops *save_state_ops);

/*
 * This function should be implemented in SOC specific code to handle
 * SMI_PM1 event. The default functionality is provided in
 * soc/intel/common/smihandler.c
 */
void southbridge_smi_pm1(const struct smm_save_state_ops *save_state_ops);

/*
 * This function should be implemented in SOC specific code to handle
 * SMI_GPE0 event. The default functionality is provided in
 * soc/intel/common/smihandler.c
 */
void southbridge_smi_gpe0(const struct smm_save_state_ops *save_state_ops);

/*
 * This function should be implemented in SOC specific code to handle
 * SMI_TCO event. The default functionality is provided in
 * soc/intel/common/smihandler.c
 */
void southbridge_smi_tco(const struct smm_save_state_ops *save_state_ops);

/*
 * This function should be implemented in SOC specific code to handle
 * SMI PERIODIC_STS event. The default functionality is provided in
 * soc/intel/common/smihandler.c
 */
void southbridge_smi_periodic(const struct smm_save_state_ops *save_state_ops);

/*
 * This function returns a 1 or 0 depending on whether disable_busmaster
 * needs to be done for the specified device on S5 entry
 */
int smm_disable_busmaster(device_t dev);

/*
 * Returns gnvs pointer within SMM context
 */
struct global_nvs_t *smm_get_gnvs(void);

extern const struct smm_save_state_ops em64t100_smm_ops;

extern const struct smm_save_state_ops em64t101_smm_ops;
#endif
