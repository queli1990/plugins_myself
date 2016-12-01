/* ***** BEGIN LICENSE BLOCK *****
 * Distributed under the BSD license:
 *
 * Copyright (c) 2015-2016, Gochinatv
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of Gochinatv nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL Gochinatv BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ***** END LICENSE BLOCK ***** */

#include <stdlib.h>
#include "discovery.h"
#include "http.h"
#include "dial.h"

using namespace std;
using namespace dlna;

typedef Service::Result Result;

static void printServerList( vector<Service*> list )
{
  int i;
  vector<Service*>::iterator it;
  for( i = 0, it = list.begin(); it < list.end(); it++, i++ )
  {
    string uuid, name, macAddr;
    int wolTimeOut;
    Dial* dial = (*it)->as_dial();
    name = dial->get_friendly_name();
    uuid = dial->get_uuid();
    macAddr    = "";
    wolTimeOut = 0;
    const char* ip = dial->get_hostname().c_str();
    printf("%d: Server IP[%s] UUID[%s] FriendlyName[%s] MacAddress[%s] WakeOnLanTimeout[%d]\n",
           i+1, ip, uuid.c_str(), name.c_str(), macAddr.c_str(), wolTimeOut);
  }
}

static Dial* getServerFromUser( vector<Service*> list )
{
  Dial* dial = NULL;
  // show a list to the user
  char buf[80] = { 0 };
  vector<Service*>::iterator it;
  
  printf("Found Multiple servers\n");
  printf("0: Rescan and list DIAL servers\n");
  printServerList(list);
  printf("Enter server: ");
  scanf("%s", buf);
  
  unsigned int server = atoi(buf);
  
  if ( server > 0 && server <= list.size()) {
    dial = list[server - 1]->as_dial();
  } else {
    dial = NULL;
  }
  return dial;
}

static int handle_user(Discovery* discovery) {
  int processInputOuter = 1;
  char buf[80];
  
  while (processInputOuter) {
    discovery->scan();
    std::this_thread::sleep_for(std::chrono::seconds(10)); // sleep s
    discovery->stop();
    
    vector<Service*> list = discovery->service(Discovery::DIAL);
    if ( list.size() == 0 ) {
      printf("\nNo servers available\n");
      return 1;
    }
    
    Dial* dial = getServerFromUser( list );
    if ( dial == NULL ) {
      continue;
    }
    
    int processInput = 1;
    
    while ( processInput ) {
      string payload = "ABCDEFG";
      string netflix = "Netflix";
      string youtube = "YouTube";
      
      memset(buf, 0, 80);
      printf("0. Rescan and list DIAL servers\n");
      printf("1. Launch Netflix\n");
      printf("2. Hide Netflix\n");
      printf("3. Stop Netflix\n");
      printf("4. Netflix status\n");
      printf("5. Launch YouTube\n");
      printf("6. Hide YouTube\n");
      printf("7. Stop YouTube\n");
      printf("8. YouTube status\n");
      printf("9. QUIT\n");
      printf("Command (0:1:2:3:4:5:6:7:8:9): ");
      scanf("%s", buf);
      
      switch( atoi(buf) )
      {
        case 0:
          processInput = 0;
          break;
        case 1:
          printf("Launch Netflix\n");
          dial->launch( netflix, payload );
          break;
        case 2:
          printf("Hide Netflix\n");
          dial->hide( netflix );
          break;
        case 3:
          printf("Stop Netflix\n");
          dial->stop( netflix );
          break;
        case 4: {
          printf("Netflix Status: \n");
          Result r = dial->status( netflix );
          if ( ! r.code ) {
            printf("RESPONSE: \n%s\n", r.data["body"].GetString());
          }
          break;
        }
        case 5:
          printf("Launch YouTube\n");
          dial->launch( youtube, payload );
          break;
        case 6:
          printf("Hide YouTube\n");
          dial->hide( youtube );
          break;
        case 7:
          printf("Stop YouTube\n");
          dial->stop( youtube );
          break;
        case 8: {
          printf("YouTube Status: \n");
          Result r = dial->status( youtube );
          if ( ! r.code ) {
            printf("RESPONSE: \n%s\n", r.data["body"].GetString());
          }
          break;
        }
        case 9:
          processInput = 0;
          processInputOuter = 0;
          break;
        default:
          printf("Invalid, try again\n");
      }
    }
  }
  return 0;
}

int main(int argc, char* argv[]) {
  
  parse_args(argc, argv);
  
  Discovery discovery;
  
  return handle_user(&discovery);
}

