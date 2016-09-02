#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <wiringPi.h> // EXTERNAL LIBRARY
#include "bluetooth.h" // EXTERNAL LIBRARY
#include "hci.h" // EXTERNAL LIBRARY
#include "hci_lib.h" // EXTERNAL LIBRARY
#include "lock.h"

int main(int argc, char **argv) {

  wiringPiSetup();
  pinMode(LOCK, OUTPUT); // SET LOCK PIN AS OUTPUT
  digitalWrite(LOCK, 1); // Set lock on

  // Open a socket to specified bluetooth dongle.
  Open_socket(DONGLE_MAC_ADDRESS);

  // info is an array of inquiry_info structs.
  // Will hold information about each available bluetooth device.
  inquiry_info *info = (inquiry_info*) malloc(MAX_CONNS * sizeof(inquiry_info));

  while (1) {
    int no_of_conns = hci_inquiry(dongle_id, ENQUIRY_WAIT_TIME, MAX_CONNS,
                                                NULL, &info, IREQ_CACHE_FLUSH);
    if (no_of_conns < 0) {
      perror("Error scanning for bluetooth devices.");
    }

    for (int i = 0; i < no_of_conns; i++) {
      Get_device_address(); // Sets curr_addr variable

      if (Is_valid_address(curr_addr)) {
        digitalWrite(LOCK, 0); // Turn the lock off
        delay(DELAY_TIME);
        digitalWrite(LOCK, 1); // lock on
      }
    }

    delay(DELAY_TIME);
  }

  free(info);
  close(socket_no);
}
