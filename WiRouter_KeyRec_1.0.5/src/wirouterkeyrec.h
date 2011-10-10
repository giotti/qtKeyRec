/*

    WiRouter KeyRec - Salvatore Fresta

    Contact: salvatorefresta@gmail.com

    Copyright 2010-2011 Salvatore Fresta aka Drosophila

    This program is free software; you can redistribute it and/or
    modify it under the terms of  the  GNU General Public License
    as published by the  Free Software Foundation; either version
    2 of the License, or (at your option) any later version.

    This program  is  distributed  in the hope  that  it  will be
    useful, but WITHOUT ANY WARRANTY;  without  even the  implied
    warranty  of  MERCHANTABILITY  or  FITNESS  FOR  A PARTICULAR
    PURPOSE. See the GNU General Public License for more details.

    You should have  received a copy  of  the  GNU General Public
    License along  with  this program;  if not, write to the Free
    Software Foundation,Inc., 59 Temple Place, Suite 330, Boston,
    MA 02111-1307 USA

    http://www.gnu.org/licenses/gpl-2.0.txt

*/

#ifndef WIROUTERKEYREC_H
#define WIROUTERKEYREC_H

#include <stdarg.h>
#include "telsey.h"
#include "pirelli.h"
#include "agpf.h"


#ifdef WIN32
    #define strncasecmp strnicmp
    #define strcasecmp  stricmp
    #define strcasestr  stristr
#endif



#define ERROR_LENGTH        256
#define MAX_SSID_LENGTH     23
#define SOFTWARE_NAME       "WiRouter KeyRec"
#define SOFTWARE_VERSION    "1.0.5"



#define PRINT_HEADER printf("\n%s %s - (C) 2011 Salvatore Fresta" \
                            "\nhttp://www.salvatorefresta.net\n\n", SOFTWARE_NAME, SOFTWARE_VERSION);

// #define MAC_DELIMITER "/"


#define TYPE_AGPF     0
#define TYPE_PIRELLI  1
#define TYPE_TELSEY   2



#define PRINT_FATAL_ERROR(...) {        \
    fprintf(stderr, "\nFATAL ERROR: ");        \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n\n");      \
    exit(-1);                     \
}



#define PRINT_ERROR(...) {        \
    fprintf(stderr, "\nERROR: ");        \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n\n");      \
}



#define PRINT_DEBUG(...) {        \
    fprintf(stderr, "DEBUG: ");        \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n");      \
}



/*
    The following dynamic list contains the information about the tested networks
    such as the SSID and the list of the its own configurations.
*/

typedef struct essid_list {

    unsigned char ssid[MAX_SSID_LENGTH]; /* Network's name */
    unsigned char mac_wifi[6];           /* Network's MAC Address */
    unsigned char error[ERROR_LENGTH];   /* A possible error */
    int type;                            /* Router's model */

    AGPF_config    *agpf;                /* A pointer to the list of AGPF configurations */
    Pirelli_config pirelli;              /* Pirelli configuration */
    Telsey_config  telsey;               /* Telsey configuration */

    struct essid_list *next;             /* Pointer to the next network */

} Essid_list;


void wr_ascii_mac_to_octet(unsigned char *mac_in, unsigned char mac_out[6]);
void wr_print_essid(Essid_list *first, int verbose);
void wr_print_agpf_config(AGPF_config *first, int verbose);
void wr_usage(char *cmd);
void wr_print_mac(unsigned char mac[6]);
int vspr(char **buff, char *fmt, ...);
int wr_parsing_web_page(unsigned char *url, unsigned char **host, unsigned char **page);
int wr_check_file_exist(unsigned char *filename);
int wr_exist_ssid(Essid_list *first, unsigned char *ssid);
int wr_get_type(unsigned char *ssid);
int wr_is_alice(unsigned char *in);
int wr_is_fastweb_pirelli(unsigned char *in);
int wr_is_fastweb_telsey(unsigned char *in);
unsigned char *wr_myfgets(FILE *fd);
char *grab(char *input, char *delone, char *deltwo, int history, int inclusion);
Essid_list *wr_essid_add(Essid_list *first, unsigned char *ssid);
Essid_list *wr_essid_free(Essid_list *first);
Essid_list *wr_load_essid_from_file(Essid_list *first, unsigned char *filename);
Essid_list *wr_get_keys(Essid_list *first, unsigned char *config_file, int debug);


#endif /* WIROUTERKEYREC_H */
