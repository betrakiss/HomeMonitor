#include "EthernetController.h"

#include <EtherCard.h>

#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,1,200 };
// gateway ip address
static byte gwip[] = { 192,168,1,1 };
#endif

// ethernet mac address - must be unique on your network
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char page[] PROGMEM =
"HTTP/1.0 503 Service Unavailable\r\n"
"Content-Type: text/html\r\n"
"Retry-After: 600\r\n"
"\r\n"
"<html>"
"<head><title>"
"Service Temporarily Unavailable"
"</title></head>"
"<body>"
"<h3>This service is currently unavailable</h3>"
"<p><em>"
"The main server is currently off-line.<br />"
"Please try again later."
"</em></p>"
"</body>"
"</html>"
;

void EthernetController::SendMessage()
{
	if (ether.packetLoop(ether.packetReceive())) {
		memcpy_P(ether.tcpOffset(), page, sizeof page);
		ether.httpServerReply(sizeof page - 1);
	}
}

void EthernetController::Initialize()
{
	if (ether.begin(sizeof Ethernet::buffer, mymac, 9) == 0)
		Serial.println("Failed to access Ethernet controller");
#if STATIC
	ether.staticSetup(myip, gwip);
#else
	Serial.println("Begin DHCP Query");
	if (!ether.dhcpSetup())
		Serial.println("DHCP failed");
#endif

	ether.printIp("IP:  ", ether.myip);
	ether.printIp("GW:  ", ether.gwip);
	ether.printIp("DNS: ", ether.dnsip);
}