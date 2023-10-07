#include "task_network.h"
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "FreeRTOS.h"
#include "task.h"
#include "task_prio.h"

#include "global_signal.h"
#include "can.h"
#include "panic.h"
#include "parameter.h"

static TaskHandle_t network_taskhandle;

#define CONNECT_TIMEOUT_MS 10000

#define UDP_PORT_RECEIVE 50000
#define UDP_PORT_TRANS   50100
#define HEARTBEAT_TIMEOUT_MS 1000

// thats magic
//extern struct netif gnetif;

void recCallBack(void *vp, struct udp_pcb *pcb, struct pbuf *p,
		const ip_addr_t *addr, u16_t port)
{
	// is ok for FreeRTOS function
	// or from ISR ????
	MainEnvironement_t *MainEnvironement = (MainEnvironement_t*) vp;
	if (p->len == sizeof(can_msg_t))
	{
		xQueueSend(MainEnvironement->from_host, p->payload, XDELAY);
		// should return immediately
	}
	else
	{
		printf("\np ->len %i", p->len);
	}
	pbuf_free(p);
}

void network_thread(MainEnvironement_t *MainEnvironement)
{
	if (cyw43_arch_init_with_country(CYW43_COUNTRY_GERMANY))
	{
		app_panic("cyw43 init failed");
		return;
	}

	xEventGroupSetBits(MainEnvironement -> mainEventGroup, EVENT_MASK_CYW43_INIT);
	cyw43_arch_enable_sta_mode();

	if (cyw43_arch_wifi_connect_timeout_ms(PARA_SSID, PARA_PWD,
	CYW43_AUTH_WPA2_AES_PSK, CONNECT_TIMEOUT_MS))
	{
		xEventGroupSetBits(MainEnvironement ->mainEventGroup, EVENT_MASK_FAIL);
		cyw43_arch_deinit();
		app_panic("failed to connect");
		return;
	}

	xEventGroupSetBits(MainEnvironement ->mainEventGroup, EVENT_MASK_CONNECTED);
	printf("Connected.\n");

	struct udp_pcb *broadcast_receive_socket;
	ip4_addr_t broadcast_destination;

	EventBits_t bits = 0;
	int reciece_counter = 0;

	IP4_ADDR(&broadcast_destination, 255, 255, 255, 255); //non routing local broadcast
	broadcast_receive_socket = udp_new();

	udp_bind(broadcast_receive_socket, NULL, UDP_PORT_RECEIVE); //to allow receiving multicast
	udp_recv(broadcast_receive_socket, recCallBack, MainEnvironement); //recCallBack is the callback function that will be called every time you    receive multicast

	while (true)
	{
		can_msg_t msg;
		BaseType_t result;

		// TODO: what triggers a package send ????
		result = xQueueReceive(MainEnvironement->to_host, &msg, XDELAY);

		if (result == pdPASS)
		{
			struct pbuf *p;
			p = pbuf_alloc(PBUF_TRANSPORT, sizeof(can_msg_t), PBUF_RAM);
			memcpy(p->payload, &msg, sizeof(can_msg_t));

			udp_sendto(broadcast_receive_socket, p, &broadcast_destination,
			UDP_PORT_TRANS); //send a multicast packet
			pbuf_free(p);
		}
		else
		{
			puts("You should not be here");
		}
	}
}

void network_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((CALLEE) network_thread, "NETWORK", configMINIMAL_STACK_SIZE,
			MainEnvironement,
			NETWORK_TASK_PRIO, &network_taskhandle);
}

