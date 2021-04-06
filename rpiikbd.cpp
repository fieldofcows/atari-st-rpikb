#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>
#include <termios.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <queue>
#include <math.h>
#include "st_key_lookup.h"
#include "6301.h"
#include "THD6301.h"
#include "options.h"

BYTE    ST_Key_Down[128];
THD6301 Ikbd;
int     mousek = 0;
BYTE    stick[8];
extern "C" { RPIConfig IKBDConfig; }

int kbd = -1;
int ms = -1;
int ser = -1;
unsigned char mouse_data[3] = { 0xf8, 0x00, 0x00 };
std::queue<BYTE> rx_from_st;
static bool         terminate_cpu = false;
static COUNTER_VAR  tick_count = 0;

void handle_send_to_st() {
    if (hd6301_check_for_tx_byte()) {
        BYTE bt = hd6301_read_tx_byte();
        printf("6301 -> ST %X\n", bt);
        write(ser, &bt, 1);
    }

}

void handle_rx_from_st() {
    if (!rx_from_st.empty()) {
        BYTE bt = rx_from_st.front();
        printf("ST -> 6301 %X\n", bt);
        rx_from_st.pop();
        Ikbd.rdrs = bt;
        hd6301_receive_byte(bt);        
    }
}

void send_mouse(unsigned char x, unsigned char y)
{
    printf("Mouse: %02X %02X %02X\n", mouse_data[0], x, y);
    mouse_data[1] = x;
    mouse_data[2] = y;
    Ikbd.MouseVblDeltaX = x;
    Ikbd.MouseVblDeltaY = y;
    //mousek = mouse_data[0] >> 1;
    write(ser, mouse_data, sizeof(mouse_data));
}

void handle_keyboard() 
{
    struct input_event evts[100];
    // Read any pending events
    int count = read(kbd, evts, sizeof(evts));
    if (count != -1) {
        count /= sizeof(struct input_event);
        for (int i = 0; i < count; ++i)
        {
            // Look for a keyboard event
            if ((evts[i].type == EV_KEY) && (evts[i].code < 128))
            {
                unsigned char st_key = st_key_lookup_gb[evts[i].code];
                if (st_key & 0x7f)
                {
                    ST_Key_Down[st_key] = evts[i].value;
                    //printf("Key: %02X\n", st_key | (evts[i].value ? 0 : 0x80));
                }
            }
        }
    }
}

void handle_mouse() 
{
    struct input_event evts[100];
    // Read any pending events
    int count = read(ms, evts, sizeof(evts));
    if (count != -1) {
        count /= sizeof(struct input_event);
        for (int i = 0; i < count; ++i)
        {
            // An EV_KEY event for a mouse represents the mouse buttons
            if (evts[i].type == EV_KEY)
            {
                if (evts[i].code == BTN_LEFT)
                    mousek = (mousek & 0xfe) | (evts[i].value ? 1 : 0);
                if (evts[i].code == BTN_RIGHT)
                    mousek = (mousek & 0xfd) | (evts[i].value ? 2 : 0);
                //send_mouse(0, 0);
            }
            // Check for mouse movement
            if (evts[i].type == EV_REL)
            {
                // Clamp to +/- 100
                int value = evts[i].value;
                if (value > 100) 
                    value = 100;
                if (value < -100) 
                    value = -100;
                unsigned char x = (evts[i].code == 0) ? (unsigned char)value : 0;
                unsigned char y = (evts[i].code == 1) ? (unsigned char)value : 0;
                Ikbd.click_x = 0;
                Ikbd.click_y = 0;
                Ikbd.MouseVblDeltaX = x;
                Ikbd.MouseVblDeltaY = y;
                Ikbd.MouseCyclesPerTickX = x ? (1000 / abs(x)) : 0;
                Ikbd.MouseCyclesPerTickY = y ? (1000 / abs(y)) : 0;
                Ikbd.MouseNextTickX = Ikbd.MouseCyclesPerTickY = tick_count / 100;
                //send_mouse(x, y);
            }
        }
    }
}

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

void* run_cpu(void*) 
{
    timespec last_time;

    // Initialise the keyboard controller
    Ikbd.Init();
    Ikbd.ResetChip(1);
    Ikbd.MouseCyclesPerTickX = 1000;
    Ikbd.MouseCyclesPerTickY = 1000;

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &last_time);
    while (!terminate_cpu) {
        // Cycle the CPU, keeping track of how many nanoseconds since the last cycle
        timespec cur_time;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);
        timespec diff_time = diff(last_time, cur_time);
        last_time = cur_time;
        tick_count += diff_time.tv_nsec;

        hd6301_run_cycles(tick_count / 100);
        handle_send_to_st();
        handle_rx_from_st();
    }
    return NULL;
}

int instr_exec ();

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Usage: rpiikbd kb_device mouse_device\n");
		return -1;
	}

    // Open the keyboard, mouse and serial port
	kbd = open(argv[1], O_RDONLY | O_NONBLOCK);
	ms = open(argv[2], O_RDONLY | O_NONBLOCK);
	ser = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);

    if (kbd == -1) {
        printf("Could not open keyboard device\n");
    }
    if (ms == -1) {
        printf("Could not open mouse device\n");
    }
    if (ser == -1) {
        printf("Could not open serial device\n");
    }

	if (ioctl(kbd, EVIOCGRAB, 1) != 0) {
		return -1;
    }
	if (ioctl(ms, EVIOCGRAB, 1) != 0) {
		return -1;
    }

    // Set serial port options
    struct termios seropt;
	tcgetattr(ser, &seropt);
	seropt.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
	seropt.c_iflag = IGNPAR;
	seropt.c_oflag = 0;
	seropt.c_lflag = 0;
	tcflush(ser, TCIFLUSH);
	tcsetattr(ser, TCSANOW, &seropt);

    //pthread_t cpu_thread;
    //pthread_create(&cpu_thread, NULL, run_cpu, NULL);

    // Initialise the keyboard controller
    Ikbd.Init();
    Ikbd.ResetChip(1);
    Ikbd.MouseCyclesPerTickX = 1000;
    Ikbd.MouseCyclesPerTickY = 1000;
    hd6301_run_cycles(0);

    // Keep track of time so we can get a fairly accurate clock cycle
    timespec last_time;
    timespec frame_time;
    timespec second_time;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &last_time);
    frame_time.tv_nsec = 0;
    second_time.tv_nsec = 0;
    int count = 0;

    while (1)
    {
        // Calculate the time the last cycle took
        timespec cur_time;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);
        timespec diff_time = diff(last_time, cur_time);
        last_time = cur_time;
        frame_time.tv_nsec += diff_time.tv_nsec;
        second_time.tv_nsec += diff_time.tv_nsec;


        if (frame_time.tv_nsec > (1000 * 1000 * 20)) 
        {
            ++count;
            hd6301_run_clocks(20000);

            handle_send_to_st();
            handle_rx_from_st();
            handle_keyboard();
            handle_mouse();
            frame_time.tv_nsec = 0;
        }
        if (second_time.tv_nsec > (1000 * 1000 * 1000)) 
        {
            //printf("Count = %d\n", count);
            static bool b = false;
            if (!b) {
                b = true;
                rx_from_st.push(0x80);
                rx_from_st.push(0x1);
            }
            second_time.tv_nsec = 0;
            count = 0;
        }

        //sleep(1);
        //printf("ns = %llu\n", ns);
    }

    //pthread_join(cpu_thread, NULL);
}