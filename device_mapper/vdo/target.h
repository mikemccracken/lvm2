/*
 * Copyright (C) 2018 Red Hat, Inc. All rights reserved.
 *
 * This file is part of the device-mapper userspace tools.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU Lesser General Public License v.2.1.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef DEVICE_MAPPER_VDO_TARGET_H
#define DEVICE_MAPPER_VDO_TARGET_H

#include <stdbool.h>
#include <stdint.h>

//----------------------------------------------------------------

enum dm_vdo_operating_mode {
	DM_VDO_MODE_RECOVERING,
	DM_VDO_MODE_READ_ONLY,
	DM_VDO_MODE_NORMAL
};

enum dm_vdo_compression_state {
	DM_VDO_COMPRESSION_ONLINE,
	DM_VDO_COMPRESSION_OFFLINE
};

enum dm_vdo_index_state {
	DM_VDO_INDEX_ERROR,
	DM_VDO_INDEX_CLOSED,
	DM_VDO_INDEX_OPENING,
	DM_VDO_INDEX_CLOSING,
	DM_VDO_INDEX_OFFLINE,
	DM_VDO_INDEX_ONLINE,
	DM_VDO_INDEX_UNKNOWN
};

struct dm_vdo_status {
	char *device;
	enum dm_vdo_operating_mode operating_mode;
	bool recovering;
	enum dm_vdo_index_state index_state;
	enum dm_vdo_compression_state compression_state;
	uint64_t used_blocks;
	uint64_t total_blocks;
};

void dm_vdo_status_destroy(struct dm_vdo_status *s);

#define VDO_MAX_ERROR 256

struct dm_vdo_status_parse_result {
	char error[VDO_MAX_ERROR];
	struct dm_vdo_status *status;
};

struct dm_pool;

// Parses the status line from the kernel target.
bool dm_vdo_status_parse(struct dm_pool *mem, const char *input,
			 struct dm_vdo_status_parse_result *result);

//----------------------------------------------------------------

#endif
