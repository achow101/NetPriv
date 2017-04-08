#include "capture.h"
#include <QtConcurrent/QtConcurrent>
#include <string.h>
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <processthreadsapi.h>
#include <WinBase.h>
#include <Psapi.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <malloc.h>

#define MAX_NAME_LEN 1000

QMutex lock;
QMap<QString, int> hostnames_map;
QMap<QString, int> programs_map;
bool exit_threads = false;

int pcap_thread()
{
    pcap_if_t *alldevs, *d;
    char errbuf[PCAP_ERRBUF_SIZE];

    // Get all devices
    if (pcap_findalldevs_ex((char *)PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
    {
        fprintf(stderr,"Error in pcap_findalldevs_ex: %s\n", errbuf);
        return -1;
    }

    // Threads for each device
    for(d=alldevs; d; d=d->next)
    {
        pcap_t *dev;
        if ((dev = pcap_open(d->name, 100, PCAP_OPENFLAG_PROMISCUOUS, 20, NULL, errbuf)) == NULL)
        {
            fprintf(stderr,"\nError opening adapter\n");
            return -1;
        }

        // Get ip address of this device
        pcap_addr_t *a;

        /* IP addresses */
        QString filter_expression;
        for(a=d->addresses;a;a=a->next) {
            if (a->addr)
            {
                filter_expression.append("ip.src == ");
                filter_expression.append(iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
                filter_expression.append(" || ");
            }
        }
        struct bpf_program fcode;
        pcap_compile(dev, &fcode, filter_expression.toStdString().c_str(), 1, 0);
        pcap_setfilter(dev, &fcode);

        QtConcurrent::run(capture_thread, dev);
    }
    return 0;
}

void capture_thread(pcap_t *fp)
{
    int res;
    struct pcap_pkthdr *header;
    const unsigned char *pkt_data;

    // Read the packets
    while((res = pcap_next_ex( fp, &header, &pkt_data)) >= 0 && !exit_threads)
    {

        if(res == 0)
        {
            /* Timeout elapsed */
            continue;
        }

        ip_header *ih;

        /* retireve the position of the ip header */
        ih = (ip_header *) (pkt_data + 14); //length of ethernet header

        // Get the source port from udp header
        unsigned int ip_len = (ih->ver_ihl & 0xf) * 4;
        udp_header *uh = (udp_header *) ((u_char*)ih + ip_len);
        u_short src_port = ntohs( uh->sport );

        // Get PID of process using the port from windows
        PMIB_TCPTABLE2 pTcpTable = (MIB_TCPTABLE2 *) malloc(sizeof (MIB_TCPTABLE2));
        ULONG ulSize = sizeof (MIB_TCPTABLE);
        // Make an initial call to GetTcpTable2 to
        // get the necessary size into the ulSize variable
        if (GetTcpTable2(pTcpTable, &ulSize, TRUE) == ERROR_INSUFFICIENT_BUFFER)
        {
            free(pTcpTable);
            pTcpTable = (MIB_TCPTABLE2 *) malloc(ulSize);
            if (pTcpTable == NULL) {
                printf("Error allocating memory\n");
            }
        }
        // Make a second call to GetTcpTable2 to get
        // the actual data we require
        DWORD port_pid = 0;
        if (GetTcpTable2(pTcpTable, &ulSize, TRUE) == NO_ERROR)
        {
            for (int i = 0; i < (int) pTcpTable->dwNumEntries; i++)
            {
                MIB_TCPROW2 loc = pTcpTable->table[i];
                if((int)src_port == (int)loc.dwLocalPort)
                {
                    port_pid = loc.dwOwningPid;
                    break;
                }
            }
        }
        // Get the process name from the PID
        HANDLE proc = OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, port_pid);
        DWORD dw = GetLastError();
        LPWSTR proc_name = new WCHAR[MAX_NAME_LEN];
        DWORD proc_name_len = MAX_NAME_LEN;
        QString proc_name_str;
        DWORD ret = 0;
        if((ret = GetModuleFileNameEx(proc, NULL, proc_name, proc_name_len)) != 0)
        {
            proc_name_str = QString::fromLocal8Bit((const char *)proc_name);
        }
        else
        {
            proc_name_str = QString("Unknown Process");
        }
        dw = GetLastError();

        // Get destination IP address
        QString ip_addr_str = QString("%1.%2.%3.%4").arg(ih->daddr.byte1).arg(ih->daddr.byte2).arg(ih->daddr.byte3).arg(ih->daddr.byte4);

        // Obtain lock
        QMutexLocker locker(&lock);

        // Update packet counter for ip address
        hostnames_map[ip_addr_str]++;
        qDebug() << ip_addr_str << QString::number(hostnames_map[ip_addr_str]);

        // Update packet counter for process name
        programs_map[proc_name_str]++;
        qDebug() << proc_name_str << QString::number(programs_map[proc_name_str]);

        locker.~QMutexLocker();
    }
}

void stop_threads()
{
    exit_threads = true;
}

QMap<QString, int> get_hostnames_list()
{
    return hostnames_map;
}

QMap<QString, int> get_programs_map()
{
    return programs_map;
}

/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS    12
char *iptos(u_long in)
{
    static char output[IPTOSBUFFERS][3*4+3+1];
    static short which;
    u_char *p;

    p = (u_char *)&in;
    which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
    _snprintf_s(output[which], sizeof(output[which]), sizeof(output[which]),"%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return output[which];
}
