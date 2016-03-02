#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "rfcomm-server.h"

int rfcomm_server(void)
{
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    //char myaddr[18] = "84:7E:40:15:07:F3";
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 11 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    //str2ba( myaddr, &loc_addr.rc_bdaddr );
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 11;

    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    ba2str( &loc_addr.rc_bdaddr, buf );
    printf("Binded to %s channel %d.\n", buf, loc_addr.rc_channel);

    // put socket into listening mode
    listen(s, 1);
    printf("Listening.\n");

    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    // read data from the client
    bytes_read = read(client, buf, sizeof(buf));
    if( bytes_read > 0 ) {
        printf("received [%s]\n", buf);
    }
    sprintf(buf,"to Android.");
    printf("sent [%s]\n", buf);
    write(client, buf, sizeof(buf));

    // close connection
    close(client);
    close(s);
    return 0;
}