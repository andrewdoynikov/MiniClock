#ifndef INTERFACE_H
#define INTERFACE_H
//=============================================================================
void run_main(unsigned char event);
void run_edit_time(unsigned char event);
void set_blink(void);
void KBD_scan(void);
//=============================================================================
void show_time(void);
void show_hour(unsigned char value);
void show_min(unsigned char value);
//=============================================================================
#endif
