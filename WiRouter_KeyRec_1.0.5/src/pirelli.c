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

#include <stdio.h>
#include <string.h>
#include "md5.h"
#include "pirelli.h"
#include "wirouterkeyrec.h"

void pirelli_calculate_wpa_key(unsigned char *extended_ssid, int debug, unsigned char *mac_out, Pirelli_config *config) {

    int i, t;

    unsigned char ssid[6],
                  hash[16],
                  wpa[5],
                  *ssid_id = NULL;

    MD5_CTX ctx;

    ssid_id = (unsigned char *) strrchr((char *)extended_ssid, '-');
    if(!ssid_id) {
        if(debug) PRINT_DEBUG("[%s] Invalid SSID.", extended_ssid);
        return;
    }

    ssid_id += 1;
    if(strlen((char *)ssid_id) != 12) {
        if(debug) PRINT_DEBUG("[%s] Invalid SSID ID.", extended_ssid);
        return;
    }

    /* Converting the SSID from ASCII to Byte */
    for(i=0; i<6; i++) {
        sscanf((char *)ssid_id, "%02x", &t);
        ssid[i] = mac_out[i] = t;
        ssid_id += 2;
    }

    /* Calculating the MD5 Hash */
    md5_starts(&ctx);
    md5_update(&ctx, ssid, 6);
    md5_update(&ctx, pirelli_header, sizeof(pirelli_header));
    md5_finish(&ctx, hash);

    /* Getting the first 5 bits from the 4 MSB of the MD5 hash */
    wpa[0] = (hash[0] >> 3) & 0x1f;
    wpa[1] = (((hash[0] & 7) << 2) | (hash[1] >> 6)) & 0x1f;
    wpa[2] = (hash[1] >> 1) & 0x1f;
    wpa[3] = (((hash[1] & 1) << 4) | (hash[2] >> 4)) & 0x1f;
    wpa[4] = (((hash[2] & 0xf) << 1) | (hash[3] >> 7)) & 0x1f;

    /* Adding 0x57 to each byte bigger than 0x0A */
    for(i=0; i<5; i++) if(wpa[i] >= 0x0A) wpa[i] += 0x57;

    sprintf((char *)config->wpa_key, "%02x%02x%02x%02x%02x", wpa[0], wpa[1], wpa[2], wpa[3], wpa[4]);

}
