#include "capture.h"
#include <QtConcurrent/QtConcurrent>

QMutex lock;
QMap<QString, int> hostnames_map;
boolean exit_threads = false;

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
        QString ip_addr_str = QString("%1.%2.%3.%4").arg(ih->saddr.byte1).arg(ih->saddr.byte2).arg(ih->saddr.byte3).arg(ih->saddr.byte4);

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
