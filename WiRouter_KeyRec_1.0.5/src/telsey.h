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

#ifndef TELSEY_H
#define TELSEY_H


#define P_BOX(out, mac) \
{ \
  out[0]   = mac[5] << 24 | mac[1] << 16 | mac[0] << 8 | mac[5]; \
  out[1]   = mac[1] << 24 | mac[0] << 16 | mac[1] << 8 | mac[5]; \
  out[2]   = mac[4] << 24 | mac[2] << 16 | mac[3] << 8 | mac[2]; \
  out[3]   = mac[4] << 24 | mac[3] << 16 | mac[2] << 8 | mac[2]; \
  out[4]   = mac[2] << 24 | mac[4] << 16 | mac[2] << 8 | mac[0]; \
  out[5]   = mac[2] << 24 | mac[5] << 16 | mac[3] << 8 | mac[1]; \
  out[6]   = mac[0] << 24 | mac[4] << 16 | mac[0] << 8 | mac[1]; \
  out[7]   = mac[1] << 24 | mac[4] << 16 | mac[1] << 8 | mac[0]; \
  \
  out[8]   = mac[2] << 24 | mac[4] << 16 | mac[2] << 8 | mac[2]; \
  out[9]   = mac[3] << 24 | mac[1] << 16 | mac[3] << 8 | mac[4]; \
  out[10]  = mac[4] << 24 | mac[1] << 16 | mac[4] << 8 | mac[3]; \
  out[11]  = mac[5] << 24 | mac[1] << 16 | mac[5] << 8 | mac[5]; \
  out[12]  = mac[2] << 24 | mac[1] << 16 | mac[0] << 8 | mac[5]; \
  out[13]  = mac[1] << 24 | mac[0] << 16 | mac[1] << 8 | mac[1]; \
  out[14]  = mac[4] << 24 | mac[2] << 16 | mac[1] << 8 | mac[3]; \
  out[15]  = mac[3] << 24 | mac[3] << 16 | mac[5] << 8 | mac[2]; \
  \
  out[16]  = mac[4] << 24 | mac[4] << 16 | mac[5] << 8 | mac[4]; \
  out[17]  = mac[5] << 24 | mac[1] << 16 | mac[4] << 8 | mac[0]; \
  out[18]  = mac[2] << 24 | mac[5] << 16 | mac[0] << 8 | mac[5]; \
  out[19]  = mac[2] << 24 | mac[1] << 16 | mac[3] << 8 | mac[5]; \
  out[20]  = mac[5] << 24 | mac[2] << 16 | mac[2] << 8 | mac[4]; \
  out[21]  = mac[2] << 24 | mac[3] << 16 | mac[1] << 8 | mac[4]; \
  out[22]  = mac[0] << 24 | mac[4] << 16 | mac[4] << 8 | mac[3]; \
  out[23]  = mac[3] << 24 | mac[0] << 16 | mac[5] << 8 | mac[3]; \
  \
  out[24]  = mac[4] << 24 | mac[3] << 16 | mac[0] << 8 | mac[0]; \
  out[25]  = mac[3] << 24 | mac[2] << 16 | mac[1] << 8 | mac[1]; \
  out[26]  = mac[2] << 24 | mac[1] << 16 | mac[2] << 8 | mac[5]; \
  out[27]  = mac[1] << 24 | mac[3] << 16 | mac[4] << 8 | mac[3]; \
  out[28]  = mac[0] << 24 | mac[2] << 16 | mac[3] << 8 | mac[4]; \
  out[29]  = mac[0] << 24 | mac[0] << 16 | mac[2] << 8 | mac[2]; \
  out[30]  = mac[0] << 24 | mac[0] << 16 | mac[0] << 8 | mac[5]; \
  out[31]  = mac[1] << 24 | mac[1] << 16 | mac[1] << 8 | mac[4]; \
  \
  out[32]  = mac[4] << 24 | mac[0] << 16 | mac[2] << 8 | mac[2]; \
  out[33]  = mac[3] << 24 | mac[3] << 16 | mac[3] << 8 | mac[0]; \
  out[34]  = mac[0] << 24 | mac[2] << 16 | mac[4] << 8 | mac[1]; \
  out[35]  = mac[5] << 24 | mac[5] << 16 | mac[5] << 8 | mac[0]; \
  out[36]  = mac[0] << 24 | mac[4] << 16 | mac[5] << 8 | mac[0]; \
  out[37]  = mac[1] << 24 | mac[1] << 16 | mac[5] << 8 | mac[2]; \
  out[38]  = mac[2] << 24 | mac[2] << 16 | mac[5] << 8 | mac[1]; \
  out[39]  = mac[3] << 24 | mac[3] << 16 | mac[2] << 8 | mac[3]; \
  \
  out[40]  = mac[1] << 24 | mac[0] << 16 | mac[2] << 8 | mac[4]; \
  out[41]  = mac[1] << 24 | mac[5] << 16 | mac[2] << 8 | mac[5]; \
  out[42]  = mac[0] << 24 | mac[1] << 16 | mac[4] << 8 | mac[0]; \
  out[43]  = mac[1] << 24 | mac[1] << 16 | mac[1] << 8 | mac[4]; \
  out[44]  = mac[2] << 24 | mac[2] << 16 | mac[2] << 8 | mac[2]; \
  out[45]  = mac[3] << 24 | mac[3] << 16 | mac[3] << 8 | mac[3]; \
  out[46]  = mac[5] << 24 | mac[4] << 16 | mac[0] << 8 | mac[1]; \
  out[47]  = mac[4] << 24 | mac[0] << 16 | mac[5] << 8 | mac[5]; \
  \
  out[48]  = mac[1] << 24 | mac[0] << 16 | mac[5] << 8 | mac[0]; \
  out[49]  = mac[0] << 24 | mac[1] << 16 | mac[5] << 8 | mac[1]; \
  out[50]  = mac[2] << 24 | mac[2] << 16 | mac[4] << 8 | mac[2]; \
  out[51]  = mac[3] << 24 | mac[4] << 16 | mac[4] << 8 | mac[3]; \
  out[52]  = mac[4] << 24 | mac[3] << 16 | mac[1] << 8 | mac[5]; \
  out[53]  = mac[5] << 24 | mac[5] << 16 | mac[1] << 8 | mac[4]; \
  out[54]  = mac[3] << 24 | mac[0] << 16 | mac[1] << 8 | mac[5]; \
  out[55]  = mac[3] << 24 | mac[1] << 16 | mac[0] << 8 | mac[4]; \
  \
  out[56]  = mac[4] << 24 | mac[2] << 16 | mac[2] << 8 | mac[5]; \
  out[57]  = mac[4] << 24 | mac[3] << 16 | mac[3] << 8 | mac[1]; \
  out[58]  = mac[2] << 24 | mac[4] << 16 | mac[3] << 8 | mac[0]; \
  out[59]  = mac[2] << 24 | mac[3] << 16 | mac[5] << 8 | mac[1]; \
  out[60]  = mac[3] << 24 | mac[1] << 16 | mac[2] << 8 | mac[3]; \
  out[61]  = mac[5] << 24 | mac[0] << 16 | mac[1] << 8 | mac[2]; \
  out[62]  = mac[5] << 24 | mac[3] << 16 | mac[4] << 8 | mac[1]; \
  out[63]  = mac[0] << 24 | mac[2] << 16 | mac[3] << 8 | mac[0]; \
  \
}


typedef struct telsey_net_conf {

    unsigned char wpa_key[10];

} Telsey_config;


void telsey_calculate_wpa_key(unsigned char *extended_ssid, int debug, unsigned char *mac_out, Telsey_config *config);


#endif /* TELSEY_H */
