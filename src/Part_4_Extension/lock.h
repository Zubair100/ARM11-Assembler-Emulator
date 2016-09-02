#define LOCK 6

#define DONGLE_MAC_ADDRESS "00:15:83:3D:0A:57"
#define VALID_MAC_ADDRESS "6C:2F:2C:7D:26:15"

#define MAX_CONNS 255
#define ENQUIRY_WAIT_TIME 8
#define DELAY_TIME 500

#define Open_socket(x) \
  int dongle_id = hci_devid(x); \
  int socket_no = hci_open_dev(dongle_id); \
  if (dongle_id < 0 || socket_no < 0) { \
      perror("Unable to open socket to bluetooth dongle."); \
      exit(1); \
  } \
  do {} while(0)

#define Get_device_address() \
  char curr_addr[19]; \
  ba2str(&(info+i)->bdaddr, curr_addr)

#define Is_valid_address(x) \
  strcmp(x, VALID_MAC_ADDRESS) == 0
