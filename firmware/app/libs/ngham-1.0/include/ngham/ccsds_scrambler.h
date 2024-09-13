/*
 * ccsds_scrambler.h
 * 
 * Copyright The NGHam Contributors.
 * 
 * This file is part of NGHam.
 * 
 * NGHam is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * NGHam is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with NGHam. If not, see <http:/\/www.gnu.org/licenses/>.
 * 
 */

/**
 * \brief CCSDS scrambler definition.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * \author Miguel Boing <miguelboing13@gmail.com>
 * 
 * \version 1.0.0
 * 
 * \date 2024/08/26
 * 
 * \defgroup ccsds-scrambler CCSDS Scrambler
 * \ingroup ngham
 * \{
 */

#ifndef CCSDS_SCRAMBLER_H_
#define CCSDS_SCRAMBLER_H_

#include <stdint.h>

extern const uint8_t ccsds_poly[255];

/**
 * \brief .
 *
 * \return .
 */
void ccsds_scrambler_init(void);

/**
 * \brief .
 *
 * \param[in] data .
 *
 * \return .
 */
uint8_t ccsds_scrambler_xor(uint8_t data);

#endif /* CCSDS_SCRAMBLER_H_ */

/**< \} End of ccsds-scrambler group */
