#ifndef CAPTURE_H
#define CAPTURE_H

#include <QMap>
#include <QMutex>
#include <pcap.h>

/* 4 bytes IP address */
typedef struct ip_address{
    unsigned char byte1;
    unsigned char byte2;
    unsigned char byte3;
    unsigned char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header{
    unsigned char  ver_ihl;        // Version (4 bits) + Internet header length (4 bits)
    unsigned char  tos;            // Type of service
    unsigned short tlen;           // Total length
    unsigned short identification; // Identification
    unsigned short flags_fo;       // Flags (3 bits) + Fragment offset (13 bits)
    unsigned char  ttl;            // Time to live
    unsigned char  proto;          // Protocol
    unsigned short crc;            // Header checksum
    ip_address  saddr;      // Source address
    ip_address  daddr;      // Destination address
    unsigned int   op_pad;         // Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header{
    u_short sport;          // Source port
    u_short dport;          // Destination port
    u_short len;            // Datagram length
    u_short crc;            // Checksum
}udp_header;

int pcap_thread();
void capture_thread(pcap_t *fp);
void stop_threads();

// Stuff to get data from maps
QMap<QString, int> get_hostnames_list();
QMap<QString, int> get_programs_map();

char *iptos(u_long in);

#endif // CAPTURE_H
