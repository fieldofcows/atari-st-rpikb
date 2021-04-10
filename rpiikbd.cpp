#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/input.h>
#include <signal.h>
#include <stdint.h>
#include <time.h>
#include <queue>
#include <math.h>
#include "st_key_lookup.h"
#include "6301.h"
#include "THD6301.h"
#include "options.h"
#include <asm-generic/ioctls.h>
#include <asm-generic/termbits.h>
#include "cpu.h"

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
        //printf("6301 -> ST %X\n", bt);
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

static int x_ticks = 0;
static int y_ticks = 0;
static COUNTER_VAR cur_tick = 0;
static COUNTER_VAR next_x_tick = 0;
static COUNTER_VAR next_y_tick = 0;

#define MIN(x, y) ((x) < (y) ? (x) : (y))

void set_mouse_speed_x(int x) 
{
    if (x != 0) {
        double freq = MIN((abs(x) - 1) * 10.0 + 100.0, 1500.0);
        freq = (x > 0) ? freq : -freq;
        x_ticks = 1000.0 / freq;
        next_x_tick = cur_tick + abs(x_ticks);
    }
    else 
    {
        x_ticks = 0;
        next_x_tick = 0;
    }
}

void set_mouse_speed_y(int y) 
{
    if (y != 0) {
        double freq = MIN((abs(y) - 1) * 10.0 + 100.0, 1500.0);
        freq = (y > 0) ? freq : -freq;
        y_ticks = 1000.0 / freq;
        next_y_tick = cur_tick + abs(y_ticks);
    }
    else 
    {
        y_ticks = 0;
        next_y_tick = 0;
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
    int num_events = 0;
    static int max_events = 0;
    struct input_event evts[100];

    int val_x = 0;
    int val_y = 0;

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
                    mousek = (mousek & 0xfd) | (evts[i].value ? 2 : 0);
                if (evts[i].code == BTN_RIGHT)
                    mousek = (mousek & 0xfe) | (evts[i].value ? 1 : 0);
                //send_mouse(0, 0);
            }
            // Check for mouse movement
            if (evts[i].type == EV_REL)
            {
                if (evts[i].code == 0) 
                {
                    val_x += evts[i].value;
                }
                else if (evts[i].code == 1) 
                {
                    val_y += evts[i].value;
                }
            }
        }
    }
    set_mouse_speed_x(val_x);
    set_mouse_speed_y(val_y);
}

extern unsigned int mouse_x_counter;
extern unsigned int mouse_y_counter;

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

int instr_exec ();

// Needs to be called every 1ms
void update_mouse() 
{

    ++cur_tick;
    
    if ((x_ticks != 0) && (cur_tick >= next_x_tick))
    {
        mouse_x_counter = (x_ticks > 0) ? _rotr(mouse_x_counter, 1) : _rotl(mouse_x_counter, 1);
        next_x_tick += abs(x_ticks);
    }
    if ((y_ticks != 0) && (cur_tick >= next_y_tick))
    {
        mouse_y_counter = (y_ticks > 0) ? _rotr(mouse_y_counter, 1) : _rotl(mouse_y_counter, 1);
        next_y_tick += abs(y_ticks);
    }

}

int setbaud(int fd, int speed)
{
	struct termios2 tio;
	ioctl(fd, TCGETS2, &tio);
	//tio.c_cflag &= ~CBAUD;
	tio.c_cflag = BOTHER | CS8 | CLOCAL | CREAD;
    tio.c_iflag = IGNPAR;
	tio.c_ispeed = speed;
	tio.c_ospeed = speed;
	return ioctl(fd, TCSETS2, &tio);
}

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
    setbaud(ser, 8000);

    // Initialise the keyboard controller
    Ikbd.Init();
    Ikbd.ResetChip(1);
    Ikbd.MouseCyclesPerTickX = 1000;
    Ikbd.MouseCyclesPerTickY = 1000;
    hd6301_run_cycles(0);

    // Keep track of time so we can get a fairly accurate clock cycle
    timespec last_time;
    timespec ms_time;
    unsigned long ms_count = 0;
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &last_time);
    ms_time.tv_nsec = 0;



    int count = 0;


    while (1)
    {
        // Calculate the time the last cycle took
        timespec cur_time;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cur_time);
        timespec diff_time = diff(last_time, cur_time);
        last_time = cur_time;
        ms_time.tv_nsec += diff_time.tv_nsec;

        if (ms_time.tv_nsec > 1000 * 1000) {
            ++count;
            hd6301_run_clocks(1000);
            handle_send_to_st();
            handle_rx_from_st();
            handle_keyboard();
            update_mouse();
            ms_time.tv_nsec = 0;
            ++ms_count;

            // 10ms handler
            if ((ms_count % 10) == 0) {
                handle_mouse();
            }
        }
    }

}
