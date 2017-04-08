#include "capture.h"
#include <QtConcurrent/QtConcurrent>
#include <string.h>

QMutex lock;
QMap<QString, int> hostnames_map;
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
        char *filter_expression = (char *)malloc(sizeof(char*));
        for(a=d->addresses;a;a=a->next) {
            if (a->addr)
                strcat(filter_expression, "ip.src == ");
                strcat(filter_expression, iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
                strcat(filter_expression, " || ");
        }
        struct bpf_program fcode;
        pcap_compile(dev, &fcode, filter_expression, 1, 0);
        pcap_setfilter(dev, &fcode);
        free(filter_expression);

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

        // Get IP address
        QString ip_addr_str = QString("%1.%2.%3.%4 ").arg(ih->daddr.byte1).arg(ih->daddr.byte2).arg(ih->daddr.byte3).arg(ih->daddr.byte4);

        // Obtain lock and add one
        QMutexLocker locker(&lock);
        hostnames_map[ip_addr_str]++;
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
