/* This file is a part of the Stuart. */

#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

/* Configure port parameters. */
void port_init(int fd, struct termios *ter)
{    
    /* Set port speed. */
    cfsetispeed(ter, B4800);
    cfsetospeed(ter, B4800);

    /* Enable reciever and ignore modem control lines. */
    ter->c_cflag |= (CREAD | CLOCAL);

    /* Set to 8N1 type. */
    ter->c_cflag &= ~PARENB;
    ter->c_cflag &= ~CSTOPB;
    
    /* Set 8 bit character. */
    //ter->c_cflag &= ~CSIZE;
    ter->c_cflag |= CS8;

    /* Dissable hardware flow control. */
    ter->c_cflag &= ~CRTSCTS;

    /* Use canonical input. ECHO not set, because that can makes a loop. */
    ter->c_lflag &= ~ICANON;

    /* No parity checking. */
    ter->c_iflag &= ~INPCK;

    /* Dissable hardware flow control for input. */
    ter->c_iflag &= ~(IXON | IXOFF | IXANY);

    /* Not a raw output mode. */
    ter->c_oflag |= OPOST;

    tcsetattr(fd, TCSANOW, ter);
    
    return;
}

/* Return file descriptor of the serial port. */
int port_open(char *portname)
{
    int fd;

    fd = open(portname, O_RDWR | O_NDELAY | O_NOCTTY);
    if(fd < 0) {
        perror("open");
    } else {
        if(fcntl(fd, F_SETFL, 0) < 0) {
            perror("fnctl");
        }
    }

    return fd;
}

int port_close(int fd)
{
    int ret;

    ret = close(fd);
    if(ret < 0) {
        perror("close");
    }

    return ret;
}

ssize_t port_write(int fd, char *buf, size_t size)
{
    ssize_t ret;

    ret = write(fd, buf, size);
    if(ret < 0) {
        perror("write");
    }

    return ret;
}


ssize_t port_read(int fd, char *buf, size_t size)
{
    ssize_t ret;

    ret = read(fd, buf, size);
    if(ret < 0) {
        perror("read");
    }

    return ret;
}

#if 0
ssize_t port_read_string(int fd, char *buf, size_t size)
{
    size_t buf_offset = 0, done = 0;
    do {
        fd_set readfds;
        struct timeval tv;
        char rbuf[256];

        memset(rbuf, 0, 256);
        done = 0;

        FD_ZERO(&readfds);
        FD_SET(portfd, &readfds);

        /* Wait a one second. */
        tv.tv_sec = 2;
        tv.tv_usec = 0;
                
        if(select(portfd + 1, &readfds, NULL, NULL, &tv) < 0) {
            perror("select");
        } else {
            done = FD_ISSET(portfd, &readfds);
            if(done) {
                printf("Reading...\n");
                            
                buflen = port_read(portfd, rbuf, 256);
                if(buflen < 0) {
                    printf("Some error occured when recieve data from device.\n");
                    goto exit;
                }
                int i;
                for(i = 0; i < buflen; i++) {
                    buf[buf_offset + i] = rbuf[i];
                }
                buf_offset += buflen;
            }
        }
        printf("buflen = %d; buf_offset = %d; done = %d\n", buflen, buf_offset, done);
    } while(buf[buf_offset - 1] != '\0');

}
#endif

int main(int argc, char *argv[])
{
    struct termios ter, oldter;
    int portfd, status;

    portfd = port_open("/dev/ttyS0");
    if(portfd < 0) {
        exit(EXIT_FAILURE);
    }

    /* Save current port settings for restore it later. */
    tcgetattr(portfd, &oldter);

    /* Avoid all bits in new structure. */
    memset(&ter, 0, sizeof(ter));
    port_init(portfd, &ter);

    /* Set high level signal on DTR line. */
    ioctl(portfd, TIOCMGET, &status);    
    status |= TIOCM_DTR;
    status |= TIOCM_RTS;
    ioctl(portfd, TIOCMSET, &status);
    
    /* Flush input/output buffers of the port. */
    tcflush(portfd, TCIOFLUSH);
    
    /* Interactive I/O loop. */
    while("All humans alive") {
        char buf[256];
        int buflen;
        
        printf("for quit, type 'q'(for help, type '?') "
               "else send string to port:\n");
        printf("> ");
        
        fgets(buf, 256, stdin);
        buflen = strlen(buf);
        /* Skip newline character. */
        buf[buflen - 1] = '\0';
        
        if(!strcmp(buf, "q")) {
            break;
        } else if(!strcmp(buf, "?")) {
            printf("some help.\n");
        } else {
            if(port_write(portfd, buf, buflen) < 0) {
                printf("Some error occured when send data to device.\n");
            } else {
                usleep(500000);
                buflen = port_read(portfd, buf, 256);
                if(buflen < 0) {
                    printf("Some error occered when read data from device.\n");
                }
                printf("Read %d bytes: %s\n", buflen, buf);
            }
        }
    }

    exit:
    /* Restore old port settings. */
    tcsetattr(portfd, TCSANOW, &oldter);
    status &= ~TIOCM_DTR;
    status &= ~TIOCM_RTS;
    ioctl(portfd, TIOCMSET, &status);
    
    /* Close port. */
    port_close(portfd);
    
    return 0;
}
