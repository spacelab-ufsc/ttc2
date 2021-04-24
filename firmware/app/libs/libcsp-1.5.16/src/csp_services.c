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

/* #include <stdio.h> */
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

/* CSP includes */
#include <csp/csp.h>
#include <csp/csp_cmp.h>
#include <csp/csp_endian.h>

#include <csp/arch/csp_time.h>

#include <system/sys_log/sys_log.h>

int csp_ping(uint8_t node, uint32_t timeout, unsigned int size, uint8_t conn_options) {

	unsigned int i;
	uint32_t start, time, status = 0;

	/* Counter */
	start = csp_get_ms();

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_PING, timeout, conn_options);
	if (conn == NULL)
		return -1;

	/* Prepare data */
	csp_packet_t * packet;
	packet = csp_buffer_get(size);
	if (packet == NULL)
		goto out;

	/* Set data to increasing numbers */
	packet->length = size;
	for (i = 0; i < size; i++)
		packet->data[i] = i;

	/* Try to send frame */
	if (!csp_send(conn, packet, 0))
		goto out;

	/* Read incoming frame */
	packet = csp_read(conn, timeout);
	if (packet == NULL)
		goto out;

	/* Ensure that the data was actually echoed */
	for (i = 0; i < size; i++)
		if (packet->data[i] != i % (0xff + 1))
			goto out;

	status = 1;

out:
	/* Clean up */
	if (packet != NULL)
		csp_buffer_free(packet);
	csp_close(conn);

	/* We have a reply */
	time = (csp_get_ms() - start);

	if (status) {
		return time;
	} else {
		return -1;
	}

}

void csp_ping_noreply(uint8_t node) {

	/* Prepare data */
	csp_packet_t * packet;
	packet = csp_buffer_get(1);

	/* Check malloc */
	if (packet == NULL)
		return;

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_PING, 0, 0);
	if (conn == NULL) {
		csp_buffer_free(packet);
		return;
	}

	packet->data[0] = 0x55;
	packet->length = 1;

    /* printf("Ping ignore reply node %u.\r\n", (unsigned int) node); */
    sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "Ping ignore reply node ");
    sys_log_print_uint(node);
    sys_log_print_msg(".");
    sys_log_new_line();

	/* Try to send frame */
	if (!csp_send(conn, packet, 0))
		csp_buffer_free(packet);

	csp_close(conn);

}

void csp_reboot(uint8_t node) {
	uint32_t magic_word = csp_hton32(CSP_REBOOT_MAGIC);
	csp_transaction(CSP_PRIO_NORM, node, CSP_REBOOT, 0, &magic_word, sizeof(magic_word), NULL, 0);
}

void csp_shutdown(uint8_t node) {
	uint32_t magic_word = csp_hton32(CSP_REBOOT_SHUTDOWN_MAGIC);
	csp_transaction(CSP_PRIO_NORM, node, CSP_REBOOT, 0, &magic_word, sizeof(magic_word), NULL, 0);
}

void csp_ps(uint8_t node, uint32_t timeout) {

	/* Open connection */
	csp_conn_t * conn = csp_connect(CSP_PRIO_NORM, node, CSP_PS, 0, 0);
	if (conn == NULL)
		return;

	/* Prepare data */
	csp_packet_t * packet;
	packet = csp_buffer_get(95);

	/* Check malloc */
	if (packet == NULL)
		goto out;

	packet->data[0] = 0x55;
	packet->length = 1;

    /* printf("PS node %u: \r\n", (unsigned int) node); */
    sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "PS node ");
    sys_log_print_uint(node);
    sys_log_print_msg(": ");
    sys_log_new_line();

	/* Try to send frame */
	if (!csp_send(conn, packet, 0))
		goto out;

	while(1) {

		/* Read incoming frame */
		packet = csp_read(conn, timeout);
		if (packet == NULL)
			break;

		/* We have a reply, add our own NULL char */
		packet->data[packet->length] = 0;
        /* printf("%s", packet->data); */
        sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "");
        sys_log_print_str(packet->data);

		/* Each packet from csp_read must to be freed by user */
		csp_buffer_free(packet);
	}

    /* printf("\r\n"); */
    sys_log_new_line();

	/* Clean up */
out:
	if (packet != NULL)
		csp_buffer_free(packet);
	csp_close(conn);

}

void csp_memfree(uint8_t node, uint32_t timeout) {

	uint32_t memfree;

	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_MEMFREE, timeout, NULL, 0, &memfree, sizeof(memfree));
	if (status == 0) {
        /* printf("Network error\r\n"); */
        sys_log_print_event_from_module(SYS_LOG_ERROR, LIBCSP_MODULE_NAME, "Network error");
        sys_log_new_line();
		return;
	}

	/* Convert from network to host order */
	memfree = csp_ntoh32(memfree);

    /* printf("Free Memory at node %u is %"PRIu32" bytes\r\n", (unsigned int) node, memfree); */
    sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "Free Memory at node ");
    sys_log_print_uint(node);
    sys_log_print_msg(" is ");
    sys_log_print_uint(memfree);
    sys_log_print_msg(" bytes");
    sys_log_new_line();

}

void csp_buf_free(uint8_t node, uint32_t timeout) {

	uint32_t size = 0;

	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_BUF_FREE, timeout, NULL, 0, &size, sizeof(size));
	if (status == 0) {
        /* printf("Network error\r\n"); */
        sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "Network error");
        sys_log_new_line();
		return;
	}
	size = csp_ntoh32(size);
    /* printf("Free buffers at node %u is %"PRIu32"\r\n", (unsigned int) node, size); */
    sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "Free buffers at node ");
    sys_log_print_uint(node);
    sys_log_print_msg(" is ");
    sys_log_print_uint(size);
    sys_log_new_line();

}

int csp_get_uptime(uint8_t node, uint32_t timeout, uint32_t * uptime) {

	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_UPTIME, timeout, NULL, 0, uptime, sizeof(*uptime));
	if (status == sizeof(*uptime)) {
		*uptime = csp_ntoh32(*uptime);
		return CSP_ERR_NONE;
	}
	*uptime = 0;
	return CSP_ERR_TIMEDOUT;
}

void csp_uptime(uint8_t node, uint32_t timeout) {

	uint32_t uptime;
	int err = csp_get_uptime(node, timeout, &uptime);
	if (err == CSP_ERR_NONE) {
        /* printf("Uptime of node %u is %"PRIu32" s\r\n", (unsigned int) node, uptime); */
        sys_log_print_event_from_module(SYS_LOG_INFO, LIBCSP_MODULE_NAME, "Uptime of node ");
        sys_log_print_uint(node);
        sys_log_print_msg(" is ");
        sys_log_print_uint(uptime);
        sys_log_new_line();
	} else {
        /* printf("Network error\r\n"); */
        sys_log_print_event_from_module(SYS_LOG_ERROR, LIBCSP_MODULE_NAME, "Network error");
        sys_log_new_line();
	}

}

int csp_cmp(uint8_t node, uint32_t timeout, uint8_t code, int membsize, struct csp_cmp_message * msg) {
	msg->type = CSP_CMP_REQUEST;
	msg->code = code;
	int status = csp_transaction(CSP_PRIO_NORM, node, CSP_CMP, timeout, msg, membsize, msg, membsize);
	if (status == 0)
		return CSP_ERR_TIMEDOUT;

	return CSP_ERR_NONE;
}

