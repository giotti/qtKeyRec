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
#include "telsey.h"
#include "hashword.h"
#include "wirouterkeyrec.h"

void telsey_calculate_wpa_key(unsigned char *extended_ssid, int debug, unsigned char *mac_out, Telsey_config *config) {
  
  unsigned int block[64], /* The block using for the permutation */
               i,         /* A generic counter */
               t,         /* A binary rappresentation of an ASCII value */
               s1,        /* The first part of the key */
               s2;        /* The second part of the key */
               
               
  unsigned char mac[6],
                *ssid_id = NULL;
		

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
    mac[i] = mac_out[i] = t;
    ssid_id += 2;
  }
                 
  
  /*
      Calculate the block value using the permutation box
  */
  memset(block, 0, sizeof(block));
  P_BOX(block, mac);
  
  /*
      Calculate the first part of the key
  */
  for(s1=0, i=0; i<64; i++) s1 = hashword(block, i, s1);
  
  /*
      Calculate the second part of the key
  */
  for(i=0; i<64; i++) {
    
    if(i < 8)
      block[i] <<= 3;
    else
      if(i < 16)
        block[i] >>= 5;
      else 
	if(i < 32)
	  block[i] >>= 2;
	else
	  block[i] <<= 7;
      
  }
  
  for(s2=0, i=0; i<64; i++) s2 = hashword(block, i, s2);
  
  /*
      The WPA key is the formed from the last two 20 bits of s1 and from the first 20 bits of s2
  */
  
  sprintf((char *)config->wpa_key, "%05x%05x", ((s1 << 12) >> 12),(s2 >> 12));

}


