/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef CSP_IFLIST_H_
#define CSP_IFLIST_H_

#include <csp/csp_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Add interface to list
 * @param ifc Pointer to interface to add
 */
void csp_iflist_add(csp_iface_t *ifc);

/**
 * Lookup interface by name
 * @param name String with interface name
 * @return Pointer to interface or NULL if not found
 */
csp_iface_t * csp_iflist_get_by_name(const char *name);

/**
 * Print list of interfaces to stdout
 */
void csp_iflist_print(void);

/**
 * Return list of registered interfaces.
 */
csp_iface_t * csp_iflist_get(void);

#ifdef __cplusplus
}
#endif
#endif /* CSP_IFLIST_H_ */
