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

#ifndef PIRELLI_H
#define PIRELLI_H


static const unsigned char pirelli_header[] = {

        0x22,0x33,0x11,0x34,0x02,0x81,0xFA,
        0x22,0x11,0x41,0x68,0x11,0x12,0x01,
        0x05,0x22,0x71,0x42,0x10,0x66

};


typedef struct pirelli_net_conf {

    unsigned char wpa_key[11];

} Pirelli_config;


void pirelli_calculate_wpa_key(unsigned char *extended_ssid, int debug, unsigned char *mac_out, Pirelli_config *config);


#endif /* PIRELLI_H */
