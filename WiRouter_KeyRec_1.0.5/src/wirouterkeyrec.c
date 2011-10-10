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
#include <getopt.h>
#include <errno.h>
#include "wirouterkeyrec.h"
#include "agpf.h"
#include "socket.h"

extern int errno;

int wirouterkeyrec(int argc, char *argv[]) {

    int debug   = 0,
        verbose = 0,
        option  = 0,
        optidx  = 0,
        update  = 0;

    struct option long_options[] = {
      {"ssid",             1, 0, 's'},
      {"ssid_file",        1, 0, 'f'},
      {"config",           1, 0, 'c'},
      {"agpfurl",          1, 0, 'a'},
      {"updateagpf",       0, 0, 'u'},
      {"debug",            0, 0, 'd'},
      {"help",             0, 0, 'h'},
      {"verbose",          0, 0, 'v'},
      {NULL,               0, 0, 0}
    };

    unsigned char *temp          = NULL,
                  *agpf_host     = NULL,
                  *agpf_url      = NULL,
                  *agpf_web_page = (unsigned char *) AGPF_DEFAULT_WEB_PAGE,
                  wr_config_file[255];  /* Config File Name */

    Essid_list *net_list = NULL; /* Networks list */


    /*
        Print the usage
    */
    if(argc < 2) wr_usage(argv[0]);

    memset(wr_config_file, 0, sizeof(wr_config_file));


    while((option = getopt_long(argc, argv, "s:f:c:a:dhvu", long_options, &optidx)) > 0) {

        switch(option) {

          case 's':

            /*
                Multiple networks
            */
            if(strstr(optarg, ",")) {

              /* Parsing List */
              for(temp = (unsigned char *) strtok(optarg, ","); temp; temp = (unsigned char *) strtok(NULL, ","))
                  net_list = wr_essid_add(net_list, temp);

            }
            /*
                Just one network
            */
            else {
              net_list = wr_essid_add(net_list, (unsigned char *)optarg);
            }

            if(!net_list) PRINT_FATAL_ERROR("Unable to get a valid SSID list [Errno: %s].", strerror(errno));

          break;

          case 'f':

            /*
                Load networks from file
            */
            net_list = wr_load_essid_from_file(net_list, (unsigned char *)optarg);
            if(!net_list) PRINT_FATAL_ERROR("Unable to get a valid SSID list [Errno: %s].", strerror(errno));

          break;

          case 'c':

            /*
                Set a new configuration file
            */
            if(strlen(optarg) < 255)
              strncpy((char *)wr_config_file, optarg, sizeof(wr_config_file)-1);
            else
              PRINT_ERROR("File name too long. Will be used the default config file (%s)", AGPF_DEFAULT_CONFIG_FILE);

            if(wr_check_file_exist(wr_config_file) < 0)
              PRINT_FATAL_ERROR("Unable to open the configuration file [Errno: %s].", strerror(errno));

          break;

  
          case 'u':
	    
	    /*
	        Update the configuration file
	    */
	    
	    update = 1;
	    
          break;
	  
	  
	  case 'a':
	    
	    /*
	        Set the URL from where downloading the AGPF configuration file
	    */
	    
	    agpf_web_page = (unsigned char *)optarg;
	    wr_parsing_web_page(agpf_web_page, &agpf_host, &agpf_url);
	    
	    if(!agpf_host || !agpf_url)
	      PRINT_ERROR("Invalid URL. Will be used the default URL (%s)", AGPF_DEFAULT_WEB_PAGE);
	      	    
	  break;

          case 'd':
            debug = 1;
          break;

          case 'h':
            wr_usage(argv[0]);
          break;

          case 'v':
            verbose = 1;
          break;

          default:
            wr_usage(argv[0]);
          break;

        }

    }
    
    if(update) {
      
      PRINT_HEADER
      
     if(!agpf_host || !agpf_url) {
       agpf_web_page = (unsigned char *)AGPF_DEFAULT_WEB_PAGE;
       wr_parsing_web_page(agpf_web_page, &agpf_host, &agpf_url);
	      
       if(!agpf_host || !agpf_url)
	PRINT_FATAL_ERROR("Unable to use the default URL (%s)", AGPF_DEFAULT_WEB_PAGE);    
       }
	    
       if(!wr_check_file_exist((unsigned char *) AGPF_DEFAULT_CONFIG_FILE_DIR))	     
         agpf_update_config_file((unsigned char *)AGPF_DEFAULT_CONFIG_FILE_DIR, agpf_host, 80, agpf_url, NULL, NULL, debug); 
       else
	  agpf_update_config_file((unsigned char *)AGPF_DEFAULT_CONFIG_FILE, agpf_host, 80, agpf_url, NULL, NULL, debug);
      
      return 0;
      
    }
    
    if(!net_list) return -1;

    PRINT_HEADER

    net_list = wr_get_keys(net_list, wr_config_file, debug);

    /*
        Print the result
    */
    if(net_list) {
        wr_print_essid(net_list, verbose);
        net_list = wr_essid_free(net_list);
    }
    else {
        printf("\nThere are no WPA keys found.\n\n");
        return -1;
    }

    return 0;

}



/*
    vsprintf like function
*/

int vspr(char **buff, char *fmt, ...) {
	
    va_list ap;
    int     len,
            mlen;
    char    *ret = NULL;

    mlen = strlen(fmt) + 12800;
    va_start(ap, fmt);
    
    while(1) {
    	
        ret = (char *) malloc(mlen);
        if(!ret) return -1;
        
        len = vsnprintf(ret, mlen, fmt, ap);
        
        if((len >= 0) && (len < mlen)) break;
        
        mlen = len < 0 ? mlen+128 : len+1;
        
        free(ret);
        
    }
    
    va_end(ap);

    *buff = ret;
    
    return len;
    
}



/*
    WEB URL Parsing
*/

int wr_parsing_web_page(unsigned char *url, unsigned char **host, unsigned char **page) {

	unsigned char *tmp = NULL;

	if(strncasecmp((char *)url, "http://", 7) == 0) url += 7;
	if(!url) return -1;
	
	tmp = (unsigned char *) strdup((char *)url);
	if(!tmp) return -1;
	
	*host = (unsigned char *) strtok((char *)tmp, "/"); 
	if(!host) {
		free(tmp);
		return -1;
	}
	
	*page = url+strlen((char *)*host);
	if(!page) return -1;
		
	return 0;

}



/*
    Checks if filename is a valid file
*/

int wr_check_file_exist(unsigned char *filename) {

  FILE *fp = NULL;

  fp = fopen((char *)filename, "rb");
  if(!fp) return -1;

  fclose(fp);

  return 0;

}



/*
    Prints the usage menu
*/

void wr_usage(char *cmd) {

    PRINT_HEADER
    printf("usage: %s <options>"
           "\n"
           "-s  --ssid         Load an ESSID list from the command line [separated by comma]\n"
           "-f  --ssid_file    Load an ESSID list from a file\n"
           "-c  --config       AGPF configuration file (default: %s)\n"
           "-u  --updateagpf   Update the AGPF configuration file from internet\n"
           "-a  --agpfurl      Set the URL from where downloading the AGPF configuration file (default: my website)\n"
           "-d  --debug        Show debugging info\n"
           "-v  --verbose      Verbose mode\n"
           "-h  --help         Show this help\n"
           "\n"
           "Supported models: Alice AGPF, Fastweb Pirelli, Fastweb Telsey\n\n"
           "Example: %s -s Alice-58064234,FASTWEB-1-00036FB733FC\n\n", cmd, AGPF_DEFAULT_CONFIG_FILE, cmd);
    exit(0);

}



/*
    Checks the existence of the specified SSID in the specified list
*/

int wr_exist_ssid(Essid_list *first, unsigned char *ssid) {

  for(; first && strcasecmp((char *)first->ssid, (char *)ssid) != 0; first=first->next);

  return (first != NULL) ? -1 : 0;


}



/*
    Tests each network in the specified list and returns the complete list
*/

Essid_list *wr_get_keys(Essid_list *first, unsigned char *config_file, int debug) {

  Essid_list *record = NULL;

  for(record=first; record; record=record->next)
      switch(record->type) {

      case TYPE_AGPF:
            record->agpf = agpf_get_config(record->ssid, record->error, config_file, debug);
      break;

      case TYPE_PIRELLI:
            pirelli_calculate_wpa_key(record->ssid, debug, record->mac_wifi, &record->pirelli);
      break;

      case TYPE_TELSEY:
            telsey_calculate_wpa_key(record->ssid, debug, record->mac_wifi, &record->telsey);
      break;

      default:
            continue;
      break;


      }

  return first;


}



/*
    Loads SSID from the specified file and adds them to the specified list
*/

Essid_list *wr_load_essid_from_file(Essid_list *first, unsigned char *filename) {

  FILE *fp      = NULL;
  unsigned char *buffer = NULL;

  fp = fopen((char *)filename, "rb");
  if(!fp) return first;

  for(; (buffer = wr_myfgets(fp)); first = wr_essid_add(first, buffer));

  fclose(fp);

  return first;

}



/*
    Deletes the specified list
*/

Essid_list *wr_essid_free(Essid_list *first) {

    Essid_list *tmp = NULL;

    while(first) {
      tmp = first;
      first = first->next;
      if(tmp->agpf) tmp->agpf = agpf_config_free(tmp->agpf);
      free(tmp);
    }

    return first;

}



/*
    Creates a new record of Essid_list type and attach it at the end
    of the specified list
*/

Essid_list *wr_essid_add(Essid_list *first, unsigned char *in) {

    Essid_list *new = NULL,
               *tmp = NULL;

    unsigned char *ssid = NULL;
//                   *mac  = NULL;
    int type;

    ssid = in;
//     mac = (unsigned char *) strstr((char *)ssid, MAC_DELIMITER);
//     if(mac) {
//         mac[0] = 0x00;
//         mac++;
//     }
// 
    type = wr_get_type(ssid);

    if(type < 0 || wr_exist_ssid(first, ssid) < 0) return first;

//     if(type == TYPE_TELSEY && !mac) {
//         PRINT_ERROR("%s requires the MAC Address.", ssid);
//         return first;
//     }

    if(first) for(tmp=first; tmp->next; tmp=tmp->next);

    new = (Essid_list *) malloc(sizeof(Essid_list));
    if(!new) return first;

    memset(new->error, 0, ERROR_LENGTH);

//     if(mac) wr_ascii_mac_to_octet(mac, new->mac_wifi);
    strncpy((char *)new->ssid, (char *)ssid, MAX_SSID_LENGTH);
    new->type = type;
    new->next = NULL;

    if(first)
      tmp->next = new;
    else
      first = new;

    return first;

}



/*
    An optimized fgets function
*/

unsigned char *wr_myfgets(FILE *fd) {

    static unsigned char    cache[140];
    static int      cachesz = 0;
    static unsigned char    *ret    = NULL;
    static int      retsz   = 0;
    int     i,
        t,
        eol,
        ret_len;

    if(!fd) {
        cachesz = 0;
        return(NULL);
    }

    ret_len = 0;
    redo:
    eol = -1;
    do {
        if(cachesz <= 0) {
            cachesz = fread(cache, 1, sizeof(cache), fd);
            if(cachesz <= 0) {
                if(ret_len) eol = 0;
                break;
            }
        }

        for(i = 0; i < cachesz; i++) {
            if((cache[i] == '\n') || (cache[i] == '\r') || !cache[i]) {
                if(eol < 0) eol = i;
            } else if(eol >= 0) {
                break;
            }
        }

        t = (eol < 0) ? cachesz : eol;
        if((ret_len + t) >= retsz) {
            retsz = ((ret_len + t + 1) + 0xff) & (~0xff);
            ret = realloc(ret, retsz);
            if(!ret) return(NULL);
        }
        memcpy(ret + ret_len, cache, t);
        ret_len += t;

        cachesz -= i;
        memmove(cache, cache + i, cachesz);
        //for(t = 0; t < cachesz; t++) {
        //    cache[t] = cache[t + i];
        //}
    } while(eol < 0);

    if(eol < 0) return(NULL);
    if(!ret_len) goto redo;
    ret[ret_len] = 0;
    return(ret);

}



/*
    Prints in ASCII rappresentation the specified MAC Address
*/

void wr_print_mac(unsigned char mac[6]) {

  int i;

  for(i=0; i<6; i++)
    if(i == 5)
      printf("%02x", mac[i]);
    else
      printf("%02x:", mac[i]);

}



/*
    Prints the content of the specified AGPF_config list
*/

void wr_print_agpf_config(AGPF_config *first, int verbose) {

  for(; first; first=first->next) {
    printf("\n\nWPA:   %s"
           "\nMAC:   ", first->wpa_key);
    wr_print_mac(first->mac_eth);

    if(verbose)
        printf("\nMODEL: AGPF"
               "\nSN:    %s"
               "\nK:     %d"
               "\nQ:     %d"
               "\nSN1:   %d", first->serial, first->k, first->q, first->sn1);
  }

}



/*
    Prints the content of the specified Essid_list list
*/

void wr_print_essid(Essid_list *first, int verbose) {

  for(; first; first=first->next) {

    printf("---------------------------------------------\n"
           "ESSID: %s", first->ssid);

    switch(first->type) {

        case TYPE_AGPF:

        if(first->agpf)
          wr_print_agpf_config(first->agpf, verbose);
        else
            if(strlen((char *)first->error) > 0)
                printf("\n\n%s", first->error);

        break;

        case TYPE_PIRELLI:

            printf("\n\nWPA:   %s"
                   "\nMAC:   ", first->pirelli.wpa_key);
            wr_print_mac(first->mac_wifi);
            if(verbose) printf("\nMODEL: Pirelli");

        break;

        case TYPE_TELSEY:

            printf("\n\nWPA:   %s"
                   "\nMAC:   ", first->telsey.wpa_key);
            wr_print_mac(first->mac_wifi);

            if(verbose) printf("\nMODEL: Telsey");

        break;

        default:
        break;

    }

    printf("\n---------------------------------------------\n\n");

  }

}



/*
    Converts a MAC Address in ASCII rappresentation to Octet rappresentation
*/

void wr_ascii_mac_to_octet(unsigned char *mac_in, unsigned char mac_out[6]) {

  int i, t;
  unsigned char *tmp = NULL;

  for(i=0, tmp=mac_in; i<6; ++i) {

    sscanf((char *)tmp, "%02x", &t);
    tmp += 3;
    mac_out[i] = t;

  }


}



/*
    Checks if the SSID in input corresponds to a Router Alice
*/

int wr_is_alice(unsigned char *in) {

    int i;
    unsigned char *ssid = NULL;


    if(!in && strncasecmp((char *)in, "alice-", 6)) return -1;

    ssid = (unsigned char *) strrchr((char *)in, '-');

    if(strlen((char *)in) != 14 || !ssid || strlen((char *)ssid) != 9) return -1;

    ssid  += 1;

    for(i=0; ssid[i]; i++) if(!(ssid[i] >= 0x30 && ssid[i] <= 0x39)) return -1;

    return 0;


}



/*
    Checks if the SSID in input corresponds to a Router Pirelli
*/

int wr_is_fastweb_pirelli(unsigned char *in) {

    int i;
    unsigned char *ssid_id = NULL;


    if(strncasecmp((char *)in, "FASTWEB-", 8)) return -1;

    if(strlen((char *)in) != 22) return -1;

    ssid_id = (unsigned char *) strrchr((char *)in, '-');
    if(!ssid_id) return -1;

    ssid_id += 1;
    if(strlen((char *)ssid_id) != 12) return -1;

    /* Checking the SSID */
    for(i=0; i<12; i++)
        if(!((ssid_id[i] >= 0x41 && ssid_id[i] <= 0x46) ||
            (ssid_id[i] >= 0x61 && ssid_id[i] <= 0x66) ||
            (ssid_id[i] >= 0x30 && ssid_id[i] <= 0x39)
            )) return -1;

    if(strncasecmp((char *)ssid_id, "001CA2", 6) && strncasecmp((char *)ssid_id, "001DB", 5) && strncasecmp((char *)ssid_id, "001D8B", 6) && strncasecmp((char *)ssid_id, "38229D", 6)) return -1;

    return 0;

}



/*
    Checks if the SSID in input corresponds to a Router Telsey
*/

int wr_is_fastweb_telsey(unsigned char *in) {

    int i;
    unsigned char *ssid_id = NULL;


    if(strncasecmp((char *)in, "FASTWEB-", 8)) return -1;

    if(strlen((char *)in) != 22) return -1;

    ssid_id = (unsigned char *) strrchr((char *)in, '-');
    if(!ssid_id) return -1;

    ssid_id += 1;
    if(strlen((char *)ssid_id) != 12) return -1;

    /* Checking the SSID */
    for(i=0; i<12; i++)
        if(!((ssid_id[i] >= 0x41 && ssid_id[i] <= 0x46) ||
            (ssid_id[i] >= 0x61 && ssid_id[i] <= 0x66) ||
            (ssid_id[i] >= 0x30 && ssid_id[i] <= 0x39)
            )) return -1;

    if(strncasecmp((char *)ssid_id, "00036F", 6) && strncasecmp((char *)ssid_id, "002196", 6)) return -1;

    return 0;

}



/*
    Returns the router's model parsing the SSID.
*/

int wr_get_type(unsigned char *ssid) {

    if(!wr_is_alice(ssid))           return TYPE_AGPF;
    if(!wr_is_fastweb_pirelli(ssid)) return TYPE_PIRELLI;
    if(!wr_is_fastweb_telsey(ssid))  return TYPE_TELSEY;

    return -1;


}



char *grab(char *input, char *delone, char *deltwo, int history, int inclusion) {
    
    int len,
        delonelen = -1,
        deltwolen = -1;

    static char *s_start = NULL,
                *s_end   = NULL,
                *s_in    = NULL,
                *s_tmp   = NULL;

    char *start  = NULL,
         *end    = NULL,
         *tmp    = NULL,
         *in     = NULL,
         *buffer = NULL;
    
    if(delone) delonelen = strlen(delone);
    if(deltwo) deltwolen = strlen(deltwo);
    
    if(!input || delonelen <= 0 || deltwolen <= 0) return NULL;

    if(history) {
        in = s_in;
        tmp = s_tmp;
        start = s_start;
        end = s_end;
    }

    if(!in) {
        in = input;
        tmp = input;
    }

    if(strcmp(tmp, input) || (start && strcmp(start, delone)) || (end && strcmp(end, deltwo))) {
        in = input;
        tmp = input;
        start = end = NULL;
    }
    
    if(!(start = strstr(in, delone))) goto grab_quit;
        
    start += delonelen;
        
    if(!(end = strstr(start, deltwo))) goto grab_quit;
        
    if(inclusion == 1 || inclusion == 3) start -= delonelen;
        
    in = end+deltwolen;
        
    if(inclusion == 2 || inclusion == 3) end = in;
    
    len = end-start;
        
    if(!(buffer = (char *) malloc(len+1))) goto grab_quit;
        
    memcpy(buffer, start, len);
    buffer[len]=0;

    if(history) {
        start = delone;
        end = deltwo;
        s_in = in;
        s_tmp = tmp;
        s_start = start;
        s_end = end;
    }

    return buffer;
    
grab_quit:
    if(history) s_start = s_end = s_in = NULL;
    return NULL;

}

