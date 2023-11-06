/*
 * rsc.h
 * 
 * Copyright The RSCLib Contributors.
 * 
 * This file is part of RSCLib.
 * 
 * RSCLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * RSCLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with RSCLib. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief Reed-Solomon C library definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 0.1.0
 * 
 * \date 2022/03/06
 * 
 * \defgroup rsclib Reed Solomon C Library
 * \{
 */

#ifndef RSC_H_
#define RSC_H_

#include <stdint.h>

#define RSC_VERSION         "v0.1.0"

/**
 * \brief Reed-Solomon codec control block.
 */
typedef struct
{
    int mm;                 /**< Bits per symbol */
    int nn;                 /**< Symbols per block (= (1 << mm) - 1) */
    uint8_t alpha_to[256];  /**< log lookup table */
    uint8_t index_of[256];  /**< Antilog lookup table */
    uint8_t genpoly[64];    /**< Generator polynomial */
    int nroots;             /**< Number of generator roots = number of parity symbols */
    int fcr;                /**< First consecutive root, index form */
    int prim;               /**< Primitive element, index form */
    int iprim;              /**< prim-th root of 1, index form */
    int pad;                /**< Padding bytes in shortened block */
} reed_solomon_t;

/**
 * \brief Reed-Solomon codec initialization.
 *
 * \param[in] symsize is the symbol size in bits.
 *
 * \param[in] gfpoly is the field generator polynomial coefficients.
 *
 * \param[in] fcr is the first root of RS code generator polynomial in index form.
 *
 * \param[in] prim is the primitive element to generate polynomial roots.
 *
 * \param[in] nroots is the RS code generator polynomial degree (number of roots).
 *
 * \param[in] pad is the padding bytes at front of shortened block.
 *
 * \param[in,out] rs is the reed_solomon_t structure to initialize with the given configuration parameters.
 *
 * \return The status/error code.
 */
int rsc_init(int symsize, int gfpoly, int fcr, int prim, int nroots, int pad, reed_solomon_t *rs);

/**
 * \brief Computes the parity data of a given byte sequence.
 *
 * \param[in] rs is the structure with the Reed-Solomon configuration.
 *
 * \param[in] data is the array of bytes to compute the parity data.
 *
 * \param[in] parity is the computed parity data.
 *
 * \param[in] parity_len is the length, in bytes, of the computed parity data.
 *
 * \return None.
 */
void rsc_encode(reed_solomon_t *rs, uint8_t *data, uint8_t *parity, uint8_t *parity_len);

/**
 * \brief Decodes a Reed-Solomon sequence (data + parity).
 *
 * \param[in] rs is the structure with the Reed-Solomon configuration.
 *
 * \param[in] pkt is the byte sequence to decode (data + parity).
 *
 * \param[in,out] data is the decoded data from pkt.
 *
 * \param[in,out] err_pos is a pointer to store the position of the detected errors.
 *
 * \param[in,out] num_err is the number of detected errors.
 *
 * \return The status/error code.
 */
int rsc_decode(reed_solomon_t *rs, uint8_t *pkt, uint8_t *data, uint8_t *err_pos, uint8_t *num_err);

#endif /* RSC_H_ */

/**< \} End of rsclib group */
