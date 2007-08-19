/*
 * lar - LinuxBIOS archiver
 *
 * Copyright (C) 2006 coresystems GmbH
 * (Written by Stefan Reinauer <stepan@coresystems.de> for coresystems GmbH)
 * Copyright (C) 2007 Patrick Georgi <patrick@georgi-clan.de>
 *
 * This file is dual-licensed. You can choose between:
 *   - The GNU GPL, version 2, as published by the Free Software Foundation
 *   - The revised BSD license (without advertising clause)
 *
 * ---------------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA, 02110-1301 USA
 * ---------------------------------------------------------------------------
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ---------------------------------------------------------------------------
 */

#include <stdint.h>

#define MAGIC "LARCHIVE"
#define MAX_PATHLEN 1024
#define BOOTBLOCK_SIZE 16384

typedef uint32_t u32;

struct lar_header {
	char magic[8];
	u32 len;
	u32 reallen;
	u32 checksum;
	u32 compchecksum;
	u32 offset;
	/* Compression:
	 * 0 = no compression
	 * 1 = lzma
	 * 2 = nrv2b
	 */
	u32 compression;
};

enum compalgo { none = 0, lzma = 1, nrv2b = 2 };

typedef void (*compress_func) (char *, u32, char *, u32 *);
typedef void (*uncompress_func) (char *, char *, u32);

void compress_impossible(char *in, u32 in_len, char *out, u32 *out_len);
void do_no_compress(char *in, u32 in_len, char *out, u32 *out_len);
#ifdef CONFIG_COMPRESSION_LZMA
void do_lzma_compress(char *in, u32 in_len, char *out, u32 *out_len);
#else
#define do_lzma_compress compress_impossible
#endif
#ifdef CONFIG_COMPRESSION_NRV2B
void do_nrv2b_compress(char *in, u32 in_len, char *out, u32 *out_len);
#else
#define do_nrv2b_compress compress_impossible
#endif

void uncompress_impossible(char *, char *, u32);
void do_no_uncompress(char *, char *, u32);
#ifdef CONFIG_COMPRESSION_LZMA
void do_lzma_uncompress(char *, char *, u32);
#else
#define do_lzma_uncompress uncompress_impossible
#endif
#ifdef CONFIG_COMPRESSION_NRV2B
void do_nrv2b_uncompress(char *, char *, u32);
#else
#define do_nrv2b_uncompress uncompress_impossible
#endif

static compress_func compress_functions[] = {
	do_no_compress,
	do_lzma_compress,
	do_nrv2b_compress,
};

static uncompress_func uncompress_functions[] = {
	do_no_uncompress,
	do_lzma_uncompress,
	do_nrv2b_uncompress,
};

static const char *algo_name[] = {
	"",
	"lzma",
	"nrv2b",
};
